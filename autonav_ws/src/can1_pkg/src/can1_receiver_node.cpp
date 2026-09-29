#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <string>

using SteadyClock = std::chrono::steady_clock;

class Can1ReceiverNode
{
public:
  Can1ReceiverNode()
  : nh_("~"),
    sock_(-1),
    x_(0.0),
    y_(0.0),
    yaw_(0.0),
    have_last_integrate_(false),
    wz_bias_(0.0),
    gz_bias_(0.0),
    bias_sum_wz_(0.0),
    bias_sum_gz_(0.0),
    bias_samples_(0),
    bias_ready_(false)
  {
    nh_.param<std::string>("interface", interface_, "can1");
    nh_.param<int>("can_id", can_id_, 0x011);
    nh_.param<double>("alpha", alpha_, 0.3);
    nh_.param<std::string>("odom_frame", odom_frame_, "odom");
    nh_.param<std::string>("base_frame", base_frame_, "base_link");
    nh_.param<std::string>("odom_topic", odom_topic_, "/odom");
    nh_.param<bool>("publish_tf", publish_tf_, true);

    nh_.param<bool>("enable_bias_calib", enable_bias_calib_, true);
    nh_.param<double>("bias_calib_duration", bias_calib_duration_, 2.0);
    nh_.param<double>("angular_deadzone", angular_deadzone_, 0.02);
    nh_.param<double>("linear_still_thresh", linear_still_thresh_, 0.02);
    nh_.param<bool>("online_bias_update", online_bias_update_, true);
    nh_.param<double>("online_bias_alpha", online_bias_alpha_, 0.002);

    if (alpha_ < 0.0 || alpha_ > 1.0)
    {
      ROS_WARN("alpha=%.3f is outside [0,1] and has been clamped", alpha_);
      alpha_ = std::max(0.0, std::min(1.0, alpha_));
    }
    bias_calib_duration_ = std::max(0.5, bias_calib_duration_);
    angular_deadzone_ = std::max(0.0, angular_deadzone_);
    linear_still_thresh_ = std::max(0.0, linear_still_thresh_);
    online_bias_alpha_ = std::max(0.0, std::min(1.0, online_bias_alpha_));

    if (!enable_bias_calib_)
    {
      bias_ready_ = true;
    }

    odom_pub_ = nh_.advertise<nav_msgs::Odometry>(odom_topic_, 50);

    if (!openSocket())
    {
      throw std::runtime_error("Failed to open SocketCAN interface: " + interface_);
    }

    ROS_INFO(
      "can1_receiver_node started: iface=%s id=0x%03X alpha=%.2f "
      "bias_calib=%s(%.1fs) deadzone=%.3f still_v=%.3f "
      "odom_topic=%s frames=%s->%s publish_tf=%s",
      interface_.c_str(), can_id_, alpha_,
      enable_bias_calib_ ? "on" : "off", bias_calib_duration_,
      angular_deadzone_, linear_still_thresh_,
      odom_topic_.c_str(), odom_frame_.c_str(), base_frame_.c_str(),
      publish_tf_ ? "true" : "false");

    if (enable_bias_calib_)
    {
      ROS_WARN(
        "Keep the vehicle stationary for approximately %.1f s while angular velocity biases are estimated...",
        bias_calib_duration_);
    }
  }

  ~Can1ReceiverNode()
  {
    if (sock_ >= 0)
    {
      close(sock_);
      sock_ = -1;
    }
  }

  void spin()
  {
    can_frame frame{};
    ros::Rate idle(100.0);

    while (ros::ok())
    {

      const ssize_t nbytes = read(sock_, &frame, sizeof(frame));
      if (nbytes < 0)
      {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
          ros::spinOnce();
          idle.sleep();
          continue;
        }
        ROS_ERROR_THROTTLE(1.0, "Failed to read from CAN: %s", std::strerror(errno));
        ros::spinOnce();
        idle.sleep();
        continue;
      }

      if (static_cast<size_t>(nbytes) < sizeof(can_frame))
      {
        continue;
      }

      can_frame latest = frame;
      bool have_latest = isTargetFrame(latest);

      while (true)
      {
        const ssize_t n = recv(sock_, &frame, sizeof(frame), MSG_DONTWAIT);
        if (n < 0)
        {
          break;
        }
        if (static_cast<size_t>(n) >= sizeof(can_frame) && isTargetFrame(frame))
        {
          latest = frame;
          have_latest = true;
        }
      }

      if (have_latest)
      {
        processFrame(latest);
      }
      ros::spinOnce();
    }
  }

