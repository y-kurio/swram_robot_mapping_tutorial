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
#include <swram_robot_mapping_tutorial/cluster_data.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

class LRFClustering {
private:
    ros::NodeHandle nh_;
    ros::Subscriber laser_sub_;
    ros::Publisher cluster_pub_;
    laser_geometry::LaserProjection projector_;

    int wall_threshold_;  // 障害物と壁を分ける閾値

public:
    LRFClustering() : wall_threshold_(60) {  // デフォルト値
        ros::NodeHandle private_nh("~");
        private_nh.param("wall_threshold", wall_threshold_, 100);  // パラメータ化

        laser_sub_ = nh_.subscribe("scan", 10, &LRFClustering::laserCallback, this);
        cluster_pub_ = nh_.advertise<swram_robot_mapping_tutorial::cluster_data>("clustered_points", 10);
    }

    void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan) {
        // LaserScanをPointCloud2に変換
        sensor_msgs::PointCloud2 cloud;
        swram_robot_mapping_tutorial::cluster_data cluster_data;
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


        // クラスタIDを付けて全座標を集約
        // pcl::PointCloud<pcl::PointXYZI>::Ptr clustered_cloud(new pcl::PointCloud<pcl::PointXYZI>);

        for (int cluster_id = 0; cluster_id < cluster_indices.size(); ++cluster_id) {
            size_t num_points = cluster_indices[cluster_id].indices.size();

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

            cluster_data.cluster_number[cluster_id] = cluster_id;

            // クラスタサイズで分類
            float cluster_type = (num_points >= wall_threshold_) ? 2.0 : 1.0;
            std::string label = (cluster_type == 2.0) ? "Wall" : "Obstacle";

            cluster_data.cluster_type[cluster_id] = cluster_type;

            ROS_INFO("Cluster ID: %d, Size: %zu -> Classified as: %s", cluster_id, num_points, label.c_str());

            for (const auto& idx : cluster_indices[cluster_id].indices) {
                // pcl::PointXYZI point;
                // point.x = pcl_cloud->points[idx].x;
                // point.y = pcl_cloud->points[idx].y;
                // point.z = pcl_cloud->points[idx].z;
                geometry_msgs::Point32 p;
                p.x = pcl_cloud->points[idx].x;
                p.y = pcl_cloud->points[idx].y;
                p.z = pcl_cloud->points[idx].z;
                cluster_data.cluster_points[cluster_id].polygon.points.push_back(p);
                cluster_data.cluster_type[cluster_id] = cluster_type;  // 1.0 = 障害物, 2.0 = 壁
                // clustered_cloud->points.push_back(point);
            }
            cluster_data.cluster_number[cluster_id] = cluster_id;
        }

        // PointCloud2メッセージとして送信
        // sensor_msgs::PointCloud2 output;
        // pcl::toROSMsg(*clustered_cloud, output);
        cluster_data.header = cloud.header;
        cluster_pub_.publish(cluster_data);
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "lrf_clustering");
    LRFClustering clustering_node;
    ros::spin();
    return 0;
}

