#include <comandliner.h>



void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg) //ゴール位置の受け取り
{
    target_goal = *msg;
}

void clusterCallback(const swram_robot_mapping_tutorial::cluster_data::ConstPtr& msg) {
    current_clusters_ = *msg;
}

double ggetRandomAngle() {
    // 乱数エンジンの初期化
    std::random_device rd;
    std::mt19937 gen(rd());

    // -180 から 180 までの範囲で一様分布の乱数を生成
    std::uniform_real_distribution<> dis(-180.0, 180.0);

    // do {
    angle_degrees = dis(gen); // ランダムな角度を生成
    for (int cluster_id = 0; cluster_id < current_clusters_.cluster_type.size(); ++cluster_id) 
    {
        double X = current_clusters_.orientation[cluster_id].x * 180 / M_PI;
        double Y = current_clusters_.orientation[cluster_id].y * 180 / M_PI;
        if (current_clusters_.cluster_type[cluster_id] == 1.0)
        {
            if (X  > Y)
            {
                if (X > angle_degrees && Y < angle_degrees)
                {
                    return ggetRandomAngle();
                }
            }
            if (X  < Y)
            {
                if (X < angle_degrees && Y > angle_degrees)
                {
                    return ggetRandomAngle();
                }
            }
        }
    }
        
    // } while (isExcluded(angle_degrees, excluded_ranges)); // 除外範囲に含まれる場合は再抽選
    // 生成された度数法の角度をラジアンに変換して返す
    return random_angle = angle_degrees * M_PI / 180.0;
}

void computePotentialField() 
{
        const int cells = grid_size_ / grid_resolution_;
        const double origin_x = pose_out.pose.position.x - grid_size_ / 2.0;
        const double origin_y = pose_out.pose.position.y - grid_size_ / 2.0;

        visualization_msgs::MarkerArray marker_array;

        // 可視化用のポテンシャル計算（Rviz向け）
        for (int i = 0; i < cells; ++i) {
            for (int j = 0; j < cells; ++j) {
                double x = origin_x + i * grid_resolution_;
                double y = origin_y + j * grid_resolution_;

                // 吸引ポテンシャル
                double dx = target_goal.pose.position.x - x;
                double dy = target_goal.pose.position.y - y;
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
            pose_out.pose.orientation.x,
            pose_out.pose.orientation.y,
            pose_out.pose.orientation.z,
            pose_out.pose.orientation.w
        );
        double roll, pitch, yaw;
        tf2::Matrix3x3(q).getRPY(roll, pitch, yaw);

        // 吸引ベクトル
        double fx = attractive_gain_ * (d_goal.pose.position.x);
        double fy = attractive_gain_ * (d_goal.pose.position.y);

        // 反発ベクトルを加算
        for (const auto& poly : current_clusters_.cluster_points) {
            for (const auto& pt : poly.polygon.points) {
                double dx = pose_out.pose.position.x - pt.x;
                double dy = pose_out.pose.position.y - pt.y;
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
        if (grad_magnitude < 0.1)
        {
            cmd_vel.linear.x = std::min(0.2, grad_magnitude * 0.2);  // 前進速度（最大0.5）
            cmd_vel.linear.y = 0.0;  // 差動二輪は横移動不可
            cmd_vel.angular.z = std::max(-1.0, std::min(1.0, yaw_error));  // 回転速度（±1制限）
        } else
        {
            cmd_vel.linear.x = 0.1;
            cmd_vel.linear.y = 0.0;
            cmd_vel.linear.z = 0.0;
            cmd_vel.angular.x = 0.0;
            cmd_vel.angular.y = 0.0;
            ggetRandomAngle();
            cmd_vel.angular.z = random_angle;
        }
        
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
    double angle_to_goal = atan2(d_goal.pose.position.y, d_goal.pose.position.x);

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
            computePotentialField();  // ここで毎回ポテンシャル場を計算
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
    n.getParam("attractive_gain", attractive_gain_);
    n.getParam("repulsive_gain", repulsive_gain_);
    n.getParam("repulsive_range", repulsive_range_);
    n.getParam("grid_size", grid_size_);
    n.getParam("grid_resolution", grid_resolution_);
    static tf2_ros::TransformListener tfListener(tf_buffer_);
    ros::Subscriber goal_sub = nh.subscribe("sub_goal_position", 10, poseCallback);
    ros::Subscriber encoder_sub = nh.subscribe("odom", 10, encoderCallback);
    // トピック購読
    ros::Subscriber cluster_sub_ = nh.subscribe("clusterdata", 1, clusterCallback);

    // 出力トピック
    viz_pub_ = nh.advertise<visualization_msgs::MarkerArray>("/potential_field_markers", 1);
    cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    goal_status_pub = nh.advertise<std_msgs::Int8>("goal_status", 10);

    ros::spin();
    return 0;
}
