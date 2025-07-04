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
#include <swram_robot_mapping_tutorial/cluster_data.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

tf2_ros::Buffer tf_buffer_;
std::string FRAME_ROBOT_BASE;
double ROBOT_SIZE;
geometry_msgs::PointStamped point_msg;
std::vector<geometry_msgs::PointStamped> pre_point;
nav_msgs::Odometry odomdata_;
geometry_msgs::PoseStamped pose_out_;        
std::vector<geometry_msgs::PointStamped> pose_out;
ros::Publisher spreading_pub, clusterling_pub;
geometry_msgs::Vector3 MIN_kyori;
swram_robot_mapping_tutorial::cluster_data cluster_data;

void clusterCallback(const swram_robot_mapping_tutorial::cluster_data::ConstPtr& msg) {
    // // PointCloud2をPCL形式に変換
    // pcl::PointCloud<pcl::PointXYZI>::Ptr pcl_cloud(new pcl::PointCloud<pcl::PointXYZI>);
    // pcl::fromROSMsg(*msg, *pcl_cloud);

    // // PointStamped型の配列を格納するベクター
    // std::vector<geometry_msgs::PointStamped> point_msg_array;
    // std::vector<std::string> cluster_no;

    // std::string cloud_frame_id = pcl_cloud->header.frame_id;
    // // 受け取った全てのクラスタの座標を表示し、PointStampedに格納
    // for (const auto& point : pcl_cloud->points) {
    //     geometry_msgs::PointStamped point_msg;
    //     std::string no;
    //     point_msg.header = pcl_conversions::fromPCL(pcl_cloud->header);
    //     point_msg.point.x = point.x;
    //     point_msg.point.y = point.y;
    //     point_msg.point.z = point.z;
    //     no = point_msg.header.frame_id.c_str();  // クラスタIDをframe_idに格納

    //     // PointStampedメッセージを配列に格納
    //     point_msg_array.push_back(point_msg);
    //     cluster_no.push_back(no);

    //     // ログに出力
    //     // ROS_INFO("Point: (%.2f, %.2f, %.2f), Cluster ID: %s", point_msg.point.x, point_msg.point.y, point_msg.point.z, point_msg.header.frame_id.c_str());
    // }

    cluster_data = *msg;

    for (int cluster_id = 0; cluster_id < cluster_data.cluster_points.size(); ++cluster_id) {
        for (int i = 0; i < cluster_data.cluster_points[cluster_id].polygon.points.size(); i++) {
             // 各 vector のサイズ確保
             if (cluster_data.cluster_number.size() <= cluster_id) {
                cluster_data.cluster_number.resize(cluster_id + 1);
            }
            if (cluster_data.cluster_type.size() <= cluster_id) {
                cluster_data.cluster_type.resize(cluster_id + 1);
            }
            if (cluster_data.cluster_points.size() <= cluster_id) {
                cluster_data.cluster_points.resize(cluster_id + 1);
            }
            pre_point.resize(cluster_data.cluster_points[cluster_id].polygon.points.size());
            // pre_point.push_back(point_msg_array[i]);
            pre_point[i].header = cluster_data.header;
            pre_point[i].point.x = cluster_data.cluster_points[cluster_id].polygon.points[i].x;
            pre_point[i].point.y = cluster_data.cluster_points[cluster_id].polygon.points[i].y;
            pre_point[i].point.z = cluster_data.cluster_points[cluster_id].polygon.points[i].z;
            
            std::vector<geometry_msgs::TransformStamped> transformStamped;
    
            pose_out.resize(cluster_data.cluster_points[cluster_id].polygon.points.size());
            transformStamped.resize(cluster_data.cluster_points[cluster_id].polygon.points.size());
            
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
            cluster_data.cluster_points[cluster_id].polygon.points[i].x = pose_out[i].point.x;
            cluster_data.cluster_points[cluster_id].polygon.points[i].y = pose_out[i].point.y;
            cluster_data.cluster_points[cluster_id].polygon.points[i].z = pose_out[i].point.z;
        }
    }

    
}

