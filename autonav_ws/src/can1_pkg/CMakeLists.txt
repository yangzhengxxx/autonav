cmake_minimum_required(VERSION 3.0.2)
project(can1_pkg)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  nav_msgs
  geometry_msgs
  tf2
  tf2_ros
)

catkin_package(
  CATKIN_DEPENDS roscpp nav_msgs geometry_msgs tf2 tf2_ros
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(can1_receiver_node src/can1_receiver_node.cpp)
target_link_libraries(can1_receiver_node ${catkin_LIBRARIES})
add_dependencies(can1_receiver_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_executable(can1_cmd_vel src/can1_cmd_vel_node.cpp)
target_link_libraries(can1_cmd_vel ${catkin_LIBRARIES})
add_dependencies(can1_cmd_vel ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

install(TARGETS can1_receiver_node can1_cmd_vel
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(PROGRAMS
  scripts/scan_downsampler.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

install(DIRECTORY config/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config
)

install(DIRECTORY rviz/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/rviz
)

