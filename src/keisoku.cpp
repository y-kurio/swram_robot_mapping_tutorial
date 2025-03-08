#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/PointStamped.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/search/kdtree.h>
#include <laser_geometry/laser_geometry.h>
#include <vector>
#include <map>

class LRFClustering {
private:
    ros::NodeHandle nh_;
    ros::Subscriber laser_sub_;
    ros::Publisher cluster_pub_;
    laser_geometry::LaserProjection projector_;

public:
    LRFClustering() {
        laser_sub_ = nh_.subscribe("/main/scan", 10, &LRFClustering::laserCallback, this);
        cluster_pub_ = nh_.advertise<sensor_msgs::PointCloud2>("/clustered_points", 10);
    }

    void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan) {
        // LaserScanをPointCloud2に変換
        sensor_msgs::PointCloud2 cloud;
        projector_.projectLaser(*scan, cloud);

        // PointCloud2をPCL形式に変換
        pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_cloud(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::fromROSMsg(cloud, *pcl_cloud);

        // クラスタリング処理
        std::vector<pcl::PointIndices> cluster_indices;
        pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
        tree->setInputCloud(pcl_cloud);

        pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
        ec.setClusterTolerance(0.05); // クラスタ間距離の閾値 (m)
        ec.setMinClusterSize(10);     // 最小クラスタサイズ
        ec.setMaxClusterSize(1000);   // 最大クラスタサイズ
        ec.setSearchMethod(tree);
        ec.setInputCloud(pcl_cloud);
        ec.extract(cluster_indices);

        ROS_INFO("Number of clusters found: %zu", cluster_indices.size());

        // クラスタIDを付けて全座標を集約
        pcl::PointCloud<pcl::PointXYZI>::Ptr clustered_cloud(new pcl::PointCloud<pcl::PointXYZI>);

        for (int cluster_id = 0; cluster_id < cluster_indices.size(); ++cluster_id) {
            double sum_x = 0.0, sum_y = 0.0, sum_z = 0.0;
            size_t num_points = cluster_indices[cluster_id].indices.size();

            for (const auto& idx : cluster_indices[cluster_id].indices) {
                pcl::PointXYZI point;
                point.x = pcl_cloud->points[idx].x;
                point.y = pcl_cloud->points[idx].y;
                point.z = pcl_cloud->points[idx].z;
                point.intensity = static_cast<float>(cluster_id);  // クラスタIDをintensityに格納
                clustered_cloud->points.push_back(point);

                // 重心計算用に座標を加算
                sum_x += point.x;
                sum_y += point.y;
                sum_z += point.z;
            }

            // 重心を計算
            pcl::PointXYZI centroid_point;
            centroid_point.x = sum_x / num_points;
            centroid_point.y = sum_y / num_points;
            centroid_point.z = sum_z / num_points;
            centroid_point.intensity = static_cast<float>(cluster_id) + 100; // 重心は異なるintensityでマーク

            clustered_cloud->points.push_back(centroid_point);

            // ログ出力
            ROS_INFO("Cluster ID: %d, Centroid - x: %.2f, y: %.2f, z: %.2f",
                     cluster_id, centroid_point.x, centroid_point.y, centroid_point.z);
        }

        // PointCloud2メッセージとして送信
        sensor_msgs::PointCloud2 output;
        pcl::toROSMsg(*clustered_cloud, output);
        output.header = cloud.header;
        cluster_pub_.publish(output);
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "lrf_clustering");
    LRFClustering clustering_node;
    ros::spin();
    return 0;
}