void encoderCallback(const nav_msgs::Odometry::ConstPtr& msg){
    // int count_orientation = 0;
    // for (int i = 0; i < cluster_data.cluster_type.size(); ++i) {
    //     if (cluster_data.cluster_type[i] == 1.0) {
    //         count_orientation++;
    //     }
    // }
    cluster_data.orientation.resize(cluster_data.cluster_type.size()); // 必ず先にサイズ確保
    odomdata_ = *msg;
    geometry_msgs::PointStamped odom_point;
    odom_point.header = odomdata_.header;
    odom_point.point = odomdata_.pose.pose.position;

    geometry_msgs::PoseStamped pre_point_;
    pre_point_.header = odomdata_.header;
    pre_point_.pose = odomdata_.pose.pose;
    
    geometry_msgs::TransformStamped transformStamped;
    double kyori;
    MIN_kyori.z = std::numeric_limits<double>::max();
    
    try
    {
        transformStamped = tf_buffer_.lookupTransform(FRAME_ROBOT_BASE, pre_point_.header.frame_id, pre_point_.header.stamp, ros::Duration(0.0));
    }
    catch (tf2::TransformException &ex) 
    {
        // ROS_INFO("Random value: %f", random_value);
        ROS_WARN_STREAM("get_tf TF2 exception: " << ex.what());
    }
    
    tf2::doTransform(pre_point_, pose_out_, transformStamped);


    for (int cluster_id = 0; cluster_id < cluster_data.cluster_points.size(); ++cluster_id) 
    {
        int size = cluster_data.cluster_points[cluster_id].polygon.points.size();
        double G_kyori = sqrt(pow(cluster_data.cluster_points[cluster_id].polygon.points[size/2].x - pose_out_.pose.position.x , 2) + pow(cluster_data.cluster_points[cluster_id].polygon.points[size/2].y - pose_out_.pose.position.y , 2));
        double annzennhosyou = atan2(ROBOT_SIZE, G_kyori);
        if (cluster_data.cluster_type[cluster_id] == 1.0)
        {
            cluster_data.orientation[cluster_id].x = atan2(cluster_data.cluster_points[cluster_id].polygon.points[0].y, cluster_data.cluster_points[cluster_id].polygon.points[0].x);
            cluster_data.orientation[cluster_id].y = atan2(cluster_data.cluster_points[cluster_id].polygon.points[size-1].y, cluster_data.cluster_points[cluster_id].polygon.points[size-1].x);
            if (cluster_data.orientation[cluster_id].x < cluster_data.orientation[cluster_id].y)
            {
                cluster_data.orientation[cluster_id].x = atan2(cluster_data.cluster_points[cluster_id].polygon.points[size-1].y, cluster_data.cluster_points[cluster_id].polygon.points[size-1].x);
                cluster_data.orientation[cluster_id].y = atan2(cluster_data.cluster_points[cluster_id].polygon.points[0].y, cluster_data.cluster_points[cluster_id].polygon.points[0].x);
            }
            
        ROS_INFO("kakudo: %f, %f", cluster_data.orientation[cluster_id].x, cluster_data.orientation[cluster_id].y);
        }
    }

    // ROS_INFO("spreadinghannkei-----%.2f",MIN_kyori.z);
    // spreading_pub.publish(MIN_kyori);
    clusterling_pub.publish(cluster_data);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "cluster_receiver");
    ros::NodeHandle nh;
    ros::NodeHandle n("~");
    n.getParam("FRAME/ROBOT_BASE",FRAME_ROBOT_BASE);
    n.getParam("ROBOT_SIZE",ROBOT_SIZE);
    static tf2_ros::TransformListener tfListener(tf_buffer_);
    // クラスタリング結果を購読
    ros::Subscriber cluster_sub = nh.subscribe("clustered_points", 10, clusterCallback);
    ros::Subscriber encoder_sub = nh.subscribe("odom", 10, encoderCallback);
    // spreading_pub = nh.advertise<geometry_msgs::Vector3>("/Group_radius", 10);
    clusterling_pub = nh.advertise<swram_robot_mapping_tutorial::cluster_data>("clusterdata", 10);

    ros::spin();
    return 0;
}