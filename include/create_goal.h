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

std::string FRAME_ROBOT_BASE;
double param1,param2;
tf2_ros::Buffer tf_buffer_;
geometry_msgs::PointStamped odom_point;
geometry_msgs::PoseStamped target_point;
geometry_msgs::PoseStamped sub_goal;
actionlib_msgs::GoalStatusArray action_data_;
nav_msgs::Odometry odomdata_;
double idoutyou_,move_pose_,move_pose_x_,move_pose_y_,pre_dis,newposdis,x_,y_,x_1_,x_2_,SSS_,goal,currentpos_x,currentpos_y;
ros::Publisher newgoal_pub;

    // 変数がセット済みかどうかを確認するフラグ
    bool is_variable_set = false;

