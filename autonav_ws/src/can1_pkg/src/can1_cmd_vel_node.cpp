#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <mutex>
#include <stdexcept>
#include <string>

namespace
{
int16_t toInt16Scaled(double value, double scale, int16_t abs_limit)
{
  const double scaled = value * scale;
  const int v = static_cast<int>(std::lround(scaled));
  const int clamped = std::max<int>(-abs_limit, std::min<int>(abs_limit, v));
  return static_cast<int16_t>(clamped);
}

void writeInt16LE(uint8_t* dst, int16_t value)
{
  dst[0] = static_cast<uint8_t>(value & 0xFF);
  dst[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
}
}

class Can1CmdVelNode
{
public:
  Can1CmdVelNode()
  : nh_("~"),
    sock_(-1),
    vx_(0.0),
    vy_(0.0),
    wz_(0.0),
    have_cmd_(false)
  {
    nh_.param<std::string>("interface", interface_, "can1");
    nh_.param<int>("can_id", can_id_, 0x010);
    nh_.param<std::string>("cmd_vel_topic", cmd_vel_topic_, "/cmd_vel");
    nh_.param<bool>("enable", enable_, false);
    nh_.param<double>("rate_hz", rate_hz_, 20.0);
    nh_.param<double>("cmd_timeout", cmd_timeout_, 0.3);
    nh_.param<double>("max_vx", max_vx_, 0.3);
    nh_.param<double>("max_vy", max_vy_, 0.2);
    nh_.param<double>("max_wz", max_wz_, 0.6);

    rate_hz_ = std::max(1.0, rate_hz_);
    cmd_timeout_ = std::max(0.05, cmd_timeout_);
    max_vx_ = std::max(0.0, max_vx_);
    max_vy_ = std::max(0.0, max_vy_);
    max_wz_ = std::max(0.0, max_wz_);

    if (can_id_ < 0 || can_id_ > 0x7FF)
    {
      throw std::runtime_error("can_id is outside the standard frame range 0~0x7FF");
    }

    if (!openSocket())
    {
      throw std::runtime_error("Failed to open SocketCAN interface: " + interface_);
    }

    cmd_sub_ = nh_.subscribe(cmd_vel_topic_, 10, &Can1CmdVelNode::cmdVelCallback, this);
    timer_ = nh_.createTimer(
      ros::Duration(1.0 / rate_hz_), &Can1CmdVelNode::onTimer, this);

    if (enable_)
    {
      sendVelocity(0.0, 0.0, 0.0);
    }

    ROS_INFO(
      "can1_cmd_vel started: iface=%s id=0x%03X topic=%s enable=%s "
      "rate=%.1f Hz timeout=%.2fs limit vx=%.2f vy=%.2f wz=%.2f",
      interface_.c_str(), can_id_, cmd_vel_topic_.c_str(),
      enable_ ? "true" : "false",
      rate_hz_, cmd_timeout_, max_vx_, max_vy_, max_wz_);

    if (!enable_)
    {
      ROS_WARN("enable=false: subscribed to /cmd_vel, but CAN transmission is disabled; set enable:=true for navigation");
    }
  }

  ~Can1CmdVelNode()
  {
    if (sock_ >= 0 && enable_)
    {
      sendVelocity(0.0, 0.0, 0.0);
    }
    if (sock_ >= 0)
    {
      close(sock_);
      sock_ = -1;
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

    if (bind(sock_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
      ROS_ERROR("bind(%s) failed: %s", interface_.c_str(), std::strerror(errno));
      close(sock_);
      sock_ = -1;
      return false;
    }
    return true;
  }

  void cmdVelCallback(const geometry_msgs::Twist::ConstPtr& msg)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    vx_ = clamp(msg->linear.x, max_vx_);
    vy_ = clamp(msg->linear.y, max_vy_);
    wz_ = clamp(msg->angular.z, max_wz_);
    last_cmd_stamp_ = ros::Time::now();
    have_cmd_ = true;
  }

  void onTimer(const ros::TimerEvent&)
  {
    if (!enable_)
    {
      return;
    }

    double vx = 0.0;
    double vy = 0.0;
    double wz = 0.0;

    {
      std::lock_guard<std::mutex> lock(mutex_);
      if (have_cmd_)
      {
        const double age = (ros::Time::now() - last_cmd_stamp_).toSec();
        if (age <= cmd_timeout_)
        {
          vx = vx_;
          vy = vy_;
          wz = wz_;
        }
      }
    }

    sendVelocity(vx, vy, wz);
  }

  static double clamp(double v, double abs_max)
  {
    return std::max(-abs_max, std::min(abs_max, v));
  }

  void sendVelocity(double vx, double vy, double wz)
  {
    can_frame frame{};
    frame.can_id = static_cast<canid_t>(can_id_);
    frame.can_dlc = 8;

    const int16_t vx_i = toInt16Scaled(vx, 1000.0, 30000);
    const int16_t vy_i = toInt16Scaled(vy, 1000.0, 30000);
    const int16_t wz_i = toInt16Scaled(wz, 1000.0, 30000);

    writeInt16LE(&frame.data[0], vx_i);
    writeInt16LE(&frame.data[2], vy_i);
    writeInt16LE(&frame.data[4], wz_i);
    frame.data[6] = 0;
    frame.data[7] = 0;

    const ssize_t n = write(sock_, &frame, sizeof(frame));
    if (n != static_cast<ssize_t>(sizeof(frame)))
    {
      ROS_ERROR_THROTTLE(
        1.0, "Failed to send CAN frame (%s): %s", interface_.c_str(), std::strerror(errno));
    }
  }

  ros::NodeHandle nh_;
  ros::Subscriber cmd_sub_;
  ros::Timer timer_;

  std::string interface_;
  int can_id_;
  std::string cmd_vel_topic_;
  bool enable_;
  double rate_hz_;
  double cmd_timeout_;
  double max_vx_;
  double max_vy_;
  double max_wz_;

  int sock_;

  std::mutex mutex_;
  double vx_;
  double vy_;
  double wz_;
  ros::Time last_cmd_stamp_;
  bool have_cmd_;
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "can1_cmd_vel");
  try
  {
    Can1CmdVelNode node;
    ros::spin();
  }
  catch (const std::exception& ex)
  {
    ROS_FATAL("can1_cmd_vel failed to start: %s", ex.what());
    return 1;
  }
  return 0;
}
