#include <comandliner.h>

void goalpublisher()
{
    newgoal_pub.publish(sub_goal);
    ROS_INFO("publish OK!!!" );
}

void goalposeCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
    goal_pose_ = *msg;
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
    
    try
    {
        transformStamped = tf_buffer_.lookupTransform(FRAME_ROBOT_BASE, pre_point.header.frame_id, pre_point.header.stamp, ros::Duration(0.0));
    }
    catch (tf2::TransformException &ex) 
    {
        // ROS_INFO("Random value: %f", random_value);
        ROS_WARN_STREAM("get_tf TF2 exception: " << ex.what());
    }

    // 現在位置とゴール位置を取得
    double x = pose_out.pose.position.x;
    double y = pose_out.pose.position.y;
    double goal_x = goal_pose_.position.x;
    double goal_y = goal_pose_.position.y;

    // ゴールまでの距離を計算
    double distance = sqrt(pow(goal_x - x, 2) + pow(goal_y - y, 2));
    if (distance < goal_tolerance_) {
        ROS_INFO("Goal reached!");

        // ロボットを停止
        cmd_vel_msg.linear.x = 0;
        cmd_vel_msg.linear.y = 0;
        cmd_vel_msg.linear.z = 0;  // 目標までの距離に比例した速度
        cmd_vel_msg.angular.x = 0;
        cmd_vel_msg.angular.y = 0;
        cmd_vel_msg.angular.z = 0;  // 角度差に応じた回転速度

        cmd_vel_pub_.publish(cmd_vel_msg);
        return;
    }

    // ゴールへの方向を計算
    double angle_to_goal = atan2(goal_y - y, goal_x - x);
    double yaw = tf::getYaw(goal_pose_.orientation);
    double angle_diff = angle_to_goal - yaw;

    // 角度差を正規化 (-pi ~ pi)
    angle_diff = atan2(sin(angle_diff), cos(angle_diff));

    // 速度指令を生成
    geometry_msgs::Twist cmd_vel_msg;
    cmd_vel_msg.linear.x = std::min(max_linear_speed_, distance);  // 目標までの距離に比例した速度
    cmd_vel_msg.linear.y = 0;
    cmd_vel_msg.linear.z = 0; 
    cmd_vel_msg.angular.x = 0;
    cmd_vel_msg.angular.y = 0;
    cmd_vel_msg.angular.z = std::min(max_angular_speed_, std::max(-max_angular_speed_, angle_diff));  // 角度差に応じた回転速度

    // 速度をパブリッシュ
    cmd_vel_pub_.publish(cmd_vel_msg);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "aaa");
    ros::NodeHandle nh;
    ros::NodeHandle n("~");
    n.getParam("FRAME/ROBOT_BASE",FRAME_ROBOT_BASE);
    static tf2_ros::TransformListener tfListener(tf_buffer_);
    // エンコーダデータをサブスクライブ
    ros::Subscriber encoder_sub = nh.subscribe("/main/odom", 10, encoderCallback);
    ros::Subscriber goal_action_sub = nh.subscribe("sub_goal", 10, goalposeCallback);
    new_vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    ros::spin();
    return 0;
}