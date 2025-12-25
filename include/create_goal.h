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
#include <visualization_msgs/Marker.h>
#include <cmath>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/Quaternion.h>
#include <sensor_msgs/PointCloud2.h>
#include <vector>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Int8.h>
#include <swram_robot_mapping_tutorial/cluster_data.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <Eigen/Dense>

std::string FRAME_ROBOT_BASE;
double param1,param2;
std_msgs::Int8 goal_status;
tf2_ros::Buffer tf_buffer_;
geometry_msgs::PointStamped odom_point;
geometry_msgs::PoseStamped target_point;
geometry_msgs::PointStamped sub_odom_point;
geometry_msgs::PoseStamped sub_target_point;
geometry_msgs::PoseStamped sub_goal;
actionlib_msgs::GoalStatusArray action_data_, pre_action_data_;
nav_msgs::Odometry odomdata_,sub_odomdata_;
double idoutyou_,move_pose_,move_pose_x_,move_pose_y_,pre_dis,newposdis,x_,y_,x_1_,x_2_,SSS_,goal,currentpos_x,currentpos_y;
ros::Publisher newgoal_pub;
ros::Publisher marker_pub,marker_sub_goal_pub;
double MIN_kyori_x, MIN_kyori_y, MIN_kyori_z, MIN_kyori;
int number, angle_degrees;
double random_angle;
geometry_msgs::Pose group_radius;
geometry_msgs::PoseStamped sub_pose_out;
    geometry_msgs::PoseStamped pose_out;
geometry_msgs::TransformStamped sub_transformStamped;
        Eigen::Matrix2d A;
        Eigen::Vector2d pre_x;
        Eigen::Vector2d new_x;
        double myu_;
        double sigma_;
        double Vyy, Vxx, Vxy;
        double bunnsann,theta, d_theta, d_robot_kyori, d_kyori;
// static bool is_variable_set = false;

// PointStamped型の配列を格納するベクター
std::vector<geometry_msgs::PointStamped> point_msg_array_;
std::vector<std::string> cluster_no_;
swram_robot_mapping_tutorial::cluster_data cluster_result;