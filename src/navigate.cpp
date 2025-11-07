#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>

std::vector<geometry_msgs::Point> waypoints;


geometry_msgs::Point current_pos;
double current_yaw = 0.0;
int target_index = 0;
bool pose_ready = false;

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    current_pos = msg->pose.pose.position;
    tf::Quaternion q(
        msg->pose.pose.orientation.x,
        msg->pose.pose.orientation.y,
        msg->pose.pose.orientation.z,
        msg->pose.pose.orientation.w
    );
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    current_yaw = yaw;
    pose_ready = true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "path_follower");
    ros::NodeHandle nh;

    ros::Subscriber odom_sub = nh.subscribe("/main/odom", 10, odomCallback);
    ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("/main/cmd_vel", 10);

    ros::Rate rate(10);
    double kv = 0.5; // 前進ゲイン
    double kw = 1.0; // 旋回ゲイン
        geometry_msgs::Point p;
        p.x = 2.0; p.y = 0.0; p.z = 0.0; waypoints.push_back(p);
        p.x = 6.0; p.y = -4.0; p.z = 0.0; waypoints.push_back(p);
        p.x = 7.0; p.y = -4.0; p.z = 0.0; waypoints.push_back(p);
        p.x = 7.0; p.y = 5.0; p.z = 0.0; waypoints.push_back(p);
        p.x = 6.5; p.y = 10.5; p.z = 0.0; waypoints.push_back(p);

    while (ros::ok())
    {
        ros::spinOnce();


        if (!pose_ready) { rate.sleep(); continue; }

        geometry_msgs::Point target = waypoints[target_index];

        double dx = target.x - current_pos.x;
        double dy = target.y - current_pos.y;
        double distance = sqrt(dx*dx + dy*dy);
        double target_yaw = atan2(dy, dx);
        double yaw_error = target_yaw - current_yaw;

        // -pi〜piの範囲に正規化
        while (yaw_error > M_PI) yaw_error -= 2*M_PI;
        while (yaw_error < -M_PI) yaw_error += 2*M_PI;

        geometry_msgs::Twist cmd;
        cmd.linear.x = std::min(0.05, kv * distance);
        if(yaw_error > 0)
        {
            cmd.angular.z = std::min(0.157, kw * yaw_error);
        }else{
            cmd.angular.z = std::min(-0.157, kw * yaw_error);
        }

        // 終点判定
        if (distance < 0.1) {
            target_index++;
            if (target_index >= waypoints.size()) {
                cmd.linear.x = 0.0;
                cmd.angular.z = 0.0;
                cmd_pub.publish(cmd);
                ROS_INFO("Reached final goal!");
                break;
            }
        }

        cmd_pub.publish(cmd);
        rate.sleep();
    }

    return 0;
}
