#include <create_goal.h>

bool is_initialized = false;

void goalactionCallback(const std_msgs::Int8::ConstPtr& msg)
{
    goal_status = *msg;
}

void Group_radiusCallback(const geometry_msgs::Vector3::ConstPtr& msg)
{
    group_radius = *msg;
    MIN_kyori_x = std::abs(group_radius.x);
    MIN_kyori_y = std::abs(group_radius.y);
    MIN_kyori_z = group_radius.z;


    if (group_radius.x > 4.0) 
    {
        ROS_INFO("kara_x");
        MIN_kyori_x = 4.0;
    }
    if (group_radius.y > 4.0) 
    {
        ROS_INFO("kara_y");
        MIN_kyori_y = 4.0;
    }
    // MIN_kyori_x = 4.0;//std::abs(group_radius.x);
    // MIN_kyori_y = 9.0;//std::abs(group_radius.y);
    std::cout << "x: " << MIN_kyori_x << std::endl;
    std::cout << "y: " << MIN_kyori_y << std::endl;
}

void clusterlingCallback(const swram_robot_mapping_tutorial::cluster_data::ConstPtr& msg)
{
    cluster_result = *msg;
}

double ggetRandomAngle() {
    // 乱数エンジンの初期化
    std::random_device rd;
    std::mt19937 gen(rd());

    // -180 から 180 までの範囲で一様分布の乱数を生成
    std::uniform_real_distribution<> dis(-180.0, 180.0);

    // do {
    angle_degrees = dis(gen); // ランダムな角度を生成
    for (int cluster_id = 0; cluster_id < cluster_result.cluster_type.size(); ++cluster_id) 
    {
        double X = cluster_result.orientation[cluster_id].x * 180 / M_PI;
        double Y = cluster_result.orientation[cluster_id].y * 180 / M_PI;
        if (cluster_result.cluster_type[cluster_id] == 1.0)
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


void sub_encoderCallback(const nav_msgs::Odometry::ConstPtr& msg){//フォロワロボットの自己位置取得
    sub_odomdata_ = *msg;
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーフォロワロボットのtf変換
    sub_odom_point.header = sub_odomdata_.header;
    sub_odom_point.point = sub_odomdata_.pose.pose.position;

    geometry_msgs::PoseStamped sub_pre_point;
    sub_pre_point.header = sub_odomdata_.header;
    sub_pre_point.pose = sub_odomdata_.pose.pose;
    
    try
    {
        sub_transformStamped = tf_buffer_.lookupTransform(FRAME_ROBOT_BASE, sub_pre_point.header.frame_id, sub_pre_point.header.stamp, ros::Duration(0.0));
    }
    catch (tf2::TransformException &ex) 
    {
        ROS_INFO("genninn");
        ROS_WARN_STREAM("get_tf TF2 exception: " << ex.what());
    }
    
    tf2::doTransform(sub_pre_point, sub_pose_out, sub_transformStamped);
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
}

void goalpublisher()//目標ゴール位置を送信
{
    newgoal_pub.publish(sub_goal);
    ROS_INFO("publish OK!!!" );
    std::cout << "x: " << sub_goal.pose.position.x << std::endl;
    std::cout << "y: " << sub_goal.pose.position.y << std::endl;
    std::cout << "saidaihanni: " << MIN_kyori << std::endl;
}

void encoderCallback(const nav_msgs::Odometry::ConstPtr& msg)//メインロボットの自己位置取得
{
    odomdata_ = *msg;
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーメインロボットのtf変換
    odom_point.header = odomdata_.header;
    odom_point.point = odomdata_.pose.pose.position;

    geometry_msgs::PoseStamped pre_point;
    pre_point.header = odomdata_.header;
    pre_point.pose = odomdata_.pose.pose;
    
    geometry_msgs::PoseStamped pose_out;
    geometry_msgs::TransformStamped transformStamped;
    
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
    //ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

    // 0からRAND_MAXまでの整数を生成し、0.0〜1.0の範囲にスケール
    double random_value = static_cast<double>(rand()) / RAND_MAX;
    // ROS_INFO("Random value: %f", random_value);
    double bunnsann = MIN_kyori_z / 6;
    geometry_msgs::Quaternion q = pose_out.pose.orientation;  // 例: Odometryなどから取得

    // クオータニオン → tf2のQuaternion型へ変換
    tf2::Quaternion quat(q.x, q.y, q.z, q.w);

    // クオータニオン → オイラー角 (roll, pitch, yaw)
    double pose_out_roll, pose_out_pitch, pose_out_yaw;
    tf2::Matrix3x3(quat).getRPY(pose_out_roll, pose_out_pitch, pose_out_yaw);
    
        // ROS_INFO("FRAME_ROBOT_BASE=%s",FRAME_ROBOT_BASE);
        // std::cout << "x: " << pose_out.pose.position.x << std::endl;
        // std::cout << "y: " << pose_out.pose.position.y << std::endl;
    // std::cout << "goalaction: " << action_data_.status_list[0].status << std::endl;
    //リーダーフォロワ間の相対距離を計算
    double kyori =(sqrt((sub_pose_out.pose.position.x - pose_out.pose.position.x)*(sub_pose_out.pose.position.x - pose_out.pose.position.x) + (sub_pose_out.pose.position.y - pose_out.pose.position.y)*(sub_pose_out.pose.position.y - pose_out.pose.position.y)));
    if (!is_initialized) 
    {
        idoutyou_ = sqrt(-2.0 * bunnsann * bunnsann * log(-(random_value - 1.0)));
        ggetRandomAngle();
        x_ = idoutyou_*cos(random_angle);
        y_ = idoutyou_*sin(random_angle);
        move_pose_x_ = pose_out.pose.position.x + x_;
        move_pose_y_ = pose_out.pose.position.y + y_;
        move_pose_ = sqrt(x_*x_ + y_*y_);
        pre_dis = sqrt(pow((sub_pose_out.pose.position.x - pose_out.pose.position.x), 2) + pow((sub_pose_out.pose.position.y - pose_out.pose.position.y), 2));
        newposdis = sqrt(pow((move_pose_x_ - pose_out.pose.position.x), 2) + pow((move_pose_y_ - pose_out.pose.position.y), 2));

        Vxx = pow(std::max(MIN_kyori_x, 0.2) / 3 , 2 );
        Vyy = pow(std::max(MIN_kyori_y, 0.2) / 3 , 2 );

        Vxy =0;

        // 2x2行列の定義
        A << Vxx, Vxy,
            Vxy, Vyy;

        double det_sigma = A.determinant();
        // ベクトルの定義
        pre_x << sub_pose_out.pose.position.x - pose_out.pose.position.x,
                 sub_pose_out.pose.position.y - pose_out.pose.position.y;

        // ベクトルの定義
        new_x << move_pose_x_ - pose_out.pose.position.x,
                 move_pose_y_ - pose_out.pose.position.y;

        double exponent = -0.5 * (pre_x.transpose() * A.inverse() * pre_x)(0,0);
        x_1_ = (1.0 / (2.0 * M_PI * sqrt(det_sigma))) * std::exp(exponent);

        double exponent2 = -0.5 * (new_x.transpose() * A.inverse() * new_x)(0,0);
        x_2_ = (1.0 / (2.0 * M_PI * sqrt(det_sigma))) * std::exp(exponent2);

        SSS_ = x_2_ / x_1_;
        double roll = 0.0;    // X軸周りの回転
        double pitch = 0.0;   // Y軸周りの回転
        double yaw = random_angle;    // Z軸周りの回転（90度＝1.57ラジアン）
        tf2::Quaternion q;
        // オイラー角からクォータニオンに変換
        geometry_msgs::Quaternion quaternion;
        q.setRPY(roll, pitch, yaw);
        quaternion.x = q.x();
        quaternion.y = q.y();
        quaternion.z = q.z();
        quaternion.w = q.w();

                
        //  SSSの条件分岐
        if( SSS_ > random_value )//ランダムな値と比較したものを比較
        {
            // ロボットの位置を更新
            sub_goal.header.frame_id = FRAME_ROBOT_BASE;
            sub_goal.header.stamp = ros::Time::now();
            sub_goal.pose.position.x = move_pose_x_;
            sub_goal.pose.position.y = move_pose_y_;
            sub_goal.pose.position.z = 0.0;
            sub_goal.pose.orientation.x = quaternion.x;
            sub_goal.pose.orientation.y = quaternion.y;
            sub_goal.pose.orientation.z = quaternion.z;
            sub_goal.pose.orientation.w = quaternion.w;
            // number = 1;
            ROS_INFO("syokai"); 
            goalpublisher();
            is_initialized = true;
        }
    }
    else if (goal_status.data == 0)
    {
        idoutyou_ = sqrt(-2.0 * bunnsann * bunnsann * log(-(random_value - 1.0)));
        ggetRandomAngle();
        x_ = idoutyou_*cos(random_angle);
        y_ = idoutyou_*sin(random_angle);
        move_pose_x_ = pose_out.pose.position.x + x_;
        move_pose_y_ = pose_out.pose.position.y + y_;
        move_pose_ = sqrt(x_*x_ + y_*y_);
        pre_dis = sqrt(pow((sub_pose_out.pose.position.x - pose_out.pose.position.x), 2) + pow((sub_pose_out.pose.position.y - pose_out.pose.position.y), 2));
        newposdis = sqrt(pow((move_pose_x_ - pose_out.pose.position.x), 2) + pow((move_pose_y_ - pose_out.pose.position.y), 2));


        Vxx = pow(std::max(MIN_kyori_x, 0.2) / 3 , 2 );
        Vyy = pow(std::max(MIN_kyori_y, 0.2) / 3 , 2 );

        Vxy = 0;//((Vxx - Vyy)*tan(2*pose_out_yaw * 180.0 / M_PI)) / 2;
        // 2x2行列の定義
        A << Vxx, Vxy,
            Vxy, Vyy;

        double det_sigma = A.determinant();
        // ベクトルの定義
        pre_x << sub_pose_out.pose.position.x - pose_out.pose.position.x,
                 sub_pose_out.pose.position.y - pose_out.pose.position.y;

        // ベクトルの定義
        new_x << move_pose_x_ - pose_out.pose.position.x,
                 move_pose_y_ - pose_out.pose.position.y;

        double exponent = -0.5 * (pre_x.transpose() * A.inverse() * pre_x)(0,0);
        x_1_ = (1.0 / (2.0 * M_PI * sqrt(det_sigma))) * std::exp(exponent);

        double exponent2 = -0.5 * (new_x.transpose() * A.inverse() * new_x)(0,0);
        x_2_ = (1.0 / (2.0 * M_PI * sqrt(det_sigma))) * std::exp(exponent2);

        SSS_ = x_2_ / x_1_;
        double roll = 0.0;    // X軸周りの回転
        double pitch = 0.0;   // Y軸周りの回転
        double yaw = random_angle;    // Z軸周りの回転（90度＝1.57ラジアン）
        tf2::Quaternion q;
        // オイラー角からクォータニオンに変換
        geometry_msgs::Quaternion quaternion;
        q.setRPY(roll, pitch, yaw);
        quaternion.x = q.x();
        quaternion.y = q.y();
        quaternion.z = q.z();
        quaternion.w = q.w();


        if( SSS_ > random_value )//  SSSの条件分岐
        {
            // ロボットの位置を更新
            sub_goal.header.frame_id = FRAME_ROBOT_BASE;
            sub_goal.header.stamp = ros::Time::now();
            sub_goal.pose.position.x = move_pose_x_;
            sub_goal.pose.position.y = move_pose_y_;
            sub_goal.pose.position.z = 0.0;
            sub_goal.pose.orientation.x = quaternion.x;
            sub_goal.pose.orientation.y = quaternion.y;
            sub_goal.pose.orientation.z = quaternion.z;
            sub_goal.pose.orientation.w = quaternion.w;
            // number = 2;
            ROS_INFO("seikou");
            goalpublisher();
        }
        
    }
    else if ( kyori > 5 && goal_status.data == 1)//action_data_.status_list[0].status == 0 || //フォロワがリーダーから離れすぎた場合にリーダーの位置へ行くようにする
    {
        // ロボットの位置を更新
        ggetRandomAngle();
        sub_goal.header.frame_id = FRAME_ROBOT_BASE;
        sub_goal.header.stamp = ros::Time::now();
        sub_goal.pose.position.x = 1.5*cos(random_angle) + pose_out.pose.position.x;
        sub_goal.pose.position.y = 1.5*sin(random_angle) + pose_out.pose.position.y;
        sub_goal.pose.position.z = 0.0;
        sub_goal.pose.orientation.x = -0.0000365853737606;
        sub_goal.pose.orientation.y = 0.00386090210218;
        sub_goal.pose.orientation.z = 0.00758096193567;
        sub_goal.pose.orientation.w = 0.999963809901;
        // number = 1;
        ROS_INFO("restart!!");
        goalpublisher();
    }

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix2d> eig_solver(A);
    Eigen::Vector2d eig_values = eig_solver.eigenvalues();
    Eigen::Matrix2d eig_vectors = eig_solver.eigenvectors();

    visualization_msgs::Marker ellipse;
    ellipse.header.frame_id = "map";
    ellipse.header.stamp = ros::Time::now();
    ellipse.ns = "covariance_ellipse";
    ellipse.id = 0;
    ellipse.type = visualization_msgs::Marker::LINE_STRIP;
    ellipse.action = visualization_msgs::Marker::ADD;
    ellipse.pose.orientation.w = 1.0;
    ellipse.scale.x = 0.03;

    ellipse.color.r = 1.0;
    ellipse.color.g = 0.0;
    ellipse.color.b = 0.0;
    ellipse.color.a = 1.0;

    const int resolution = 100;
    for (int i = 0; i <= resolution; ++i)
    {
        double theta = 2.0 * M_PI * i / resolution;
        Eigen::Vector2d unit_circle(std::cos(theta), std::sin(theta));

        // ✅ 修正された積の順序と型
        Eigen::Vector2d ellipse_point = eig_vectors * eig_values.cwiseSqrt().asDiagonal() * 3.0 * unit_circle;

        geometry_msgs::Point p;
        p.x = ellipse_point.x() + pose_out.pose.position.x;
        p.y = ellipse_point.y() + pose_out.pose.position.y;
        p.z = 0.0;
        ellipse.points.push_back(p);
    }
    // publish
    marker_pub.publish(ellipse);

    visualization_msgs::Marker marker_sub_goal;
    marker_sub_goal.header.frame_id = "map";  // 基準座標系
    marker_sub_goal.header.stamp = ros::Time::now();

    // マーカーの形状を円柱（CYLINDER）にする
    marker_sub_goal.type = visualization_msgs::Marker::CYLINDER;

    // 位置（地面に円を置くためにz座標を少し上げる）
    marker_sub_goal.pose.position.x = sub_goal.pose.position.x;
    marker_sub_goal.pose.position.y = sub_goal.pose.position.y;
    marker_sub_goal.pose.position.z = 0.01;  // わずかに浮かせる

    // サイズ（円の直径と厚み）
    marker_sub_goal.scale.x = 0.1;  // 直径
    marker_sub_goal.scale.y = 0.1;  // 直径
    marker_sub_goal.scale.z = 0.01; // 厚み（これを小さくすることで「円」になる）

    // 色（緑色の円）
    marker_sub_goal.color.r = 0.0;
    marker_sub_goal.color.g = 1.0;
    marker_sub_goal.color.b = 0.0;
    marker_sub_goal.color.a = 1.0;  // 透明度（1.0で不透明）

    // 永続表示
    marker_sub_goal.lifetime = ros::Duration();

    marker_sub_goal_pub.publish(marker_sub_goal);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "aaa");
    ros::NodeHandle nh;
    ros::NodeHandle n("~");
    n.getParam("FRAME/ROBOT_BASE",FRAME_ROBOT_BASE);
    n.getParam("HEIKINTI",param1);
    n.getParam("BUNNSANN",param2);
    static tf2_ros::TransformListener tfListener(tf_buffer_);
    // シードを設定（現在時刻を使う）
    srand(time(NULL));
    // 一度だけセットする変数
    double my_variable = 0.0;
    // 変数がセット済みかどうかを確認するフラグ

    
    // エンコーダデータをサブスクライブ
    marker_pub = nh.advertise<visualization_msgs::Marker>("visualization_marker", 10);
    marker_sub_goal_pub = nh.advertise<visualization_msgs::Marker>("marker_sub_goal", 10);
    ros::Subscriber Group_radius = nh.subscribe("/Group_radius", 10, Group_radiusCallback);
    ros::Subscriber clusterling_sub = nh.subscribe("clusterdata", 10, clusterlingCallback);
    ros::Subscriber encoder_sub = nh.subscribe("/main/odom", 10, encoderCallback);
    ros::Subscriber goal_action_sub = nh.subscribe("goal_status", 10, goalactionCallback);
    newgoal_pub = nh.advertise<geometry_msgs::PoseStamped>("sub_goal_position", 10);
    ros::Subscriber sub_encoder_sub = nh.subscribe("odom", 10, sub_encoderCallback);

    
    ros::spin();
    return 0;
}
