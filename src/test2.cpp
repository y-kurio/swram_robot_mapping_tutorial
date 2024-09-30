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
#include <tf2/LinearMath/Quaternion.h>

std::string FRAME_ROBOT_BASE;
tf2_ros::Buffer tf_buffer_;
geometry_msgs::PointStamped odom_point;
geometry_msgs::PoseStamped target_point;
nav_msgs::Odometry odomdata;


void encoderCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    odomdata = *msg;
    // 0からRAND_MAXまでの整数を生成し、0.0〜1.0の範囲にスケール
    double random_value = static_cast<double>(rand()) / RAND_MAX;
    ROS_INFO("Random value: %f", random_value);
    
    odom_point.header = odomdata.header;
    odom_point.point = odomdata.pose.pose.position;

    geometry_msgs::PoseStamped pre_point;
    pre_point.header = odomdata.header;
    pre_point.pose = odomdata.pose.pose;
    
    geometry_msgs::PoseStamped pose_out;
    geometry_msgs::TransformStamped transformStamped;
    
    try
    {
        transformStamped = tf_buffer_.lookupTransform(FRAME_ROBOT_BASE, pre_point.header.frame_id, pre_point.header.stamp, ros::Duration(0.0));
    }
    catch (tf2::TransformException &ex) 
    {
        ROS_INFO("失敗");
        ROS_WARN_STREAM("get_tf TF2 exception: " << ex.what());
    }
    
    tf2::doTransform(pre_point, pose_out, transformStamped);
    
        // ROS_INFO("FRAME_ROBOT_BASE=%s",FRAME_ROBOT_BASE);
        std::cout << "x: " << pose_out.pose.position.x << std::endl;
        std::cout << "y: " << pose_out.pose.position.y << std::endl;
    
    idoutyou_ = sqrt(-2 * param1 * param1 * log(-(random_value - 1)));
    x_ = idoutyou*random_value;
    y_ = idoutyou*sqrt(1-random_value^2);
    move_pose_x_ = pose_out.pose.position.x + x_;
    move_pose_y_ = pose_out.pose.position.x + y_;
    move_pose_ = sqrt(x_^2 + y_^2);
    pre_dis = sqrt((pose_out.pose.position.x - centerRow)^2 + (pose_out.pose.position.y - centerCol)^2);
    newposdis = sqrt((x_ - centerRow)^2 + (y_ - centerCol)^2);
    x_1_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((pre_dis - param2)^2) / (2 * (param1)^2));
    x_2_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((newposdis - param2)^2) / (2 * (param1)^2));
    SSS_ = x_2 / x_1;
            
    //  SSSの条件分岐
    if( SSS_ > random_value )
    {
        // ロボットの位置を更新
        goal = x_;
        currentpos_x = x_
    }
    else if
    {
        x_ = random;
        y_ = sqrt(1-x^2);
        move_pose_x_ = currentpos_x + x_;
        move_pose_y_ = currentpos_y + y_;
        move_pose_ = sqrt(x_^2 + y_^2);
        pre_dis = sqrt((pose_out.pose.position.x - centerRow)^2 + (pose_out.pose.position.y - centerCol)^2);
        newposdis = sqrt((x_ - centerRow)^2 + (y_ - centerCol)^2);
        x_1_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((pre_dis - param2)^2) / (2 * (param1)^2));
        x_2_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((newposdis - param2)^2) / (2 * (param1)^2));
    }
        

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "aaa");
    ros::NodeHandle nh;
    ros::NodeHandle n("~");
    n.getParam("FRAME/ROBOT_BASE",FRAME_ROBOT_BASE);
    n.getParam("HEIKITI",param1);
    n.getParam("BUNNSANN",param2);
    static tf2_ros::TransformListener tfListener(tf_buffer_);
    // シードを設定（現在時刻を使う）
    srand(time(NULL));
    
    // エンコーダデータをサブスクライブ
    ros::Subscriber encoder_sub = nh.subscribe("/robot_2/odom", 10, encoderCallback);
    
    ros::spin();
    return 0;
}