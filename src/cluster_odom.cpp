// #include <ros/ros.h>
// #include <sensor_msgs/PointCloud2.h>
// #include <vector>
// #include <pcl_conversions/pcl_conversions.h>
// #include <pcl/point_cloud.h>
// #include <pcl/point_types.h>
// #include <tf/transform_listener.h>
// #include <tf2_ros/buffer.h>
// #include <geometry_msgs/PointStamped.h>
// #include <geometry_msgs/PolygonStamped.h>
// #include <tf2/LinearMath/Quaternion.h>
// // #include <std_msgs/String>

// tf2_ros::Buffer tf_buffer_;
// std::string FRAME_ROBOT_BASE;

// void clusterCallback(const sensor_msgs::PointCloud2::ConstPtr& msg) {
//     // PointCloud2をPCL形式に変換
//     pcl::PointCloud<pcl::PointXYZI>::Ptr pcl_cloud(new pcl::PointCloud<pcl::PointXYZI>);
//     pcl::fromROSMsg(*msg, *pcl_cloud);

//     // PointStamped型の配列を格納するベクター
//     std::vector<geometry_msgs::PointStamped> point_msg_array;
//     std::vector<std::string> cluster_no;

//     std::string cloud_frame_id = pcl_cloud->header.frame_id;
//     // 受け取った全てのクラスタの座標を表示し、PointStampedに格納
//     for (const auto& point : pcl_cloud->points) {
//         geometry_msgs::PointStamped point_msg;
//         std::string no;
//         point_msg.header = pcl_conversions::fromPCL(pcl_cloud->header);
//         point_msg.point.x = point.x;
//         point_msg.point.y = point.y;
//         point_msg.point.z = point.z;
//         no = point_msg.header.frame_id.c_str();  // クラスタIDをframe_idに格納

//         // PointStampedメッセージを配列に格納
//         point_msg_array.push_back(point_msg);
//         cluster_no.push_back(no);

//         // ログに出力
//         // ROS_INFO("Point: (%.2f, %.2f, %.2f), Cluster ID: %s", point.x, point.y, point.z, point_msg.header.frame_id.c_str());
//     }
// }

// int main(int argc, char** argv) {
//     ros::init(argc, argv, "cluster_receiver");
//     ros::NodeHandle nh;
//     ros::NodeHandle n("~");
//     n.getParam("FRAME/ROBOT_BASE",FRAME_ROBOT_BASE);
// static tf2_ros::TransformListener tfListener(tf_buffer_);
//     // クラスタリング結果を購読
//     ros::Subscriber cluster_sub = nh.subscribe("/clustered_points", 10, clusterCallback);

//     ros::spin();
//     return 0;
// }








/////////////////////////////////////////////////////////////////////////////////
// 世界座標系に直してからやろうとしたもの、ロボット座標系でいいのではと思いいったんコメント
/////////////////////////////////////////////////////////////////////////////////







#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <std_msgs/Float64.h>
#include <vector>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <tf/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/PolygonStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Vector3.h>

tf2_ros::Buffer tf_buffer_;
std::string FRAME_ROBOT_BASE;
geometry_msgs::PointStamped point_msg;
std::vector<geometry_msgs::PointStamped> pre_point;
nav_msgs::Odometry odomdata_;
geometry_msgs::PoseStamped pose_out_;        
std::vector<geometry_msgs::PointStamped> pose_out;
ros::Publisher spreading_pub;

