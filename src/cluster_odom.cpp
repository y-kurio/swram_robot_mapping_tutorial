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
#include <swram_robot_mapping_tutorial/cluster_data.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Point.h>

tf2_ros::Buffer tf_buffer_;
std::string FRAME_ROBOT_BASE;
geometry_msgs::PointStamped point_msg;
std::vector<geometry_msgs::PointStamped> pre_point;
nav_msgs::Odometry odomdata_;
geometry_msgs::PoseStamped pose_out_;        
std::vector<geometry_msgs::PointStamped> pose_out;
ros::Publisher spreading_pub, clusterling_pub;
geometry_msgs::Pose MIN_kyori;
swram_robot_mapping_tutorial::cluster_data cluster_data;
ros::Publisher marker_pub;
double theta, wall_theta;

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
    int count_orientation = 0;
    for (int i = 0; i < cluster_data.cluster_type.size(); ++i) {
        if (cluster_data.cluster_type[i] == 1.0) {
            count_orientation++;
        }
    }
    cluster_data.orientation.resize(count_orientation + 1); // 必ず先にサイズ確保
    odomdata_ = *msg;
    geometry_msgs::PointStamped odom_point;
    odom_point.header = odomdata_.header;
    odom_point.point = odomdata_.pose.pose.position;

    geometry_msgs::PoseStamped pre_point_;
    pre_point_.header = odomdata_.header;
    pre_point_.pose = odomdata_.pose.pose;
    
    geometry_msgs::TransformStamped transformStamped;
    double kyori;
    MIN_kyori.position.x = std::numeric_limits<double>::max();
    MIN_kyori.position.y = std::numeric_limits<double>::max();
    MIN_kyori.position.z = std::numeric_limits<double>::max();
    
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////一番近い距離を探索
    // for (int cluster_id = 0; cluster_id < cluster_data.cluster_points.size(); ++cluster_id) {
    //     for (int i = 0; i < cluster_data.cluster_points[cluster_id].polygon.points.size(); i++) 
    //     {
    //         kyori = sqrt(pow(cluster_data.cluster_points[cluster_id].polygon.points[i].x - pose_out_.pose.position.x , 2) + pow(cluster_data.cluster_points[cluster_id].polygon.points[i].y - pose_out_.pose.position.y , 2));
    //         if (kyori < MIN_kyori.z && cluster_data.cluster_type[cluster_id] == 2.0)
    //         {
    //             MIN_kyori.x = cluster_data.cluster_points[cluster_id].polygon.points[i].x;
    //             MIN_kyori.y = cluster_data.cluster_points[cluster_id].polygon.points[i].y;
    //             MIN_kyori.z = kyori;
    //         }
    //     }
    // }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////x,yそれぞれの方向に障害物があるのかを探索
    for (int cluster_id = 0; cluster_id < cluster_data.cluster_points.size(); ++cluster_id) {
        for (int i = 0; i < cluster_data.cluster_points[cluster_id].polygon.points.size(); i++) 
        {

            kyori = sqrt(pow(cluster_data.cluster_points[cluster_id].polygon.points[i].x - pose_out_.pose.position.x , 2) + pow(cluster_data.cluster_points[cluster_id].polygon.points[i].y - pose_out_.pose.position.y , 2));
            if (kyori < MIN_kyori.position.z && cluster_data.cluster_type[cluster_id] == 2.0)
            {
                MIN_kyori.position.z = kyori;
                double dx = cluster_data.cluster_points[cluster_id].polygon.points[i].x - pose_out_.pose.position.x;
                double dy = cluster_data.cluster_points[cluster_id].polygon.points[i].y - pose_out_.pose.position.y;
                MIN_kyori.orientation.w = atan2(dy, dx);

            }
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if (MIN_kyori.orientation.w > 0)
{
    if (MIN_kyori.orientation.w < (M_PI / 2))
        {
            theta = MIN_kyori.orientation.w - (M_PI / 2);
        }
    else if (MIN_kyori.orientation.w > (M_PI / 2))
        {
            theta = MIN_kyori.orientation.w - (M_PI / 2);
        }
}
else if (MIN_kyori.orientation.w < 0)
{
    if (MIN_kyori.orientation.w > -(M_PI / 2))
        {
            theta = MIN_kyori.orientation.w + (M_PI / 2);
        }
    else if (MIN_kyori.orientation.w < -(M_PI / 2))
        {
            theta = (M_PI / 2) + MIN_kyori.orientation.w ;
        }
}

for (int cluster_id = 0; cluster_id < cluster_data.cluster_points.size(); ++cluster_id) {
        for (int i = 0; i < cluster_data.cluster_points[cluster_id].polygon.points.size(); i++) 
        {
            if (cluster_data.cluster_type[cluster_id] == 2.0)
            {
                double dx = cluster_data.cluster_points[cluster_id].polygon.points[i].x - pose_out_.pose.position.x;
                double dy = cluster_data.cluster_points[cluster_id].polygon.points[i].y - pose_out_.pose.position.y;
                wall_theta = atan2(dy, dx);
                if (MIN_kyori.orientation.w > 0)
                {
                    if (wall_theta < 0.087 + wall_theta && wall_theta > -0.087 + wall_theta && wall_theta <  + 3.14 + wall_theta && wall_theta > -0.087 + wall_theta + 3.14)
                    {
                        double kyori_x = cluster_data.cluster_points[cluster_id].polygon.points[i].x - pose_out_.pose.position.x;
                        if (kyori_x < MIN_kyori.position.x)
                        {
                            MIN_kyori.position.x = kyori_x;
                        }
                    }
                    if (wall_theta < 0.087 + wall_theta + 1.57 && wall_theta > -0.087 + wall_theta + 1.57 && wall_theta <  -1.57 + wall_theta && wall_theta > -0.087 + wall_theta -1.57)
                    {
                        double kyori_y = cluster_data.cluster_points[cluster_id].polygon.points[i].y - pose_out_.pose.position.y;
                        if (kyori_y < MIN_kyori.position.y)
                        {
                            MIN_kyori.position.y = kyori_y;
                        }
                    }
                }
                else if (MIN_kyori.orientation.w < 0)
                {
                    if (wall_theta < 0.087 + wall_theta && wall_theta > -0.087 + wall_theta && wall_theta <  + 3.14 + wall_theta && wall_theta > -0.087 + wall_theta + 3.14)
                    {
                        double kyori_x = cluster_data.cluster_points[cluster_id].polygon.points[i].x - pose_out_.pose.position.x;
                        if (kyori_x < MIN_kyori.position.x)
                        {
                            MIN_kyori.position.x = kyori_x;
                        }
                    }
                    if (wall_theta < 0.087 + wall_theta + 1.57 && wall_theta > -0.087 + wall_theta + 1.57 && wall_theta <  -1.57 + wall_theta && wall_theta > -0.087 + wall_theta -1.57)
                    {
                        double kyori_y = cluster_data.cluster_points[cluster_id].polygon.points[i].y - pose_out_.pose.position.y;
                        if (kyori_y < MIN_kyori.position.y)
                        {
                            MIN_kyori.position.y = kyori_y;
                        }
                    }
                }
                
            }
        }
    }


    // for (int cluster_id = 0; cluster_id < cluster_data.cluster_points.size(); ++cluster_id) {
    //     for (int i = 0; i < cluster_data.cluster_points[cluster_id].polygon.points.size(); i++) 
    //     {
    //         if ((pose_out_.pose.position.y - 0.15) < cluster_data.cluster_points[cluster_id].polygon.points[i].y && (pose_out_.pose.position.y + 0.15) > cluster_data.cluster_points[cluster_id].polygon.points[i].y && cluster_data.cluster_type[cluster_id] == 2.0)
    //         {
    //             double kyori_x = cluster_data.cluster_points[cluster_id].polygon.points[i].x - pose_out_.pose.position.x;
    //             if (kyori_x < MIN_kyori.position.x)
    //             {
    //                 MIN_kyori.position.x = kyori_x;
    //             }
    //         }
            
    //         if ((pose_out_.pose.position.x - 0.15) < cluster_data.cluster_points[cluster_id].polygon.points[i].x && (pose_out_.pose.position.x + 0.15) > cluster_data.cluster_points[cluster_id].polygon.points[i].x && cluster_data.cluster_type[cluster_id] == 2.0)
    //         {
    //             double kyori_y = cluster_data.cluster_points[cluster_id].polygon.points[i].y - pose_out_.pose.position.y;
    //             if (kyori_y < MIN_kyori.position.y)
    //             {
    //                 MIN_kyori.position.y = kyori_y;
    //             }
    //         }
    //     }
    // }

// ループ内で Marker を作成
visualization_msgs::Marker points_marker;
points_marker.header.frame_id = "map";  // RViz の Fixed Frame に合わせる
points_marker.header.stamp = ros::Time::now();
points_marker.ns = "cluster_points";
points_marker.id = 0;
points_marker.type = visualization_msgs::Marker::SPHERE_LIST;  // 複数点をまとめて表示
points_marker.action = visualization_msgs::Marker::ADD;

// 点の大きさ
points_marker.scale.x = 0.05;
points_marker.scale.y = 0.05;
points_marker.scale.z = 0.05;

// 色
points_marker.color.r = 1.0;
points_marker.color.g = 0.0;
points_marker.color.b = 0.0;
points_marker.color.a = 1.0;

// cluster_data の各点を追加
for (int cluster_id = 0; cluster_id < cluster_data.cluster_points.size(); ++cluster_id) {
    for (int i = 0; i < cluster_data.cluster_points[cluster_id].polygon.points.size(); ++i) {
        geometry_msgs::Point p;
        p.y = cluster_data.cluster_points[cluster_id].polygon.points[i].y;
        p.x = cluster_data.cluster_points[cluster_id].polygon.points[i].x;
        p.z = 0.0;  // 2D地図なので z=0
        points_marker.points.push_back(p);
    }
}

// publish
marker_pub.publish(points_marker);

    if (MIN_kyori.position.x < 100)
    {
    }else 
    {
        MIN_kyori.position.x = 0.0;
    }
    if (MIN_kyori.position.y < 100)
    {
    }else 
    {
        MIN_kyori.position.y = 0.0;
    }
    if (MIN_kyori.position.z < 100)
    {
        // ROS_INFO("spreadinghannkei-----%.2f",MIN_kyori.z);
    spreading_pub.publish(MIN_kyori);
    // clusterling_pub.publish(cluster_data);
    }else 
    {
        MIN_kyori.position.z = 0.0;
        spreading_pub.publish(MIN_kyori);
    }
    std::cout << "x: " << MIN_kyori.position.x << std::endl;
    std::cout << "y: " << MIN_kyori.position.y << std::endl;
    
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "cluster_receiver");
    ros::NodeHandle nh;
    ros::NodeHandle n("~");
    n.getParam("FRAME/ROBOT_BASE",FRAME_ROBOT_BASE);
static tf2_ros::TransformListener tfListener(tf_buffer_);
    // クラスタリング結果を購読
    ros::Subscriber cluster_sub = nh.subscribe("clustered_points", 10, clusterCallback);
    ros::Subscriber encoder_sub = nh.subscribe("odom", 10, encoderCallback);
    spreading_pub = nh.advertise<geometry_msgs::Pose>("/Group_radius", 10);
    marker_pub = nh.advertise<visualization_msgs::Marker>("cluster_points_marker", 1);
    // clusterling_pub = nh.advertise<swram_robot_mapping_tutorial::cluster_data>("clusterdata", 10);

    ros::spin();
    return 0;
}

// #include <iostream>
// #include <vector>
// #include <random>

// // 乱数生成エンジンを関数の外に作成
// std::random_device rd;
// std::mt19937 gen(rd());
// std::uniform_real_distribution<double> dist(0.0, 360.0);

// // 指定された範囲に角度が含まれるかをチェック
// bool isExcluded(double angle, const std::vector<std::pair<double, double>>& excluded_ranges) {
//     for (const auto& range : excluded_ranges) {
//         if (angle >= range.first && angle <= range.second) {
//             return true;
//         }
//     }
//     return false;
// }

// // 除外範囲を考慮してランダムな角度を生成
// double generateRandomAngle(const std::vector<std::pair<double, double>>& excluded_ranges) {
//     double angle;
//     do {
//         angle = dist(gen);
//     } while (isExcluded(angle, excluded_ranges));
    
//     return angle;
// }

// int main() {
//     std::vector<std::pair<double, double>> excluded_ranges = {{0, 60}, {180, 220}, {280, 320}};
    
//     for (int i = 0; i < 5; ++i) { // 5回試してみる
//         double random_angle = generateRandomAngle(excluded_ranges);
//         std::cout << "Generated angle: " << random_angle << " degrees" << std::endl;
//     }
    
//     return 0;
// }