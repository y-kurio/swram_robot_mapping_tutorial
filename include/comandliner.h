#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Point.h>
#include <math.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/PolygonStamped.h>
#include <visualization_msgs/MarkerArray.h>
#include <string>
#include <actionlib_msgs/GoalStatusArray.h>
#include <geometry_msgs/PoseStamped.h>
#include <iostream>
#include <random>
#include <cmath>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/Float64.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <std_msgs/Int8.h>


std::string FRAME_ROBOT_BASE;
double goal_torelanse;
geometry_msgs::Twist cmd_vel;
std_msgs::Int8 goal_status;
tf2_ros::Buffer tf_buffer_;
geometry_msgs::PointStamped odom_point;
geometry_msgs::PoseStamped target_point;
geometry_msgs::PoseStamped target_goal, d_goal;
nav_msgs::Odometry odomdata_;
double goal_hani_;
ros::Publisher cmd_vel_pub;
ros::Publisher goal_status_pub;
// static bool is_variable_set = false;

