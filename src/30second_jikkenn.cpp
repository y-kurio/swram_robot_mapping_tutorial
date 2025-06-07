#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <tf2/utils.h>
#include <cmath>

geometry_msgs::Pose current_pose_;
geometry_msgs::PoseStamped goal_pose_;
int goal_received_;
double goal_tolerance_, linear_gain_, angular_gain_, angle;
ros::Publisher cmd_vel_pub_;

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg) {
    current_pose_ = msg->pose.pose;
    if (goal_received_ == 1) {
        double dx = goal_pose_.pose.position.x - current_pose_.position.x;
    double dy = goal_pose_.pose.position.y - current_pose_.position.y;

    double distance = std::sqrt(dx * dx + dy * dy);
    // if (distance < goal_tolerance_) {
    if (distance < 1.0) {
        // 目的地に到達したら停止
        geometry_msgs::Twist stop;
        cmd_vel_pub_.publish(stop);
        ROS_INFO("Goal reached!");
        return;
    }

    double yaw = tf2::getYaw(current_pose_.orientation);
    double target_angle = std::atan2(dy, dx);
    angle = (target_angle - yaw);
    while (angle > M_PI) angle -= 2 * M_PI;
    while (angle < -M_PI) angle += 2 * M_PI;
    double angle_diff = angle;

    geometry_msgs::Twist cmd;
    // cmd.linear.x = linear_gain_ * distance;
    // cmd.angular.z = angular_gain_ * angle_diff;
    cmd.linear.x = 0.01 * distance;
    cmd.angular.z = 0.1 * angle_diff;
    cmd_vel_pub_.publish(cmd);
    }
}

void goalCallback(const geometry_msgs::PoseStamped::ConstPtr& msg) {
    goal_pose_ = *msg;
    goal_received_ = 1;
    ROS_INFO("Received new goal: (%.2f, %.2f)", goal_pose_.pose.position.x, goal_pose_.pose.position.y);
}


int main(int argc, char** argv) {
    ros::init(argc, argv, "go_to_goal_controller");
    ros::NodeHandle nh_;
    ros::NodeHandle n("~");
    n.getParam("goal_tolerance_",goal_tolerance_);
    n.getParam("linear_gain_",linear_gain_);
    n.getParam("angular_gain_",angular_gain_);
    ros::Subscriber odom_sub_;
    ros::Subscriber goal_sub_;
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/main/cmd_vel", 10);
    odom_sub_ = nh_.subscribe("/main/odom", 10, odomCallback);
    goal_sub_ = nh_.subscribe("/main/goal", 10, goalCallback);
    ros::spin();
    return 0;
}