void clusterCallback(const sensor_msgs::PointCloud2::ConstPtr& msg) {
    // PointCloud2をPCL形式に変換
    pcl::PointCloud<pcl::PointXYZI>::Ptr pcl_cloud(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::fromROSMsg(*msg, *pcl_cloud);

    // PointStamped型の配列を格納するベクター
    std::vector<geometry_msgs::PointStamped> point_msg_array;
    std::vector<std::string> cluster_no;

    std::string cloud_frame_id = pcl_cloud->header.frame_id;
    // 受け取った全てのクラスタの座標を表示し、PointStampedに格納
    for (const auto& point : pcl_cloud->points) {
        geometry_msgs::PointStamped point_msg;
        std::string no;
        point_msg.header = pcl_conversions::fromPCL(pcl_cloud->header);
        point_msg.point.x = point.x;
        point_msg.point.y = point.y;
        point_msg.point.z = point.z;
        no = point_msg.header.frame_id.c_str();  // クラスタIDをframe_idに格納

        // PointStampedメッセージを配列に格納
        point_msg_array.push_back(point_msg);
        cluster_no.push_back(no);

        // ログに出力
        // ROS_INFO("Point: (%.2f, %.2f, %.2f), Cluster ID: %s", point.x, point.y, point.z, point_msg.header.frame_id.c_str());
    }

    for (int i = 0; i < point_msg_array.size(); i++) {
        std::vector<geometry_msgs::PointStamped> pre_point;
        pre_point.resize(point_msg_array.size());
        // pre_point.push_back(point_msg_array[i]);
        pre_point[i].header = point_msg_array[i].header;
        pre_point[i].point = point_msg_array[i].point;
        
        std::vector<geometry_msgs::TransformStamped> transformStamped;

        pose_out.resize(point_msg_array.size());
        transformStamped.resize(point_msg_array.size());
        
        try
        {
            transformStamped[i] = tf_buffer_.lookupTransform(FRAME_ROBOT_BASE, pre_point[i].header.frame_id, pre_point[i].header.stamp, ros::Duration(0.1));
        }
        catch (tf2::TransformException &ex) 
        {
            // ROS_INFO("Random value: %f", random_value);
            ROS_WARN_STREAM("get_tf TF2 exception: " << ex.what());
        }
        
    //     ROS_INFO("Point");
        tf2::doTransform(pre_point[i], pose_out[i], transformStamped[i]);
    //     // 配列に格納した結果を確認
        
    }
}

void encoderCallback(const nav_msgs::Odometry::ConstPtr& msg){
    odomdata_ = *msg;
    geometry_msgs::PointStamped odom_point;
    odom_point.header = odomdata_.header;
    odom_point.point = odomdata_.pose.pose.position;

    geometry_msgs::PoseStamped pre_point;
    pre_point.header = odomdata_.header;
    pre_point.pose = odomdata_.pose.pose;
    
    geometry_msgs::TransformStamped transformStamped;
    geometry_msgs::Vector3 MIN_kyori;
    MIN_kyori.z = std::numeric_limits<double>::max();
    
    try
    {
        transformStamped = tf_buffer_.lookupTransform(FRAME_ROBOT_BASE, pre_point.header.frame_id, pre_point.header.stamp, ros::Duration(0.0));
    }
    catch (tf2::TransformException &ex) 
    {
        // ROS_INFO("Random value: %f", random_value);
        ROS_WARN_STREAM("get_tf TF2 exception: " << ex.what());
    }
    
    tf2::doTransform(pre_point, pose_out_, transformStamped);
    ROS_INFO("Point - x: %.2f, y: %.2f, z: %.2f, Cluster ID: %s",
                pose_out_.pose.position.x,
                pose_out_.pose.position.y,
                pose_out_.pose.position.z,
                pose_out_.header.frame_id);
    for (int i = 0; i < pose_out.size(); i++) 
    {
        double kyori = sqrt(pow(pose_out[i].point.x - pose_out_.pose.position.x , 2) + pow(pose_out[i].point.y - pose_out_.pose.position.y , 2));
        if (kyori < MIN_kyori.z)
        {
            MIN_kyori.x = pose_out[i].point.x;
            MIN_kyori.y = pose_out[i].point.y;
            MIN_kyori.z = kyori;
        }
    }
    ROS_INFO("spreadinghannkei-----%.2f",MIN_kyori.z);
    spreading_pub.publish(MIN_kyori);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "cluster_receiver");
    ros::NodeHandle nh;
    ros::NodeHandle n("~");
    n.getParam("FRAME/ROBOT_BASE",FRAME_ROBOT_BASE);
static tf2_ros::TransformListener tfListener(tf_buffer_);
    // クラスタリング結果を購読
    ros::Subscriber cluster_sub = nh.subscribe("/clustered_points", 10, clusterCallback);
    ros::Subscriber encoder_sub = nh.subscribe("/main/odom", 10, encoderCallback);
    spreading_pub = nh.advertise<geometry_msgs::Vector3>("/Group_radius", 10);

    ros::spin();
    return 0;
}