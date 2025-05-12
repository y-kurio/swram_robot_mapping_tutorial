// potential_field_node.cpp




#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <visualization_msgs/MarkerArray.h>
#include <swram_robot_mapping_tutorial/cluster_data.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <cmath>

class PotentialFieldPlanner {
private:
    ros::NodeHandle nh_, pnh_;
    ros::Subscriber odom_sub_, goal_sub_, cluster_sub_;
    ros::Publisher cmd_pub_, viz_pub_;

    geometry_msgs::Pose current_pose_;
    geometry_msgs::Pose goal_pose_;
    swram_robot_mapping_tutorial::cluster_data current_clusters_;

    double attractive_gain_, repulsive_gain_, repulsive_range_;
    double grid_size_, grid_resolution_;

public:
    PotentialFieldPlanner() : pnh_("~") {
        // パラメータの読み込み（YAMLやlaunchファイルから設定可能）
        pnh_.param("attractive_gain", attractive_gain_, 1.0);
        pnh_.param("repulsive_gain", repulsive_gain_, 3.0);
        pnh_.param("repulsive_range", repulsive_range_, 4.0);
        pnh_.param("grid_size", grid_size_, 10.0);
        pnh_.param("grid_resolution", grid_resolution_, 0.2);

        // トピック購読
        odom_sub_ = nh_.subscribe("/robot_1/odom", 1, &PotentialFieldPlanner::odomCallback, this);
        goal_sub_ = nh_.subscribe("/robot_1/sub_goal_position", 1, &PotentialFieldPlanner::goalCallback, this);
        cluster_sub_ = nh_.subscribe("/robot_1/clusterdata", 1, &PotentialFieldPlanner::clusterCallback, this);

        // 出力トピック
        cmd_pub_ = nh_.advertise<geometry_msgs::Twist>("/robot_1/cmd_vel", 1);
        viz_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("/potential_field_markers", 1);
    }

    // 現在位置更新時に呼ばれる
    void odomCallback(const nav_msgs::Odometry::ConstPtr& msg) {
        current_pose_ = msg->pose.pose;
    }

    void goalCallback(const geometry_msgs::PoseStamped::ConstPtr& msg) {
        goal_pose_ = msg->pose;
        computePotentialField();  // ここで毎回ポテンシャル場を計算
    }

    void clusterCallback(const swram_robot_mapping_tutorial::cluster_data::ConstPtr& msg) {
        current_clusters_ = *msg;
    }

    void computePotentialField() {
        const int cells = grid_size_ / grid_resolution_;
        const double origin_x = current_pose_.position.x - grid_size_ / 2.0;
        const double origin_y = current_pose_.position.y - grid_size_ / 2.0;

        visualization_msgs::MarkerArray marker_array;

        // 可視化用のポテンシャル計算（Rviz向け）
        for (int i = 0; i < cells; ++i) {
            for (int j = 0; j < cells; ++j) {
                double x = origin_x + i * grid_resolution_;
                double y = origin_y + j * grid_resolution_;

                // 吸引ポテンシャル
                double dx = goal_pose_.position.x - x;
                double dy = goal_pose_.position.y - y;
                double attractive = 0.5 * attractive_gain_ * (dx * dx + dy * dy);

                // 反発ポテンシャル
                double repulsive = 0.0;
                for (const auto& poly : current_clusters_.cluster_points) {
                    for (const auto& pt : poly.polygon.points) {
                        double obs_dx = pt.x - x;
                        double obs_dy = pt.y - y;
                        double dist = std::sqrt(obs_dx * obs_dx + obs_dy * obs_dy);
                        if (dist < repulsive_range_ && dist > 1e-3) {
                            repulsive += 0.5 * repulsive_gain_ *
                                std::pow((1.0 / dist - 1.0 / repulsive_range_), 2);
                        }
                    }
                }

                double potential = attractive + repulsive;

                // Rviz用マーカ生成
                visualization_msgs::Marker marker;
                marker.header.frame_id = "map";
                marker.header.stamp = ros::Time::now();
                marker.ns = "potential_field";
                marker.id = i * cells + j;
                marker.type = visualization_msgs::Marker::CUBE;
                marker.action = visualization_msgs::Marker::ADD;
                marker.scale.x = grid_resolution_;
                marker.scale.y = grid_resolution_;
                marker.scale.z = 0.01;
                marker.color.a = 0.8;
                marker.color.r = std::min(1.0, potential / 10.0);  // ポテンシャルに応じて色を調整
                marker.color.g = marker.color.b = 0.0;
                marker.pose.position.x = x;
                marker.pose.position.y = y;
                marker.pose.position.z = 0.0;

                marker_array.markers.push_back(marker);
            }
        }

        viz_pub_.publish(marker_array);  // Rvizに送信

        // --- ロボットの動作計算（勾配 → 差動二輪向け速度） ---

        // 現在のYaw角を取得（クォータニオン → RPY変換）
        tf2::Quaternion q(
            current_pose_.orientation.x,
            current_pose_.orientation.y,
            current_pose_.orientation.z,
            current_pose_.orientation.w
        );
        double roll, pitch, yaw;
        tf2::Matrix3x3(q).getRPY(roll, pitch, yaw);

        // 吸引ベクトル
        double fx = attractive_gain_ * (goal_pose_.position.x - current_pose_.position.x);
        double fy = attractive_gain_ * (goal_pose_.position.y - current_pose_.position.y);

        // 反発ベクトルを加算
        for (const auto& poly : current_clusters_.cluster_points) {
            for (const auto& pt : poly.polygon.points) {
                double dx = current_pose_.position.x - pt.x;
                double dy = current_pose_.position.y - pt.y;
                double dist = std::sqrt(dx * dx + dy * dy);
                if (dist < repulsive_range_ && dist > 1e-3) {
                    double rep = repulsive_gain_ * (1.0 / dist - 1.0 / repulsive_range_) / (dist * dist);
                    fx += rep * (dx / dist);
                    fy += rep * (dy / dist);
                }
            }
        }

        // 勾配ベクトルの方向と現在向きとの差から回転速度を決定
        double target_yaw = std::atan2(fy, fx);
        double yaw_error = target_yaw - yaw;

        // -pi ~ +pi に正規化
        while (yaw_error > M_PI) yaw_error -= 2 * M_PI;
        while (yaw_error < -M_PI) yaw_error += 2 * M_PI;

        // 勾配ベクトルの大きさ → 前進速度に変換
        double grad_magnitude = std::sqrt(fx * fx + fy * fy);

        geometry_msgs::Twist cmd;
        cmd.linear.x = std::min(0.2, grad_magnitude * 0.2);  // 前進速度（最大0.5）
        cmd.linear.y = 0.0;  // 差動二輪は横移動不可
        cmd.angular.z = std::max(-1.0, std::min(1.0, yaw_error));  // 回転速度（±1制限）

        cmd_pub_.publish(cmd);  // 速度コマンドを送信
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "potential_field_node");
    PotentialFieldPlanner planner;
    ros::spin();
    return 0;
}
