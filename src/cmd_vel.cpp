#include <comandliner.h>



void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg) //ゴール位置の受け取り
{
    target_goal = *msg;
}


void encoderCallback(const nav_msgs::Odometry::ConstPtr& msg)//メインロボットの自己位置取得
{
    odomdata_ = *msg;
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー自己位置のtf変換
    odom_point.header = odomdata_.header;
    odom_point.point = odomdata_.pose.pose.position;

    geometry_msgs::PoseStamped pre_point;
    pre_point.header = odomdata_.header;
    pre_point.pose = odomdata_.pose.pose;
    
    geometry_msgs::PoseStamped pose_out;
    geometry_msgs::TransformStamped transformStamped;
    geometry_msgs::Twist cmd_vel;
    
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
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーゴール位置との相対位置を計算
    d_goal.pose.position.x = target_goal.pose.position.x - pose_out.pose.position.x;
    d_goal.pose.position.y = target_goal.pose.position.y - pose_out.pose.position.y;
    d_goal.pose.position.z = target_goal.pose.position.z - pose_out.pose.position.z;

    d_goal.pose.orientation.x = target_goal.pose.orientation.x - pose_out.pose.orientation.x;
    d_goal.pose.orientation.y = target_goal.pose.orientation.y - pose_out.pose.orientation.y;
    d_goal.pose.orientation.z = target_goal.pose.orientation.z - pose_out.pose.orientation.z;
    d_goal.pose.orientation.w = target_goal.pose.orientation.w - pose_out.pose.orientation.w;
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

    goal_hani_ = sqrt(pow(d_goal.pose.position.x , 2) + pow(d_goal.pose.position.y , 2));//ゴール位置との相対距離計さん

    tf2::Quaternion q(pose_out.pose.orientation.x, pose_out.pose.orientation.y, pose_out.pose.orientation.z, pose_out.pose.orientation.w);
    // オイラー角の格納変数 (roll, pitch, yaw)
    double roll, pitch, yaw;
    // クオータニオンをオイラー角に変換
    tf2::Matrix3x3 m(q);
    m.getRPY(roll, pitch, yaw);
    // 目標地点への角度 (ラジアン)
    double angle_to_goal = atan2(target_goal.pose.position.y - pose_out.pose.position.y, target_goal.pose.position.x - pose_out.pose.position.x);

    // 角度誤差
    double angle_error = angle_to_goal - yaw;
    if (goal_hani_ < goal_torelanse)//閾値との相対距離比較
    {
        std::cout << "toutyaku!!!!!!" << std::endl;

        cmd_vel.linear.x = 0.0;
        cmd_vel.linear.y = 0.0;
        cmd_vel.linear.z = 0.0;
        cmd_vel.angular.x = 0.0;
        cmd_vel.angular.y = 0.0;
        cmd_vel.angular.z = 0.0;
        goal_status.data = 0;
        goal_status_pub.publish(goal_status);
        cmd_vel_pub.publish(cmd_vel);
    }
    if (goal_hani_ > goal_torelanse)
    {
        if (fabs(angle_error) > 0.09)
        {
            // 角度誤差が大きい場合は旋回
            cmd_vel.linear.x = 0.0;
            cmd_vel.linear.y = 0.0;
            cmd_vel.linear.z = 0.0;
            cmd_vel.angular.x = 0.0;
            cmd_vel.angular.y = 0.0;
            cmd_vel.angular.z = 0.1 * angle_error;
            cmd_vel_pub.publish(cmd_vel);
        } else 
        {
            cmd_vel.linear.x = 0.2;
            cmd_vel.linear.y = 0.0;
            cmd_vel.linear.z = 0.0;
            cmd_vel.angular.x = 0.0;
            cmd_vel.angular.y = 0.0;
            cmd_vel.angular.z = 0.0;
            cmd_vel_pub.publish(cmd_vel);
        }
        goal_status.data = 1;
        goal_status_pub.publish(goal_status);
    }
}


int main(int argc, char** argv) {
    ros::init(argc, argv, "wheel_controller");
    ros::NodeHandle nh;
    ros::NodeHandle n("~");
    n.getParam("FRAME/ROBOT_BASE",FRAME_ROBOT_BASE);
    n.getParam("goal_torelanse",goal_torelanse);
    static tf2_ros::TransformListener tfListener(tf_buffer_);
    ros::Subscriber goal_sub = nh.subscribe("sub_goal_position", 10, poseCallback);
    ros::Subscriber encoder_sub = nh.subscribe("odom", 10, encoderCallback);
    cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    goal_status_pub = nh.advertise<std_msgs::Int8>("goal_status", 10);

    ros::spin();
    return 0;
}