private:
  bool openSocket()
  {
    sock_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock_ < 0)
    {
      ROS_ERROR("socket(PF_CAN) failed: %s", std::strerror(errno));
      return false;
    }

    ifreq ifr{};
    std::snprintf(ifr.ifr_name, IFNAMSIZ, "%s", interface_.c_str());
    if (ioctl(sock_, SIOCGIFINDEX, &ifr) < 0)
    {
      ROS_ERROR("SIOCGIFINDEX(%s) failed: %s", interface_.c_str(), std::strerror(errno));
      close(sock_);
      sock_ = -1;
      return false;
    }

    sockaddr_can addr{};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    can_filter filter{};
    filter.can_id = static_cast<canid_t>(can_id_);
    filter.can_mask = CAN_SFF_MASK;
    if (setsockopt(sock_, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter)) < 0)
    {
      ROS_WARN("Failed to set the CAN filter: %s", std::strerror(errno));
    }

    timeval tv{};
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    if (bind(sock_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
      ROS_ERROR("bind(%s) failed: %s", interface_.c_str(), std::strerror(errno));
      close(sock_);
      sock_ = -1;
      return false;
    }
    return true;
  }

  bool isTargetFrame(const can_frame& frame) const
  {
    const canid_t id = frame.can_id & CAN_SFF_MASK;
    return id == static_cast<canid_t>(can_id_) && frame.can_dlc >= 8;
  }

  static int16_t readInt16LE(const uint8_t* data)
  {
    return static_cast<int16_t>(
      static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1]) << 8));
  }

  static double applyDeadzone(double value, double dz)
  {
    return (std::fabs(value) < dz) ? 0.0 : value;
  }

  bool updateBias(double vx, double vy, double wz, double gz, const ros::Time& stamp)
  {
    if (!enable_bias_calib_)
    {
      return true;
    }

    const double speed = std::hypot(vx, vy);
    const bool still = speed < linear_still_thresh_;

    if (!bias_ready_)
    {
      if (!still)
      {
        ROS_WARN_THROTTLE(
          1.0, "Motion detected during bias calibration (v=%.3f); keep the vehicle stationary", speed);
        return false;
      }

      if (bias_samples_ == 0)
      {
        bias_start_stamp_ = stamp;
      }

      bias_sum_wz_ += wz;
      bias_sum_gz_ += gz;
      ++bias_samples_;

      const double elapsed = (stamp - bias_start_stamp_).toSec();
      if (elapsed >= bias_calib_duration_ && bias_samples_ >= 10)
      {
        wz_bias_ = bias_sum_wz_ / static_cast<double>(bias_samples_);
        gz_bias_ = bias_sum_gz_ / static_cast<double>(bias_samples_);
        bias_ready_ = true;

        x_ = 0.0;
        y_ = 0.0;
        yaw_ = 0.0;
        have_last_integrate_ = false;
        ROS_INFO(
          "Angular velocity bias calibration complete: samples=%d "
          "wz_bias=%.4f gz_bias=%.4f rad/s (approximately %.2f deg/s). "
          "Pose reset; the vehicle may now move.",
          bias_samples_, wz_bias_, gz_bias_,
          gz_bias_ * 180.0 / M_PI);
      }
      return false;
    }

    if (online_bias_update_ && still &&
        std::fabs(wz) < (angular_deadzone_ * 3.0) &&
        std::fabs(gz) < (angular_deadzone_ * 3.0))
    {
      wz_bias_ = (1.0 - online_bias_alpha_) * wz_bias_ + online_bias_alpha_ * wz;
      gz_bias_ = (1.0 - online_bias_alpha_) * gz_bias_ + online_bias_alpha_ * gz;
    }
    return true;
  }

  void processFrame(const can_frame& frame)
  {
    const int16_t vx_i = readInt16LE(&frame.data[0]);
    const int16_t vy_i = readInt16LE(&frame.data[2]);
    const int16_t wz_i = readInt16LE(&frame.data[4]);
    const int16_t gz_i = readInt16LE(&frame.data[6]);

    const double vx = vx_i / 1000.0;
    const double vy = vy_i / 1000.0;
    const double wz_raw = wz_i / 1000.0;
    const double gz_raw = gz_i / 1000.0;

    const ros::Time stamp = ros::Time::now();
    const bool ready = updateBias(vx, vy, wz_raw, gz_raw, stamp);

    const double wz = wz_raw - wz_bias_;
    const double gz = gz_raw - gz_bias_;
    double wz_fused = alpha_ * gz + (1.0 - alpha_) * wz;
    wz_fused = applyDeadzone(wz_fused, angular_deadzone_);

    double vx_i_int = vx;
    double vy_i_int = vy;
    if (std::hypot(vx, vy) < linear_still_thresh_)
    {
      vx_i_int = 0.0;
      vy_i_int = 0.0;
    }

    ROS_DEBUG_THROTTLE(
      1.0,
      "raw wz=%.4f gz=%.4f | bias wz=%.4f gz=%.4f | fused=%.4f ready=%d",
      wz_raw, gz_raw, wz_bias_, gz_bias_, wz_fused, ready ? 1 : 0);

    if (!ready)
    {

      publishOdom(stamp, 0.0, 0.0, 0.0);
      return;
    }

    const SteadyClock::time_point t = SteadyClock::now();
    double dt = 0.0;
    if (have_last_integrate_)
    {
      dt = std::chrono::duration<double>(t - last_integrate_).count();
    }
    last_integrate_ = t;
    have_last_integrate_ = true;

    if (dt > 0.0 && dt < 1.0)
    {
      const double cos_y = std::cos(yaw_);
      const double sin_y = std::sin(yaw_);
      x_ += (vx_i_int * cos_y - vy_i_int * sin_y) * dt;
      y_ += (vx_i_int * sin_y + vy_i_int * cos_y) * dt;
      yaw_ += wz_fused * dt;
      yaw_ = normalizeAngle(yaw_);
    }

    publishOdom(stamp, vx_i_int, vy_i_int, wz_fused);
  }

  void publishOdom(const ros::Time& stamp, double vx, double vy, double wz_fused)
  {
    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, yaw_);

    nav_msgs::Odometry odom;
    odom.header.stamp = stamp;
    odom.header.frame_id = odom_frame_;
    odom.child_frame_id = base_frame_;
    odom.pose.pose.position.x = x_;
    odom.pose.pose.position.y = y_;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation.x = q.x();
    odom.pose.pose.orientation.y = q.y();
    odom.pose.pose.orientation.z = q.z();
    odom.pose.pose.orientation.w = q.w();
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.linear.z = 0.0;
    odom.twist.twist.angular.x = 0.0;
    odom.twist.twist.angular.y = 0.0;
    odom.twist.twist.angular.z = wz_fused;

    for (int i = 0; i < 36; ++i)
    {
      odom.pose.covariance[i] = 0.0;
      odom.twist.covariance[i] = 0.0;
    }
    odom.pose.covariance[0] = 0.05;
    odom.pose.covariance[7] = 0.05;
    odom.pose.covariance[35] = 0.1;
    odom.twist.covariance[0] = 0.02;
    odom.twist.covariance[7] = 0.02;
    odom.twist.covariance[35] = 0.05;

    odom_pub_.publish(odom);

    if (publish_tf_)
    {
      geometry_msgs::TransformStamped tf_msg;
      tf_msg.header.stamp = stamp;
      tf_msg.header.frame_id = odom_frame_;
      tf_msg.child_frame_id = base_frame_;
      tf_msg.transform.translation.x = x_;
      tf_msg.transform.translation.y = y_;
      tf_msg.transform.translation.z = 0.0;
      tf_msg.transform.rotation.x = q.x();
      tf_msg.transform.rotation.y = q.y();
      tf_msg.transform.rotation.z = q.z();
      tf_msg.transform.rotation.w = q.w();
      tf_broadcaster_.sendTransform(tf_msg);
    }
  }

  static double normalizeAngle(double a)
  {
    while (a > M_PI)
    {
      a -= 2.0 * M_PI;
    }
    while (a < -M_PI)
    {
      a += 2.0 * M_PI;
    }
    return a;
  }

  ros::NodeHandle nh_;
  ros::Publisher odom_pub_;
  tf2_ros::TransformBroadcaster tf_broadcaster_;

  std::string interface_;
  int can_id_;
  double alpha_;
  std::string odom_frame_;
  std::string base_frame_;
  std::string odom_topic_;
  bool publish_tf_;

  bool enable_bias_calib_;
  double bias_calib_duration_;
  double angular_deadzone_;
  double linear_still_thresh_;
  bool online_bias_update_;
  double online_bias_alpha_;

  int sock_;
  double x_;
  double y_;
  double yaw_;
  SteadyClock::time_point last_integrate_;
  bool have_last_integrate_;

  double wz_bias_;
  double gz_bias_;
  double bias_sum_wz_;
  double bias_sum_gz_;
  int bias_samples_;
  bool bias_ready_;
  ros::Time bias_start_stamp_;
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "can1_receiver_node");
  try
  {
    Can1ReceiverNode node;
    node.spin();
  }
  catch (const std::exception& ex)
  {
    ROS_FATAL("can1_receiver_node failed to start: %s", ex.what());
    return 1;
  }
  return 0;
}
