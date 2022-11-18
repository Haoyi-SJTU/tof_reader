cmake_minimum_required(VERSION 3.0.2)
project(tof)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  geometry_msgs
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp rospy geometry_msgs message_runtime
  # DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  # ${PROJECT_SOURCE_DIR}/include
)

ADD_EXECUTABLE(tof_talker src/serialRead.cpp)
TARGET_LINK_LIBRARIES(tof_talker ${catkin_LIBRARIES} )
# ADD_DEPENDENCIES(tof_talker ${${PROJECT_NAME}_EXPORTED_TARGETS})