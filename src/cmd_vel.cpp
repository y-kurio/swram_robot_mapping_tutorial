#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/Float64.h>
#include <cmath>
#include <nav_msgs/Odometry.h>
#include <tf/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2/LinearMath/Quaternion.h>
geometry_msgs::Pose target_goal;


class WheelController {
private:
    ros::NodeHandle nh;
    ros::Subscriber pose_sub;
    ros::Publisher cmd_vel_pub;
    
    double goal_x, goal_y;
    double kp_linear, kp_angular;
    double wheel_base, wheel_radius;
    
public:
    WheelController() {
        nh.param("goal_x", goal_x, 1.0);
        nh.param("goal_y", goal_y, 1.0);
        nh.param("kp_linear", kp_linear, 1.0);
        nh.param("kp_angular", kp_angular, 2.0);
        nh.param("wheel_base", wheel_base, 0.5);
        nh.param("wheel_radius", wheel_radius, 0.1);
        
        pose_sub = nh.subscribe("/pose", 10, &WheelController::poseCallback, this);
        cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    }
    
void encoderCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    odomdata_ = *msg;
    odom_point.header = odomdata_.header;
    odom_point.point = odomdata_.pose.pose.position;

    geometry_msgs::PoseStamped pre_point;
    pre_point.header = odomdata_.header;
    pre_point.pose = odomdata_.pose.pose;
    
    geometry_msgs::PoseStamped pose_out;
    geometry_msgs::TransformStamped transformStamped;
    geometry_msgs::Twist cmd_vel;
    cmd_vel.linear.x = current_linear_vel;
    cmd_vel.angular.z = current_angular_vel;
    
    try
    {
        transformStamped = tf_buffer_.lookupTransform(FRAME_ROBOT_BASE, pre_point.header.frame_id, pre_point.header.stamp, ros::Duration(0.0));
    }
    catch (tf2::TransformException &ex) 
    {
        // ROS_INFO("Random value: %f", random_value);
        ROS_WARN_STREAM("get_tf TF2 exception: " << ex.what());
    }
    
    tf2::doTransform(pre_point, pose_out, transformStamped);

    d_goal.position.x = target_goal.position.x - pose_out.pose.position.x;
    d_goal.position.y = target_goal.position.y - pose_out.pose.position.y;
    d_goal.position.z = target_goal.position.z - pose_out.pose.position.z;

    d_goal.orientation.x = target_goal.orientation.x - target_goal.orientation.x;
    d_goal.orientation.y = target_goal.orientation.y - target_goal.orientation.y;
    d_goal.orientation.z = target_goal.orientation.z - target_goal.orientation.z;
    d_goal.orientation.w = target_goal.orientation.w - target_goal.orientation.w;

    goal_hani_ = sqrt(pow(d_goal.position.x , 2) + pow(d_goal.position.y , 2));
    if (goal_hani_ < goal_seigen_)
    {
        std::cout << "toutyaku!!!!!!" << std::endl;

        cmd_vel.linear.x = 0.0;
        cmd_vel.angular.z = 0.0;
    }else 
    {
        
    }

}

    void poseCallback(const geometry_msgs::Pose2D::ConstPtr& msg) {
        double dx = goal_x - msg->x;
        double dy = goal_y - msg->y;
        double distance = std::sqrt(dx * dx + dy * dy);
        double angle_to_goal = std::atan2(dy, dx);
        double angle_error = angle_to_goal - msg->theta;
        
        geometry_msgs::Twist cmd_vel;
        cmd_vel.linear.x = kp_linear * distance;
        cmd_vel.angular.z = kp_angular * angle_error;
        
        cmd_vel_pub.publish(cmd_vel);
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "wheel_controller");
    WheelController controller;
    ros::Subscriber encoder_sub = nh.subscribe("/main/odom", 10, encoderCallback);
    ros::Subscriber goal_action_sub = nh.subscribe("move_base/status", 10, goalactionCallback);
    newgoal_pub = nh.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 10);
    ros::Subscriber sub_encoder_sub = nh.subscribe("odom", 10, sub_encoderCallback);
    ros::spin();
    
    return 0;
}
