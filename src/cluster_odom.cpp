#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/PointStamped.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

void clusterCallback(const sensor_msgs::PointCloud2::ConstPtr& msg) {
    // PointCloud2をPCL形式に変換
    pcl::PointCloud<pcl::PointXYZI>::Ptr pcl_cloud(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::fromROSMsg(*msg, *pcl_cloud);

    // PointStamped型の配列を格納するベクター
    std::vector<geometry_msgs::PointStamped> point_msg_array;

    // 受け取った全てのクラスタの座標を表示し、PointStampedに格納
    for (const auto& point : pcl_cloud->points) {
        geometry_msgs::PointStamped point_msg;
        point_msg.header.stamp = ros::Time::now();        // 時刻を設定
        point_msg.header.frame_id = std::to_string(static_cast<int>(point.intensity));  // クラスタIDをframe_idに格納
        point_msg.point.x = point.x;
        point_msg.point.y = point.y;
        point_msg.point.z = point.z;

        // PointStampedメッセージを配列に格納
        point_msg_array.push_back(point_msg);

        // ログに出力
        // ROS_INFO("Point: (%.2f, %.2f, %.2f), Cluster ID: %s", point.x, point.y, point.z, point_msg.header.frame_id.c_str());
    }

    // 配列に格納した結果を確認
    for (size_t i = 0; i < point_msg_array.size(); ++i) {
        ROS_INFO("Point %zu - x: %.2f, y: %.2f, z: %.2f, Cluster ID: %s",
                 i,
                 point_msg_array[i].point.x,
                 point_msg_array[i].point.y,
                 point_msg_array[i].point.z,
                 point_msg_array[i].header.frame_id.c_str());
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "cluster_receiver");
    ros::NodeHandle nh;

    // クラスタリング結果を購読
    ros::Subscriber cluster_sub = nh.subscribe("/clustered_points", 10, clusterCallback);

    ros::spin();
    return 0;
}