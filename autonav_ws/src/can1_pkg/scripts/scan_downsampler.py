#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Uniformly downsample LaserScan messages to reduce mapping and localization load."""

import rospy
from sensor_msgs.msg import LaserScan

class ScanDownsampler(object):
  def __init__(self):
    self.decimate = int(rospy.get_param("~decimate", 10))
    if self.decimate < 1:
      rospy.logwarn("Invalid decimate=%d; using 1 instead", self.decimate)
      self.decimate = 1

    self.pub = rospy.Publisher("scan_out", LaserScan, queue_size=1)
    rospy.Subscriber("scan_in", LaserScan, self.callback, queue_size=1)
    rospy.loginfo(
      "scan_downsampler started: decimate=%d  scan_in -> scan_out",
      self.decimate,
    )

  def callback(self, msg):
    out = LaserScan()
    out.header = msg.header
    out.angle_min = msg.angle_min
    out.angle_max = msg.angle_max
    out.scan_time = msg.scan_time
    out.range_min = msg.range_min
    out.range_max = msg.range_max
    out.ranges = list(msg.ranges[:: self.decimate])
    if msg.intensities:
      out.intensities = list(msg.intensities[:: self.decimate])

    n = len(out.ranges)
    if n > 1:
      out.angle_increment = (msg.angle_max - msg.angle_min) / float(n - 1)
      out.time_increment = msg.time_increment * self.decimate
    else:
      out.angle_increment = msg.angle_increment * self.decimate
      out.time_increment = msg.time_increment

    self.pub.publish(out)

def main():
  rospy.init_node("scan_downsampler")
  ScanDownsampler()
  rospy.spin()

if __name__ == "__main__":
  main()
