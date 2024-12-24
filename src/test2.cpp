#include <create_goal.h>

void goalactionCallback(const actionlib_msgs::GoalStatusArray::ConstPtr& msg)
{
    action_data_ = *msg;
}

double ggetRandomAngle() {
    // 乱数エンジンの初期化
    std::random_device rd;
    std::mt19937 gen(rd());

    // -180 から 180 までの範囲で一様分布の乱数を生成
    std::uniform_real_distribution<> dis(-180.0, 180.0);

    // 生成された度数法の角度をラジアンに変換して返す
    double angle_degrees = dis(gen);
    return angle_degrees * M_PI / 180.0;
}


void sub_encoderCallback(const nav_msgs::Odometry::ConstPtr& msg){
    sub_odomdata_ = *msg;
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
}

void goalpublisher()
{
    newgoal_pub.publish(sub_goal);
    ROS_INFO("publish OK!!!" );
}

void encoderCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    odomdata_ = *msg;
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

    // 0からRAND_MAXまでの整数を生成し、0.0〜1.0の範囲にスケール
    double random_value = static_cast<double>(rand()) / RAND_MAX;
    // ROS_INFO("Random value: %f", random_value);
    double heikinnti = param1;
    double bunnsann = param2;
    
        // ROS_INFO("FRAME_ROBOT_BASE=%s",FRAME_ROBOT_BASE);
        // std::cout << "x: " << pose_out.pose.position.x << std::endl;
        // std::cout << "y: " << pose_out.pose.position.y << std::endl;
    // std::cout << "goalaction: " << action_data_.status_list[0].status << std::endl;
    double kyori =(sqrt((sub_pose_out.pose.position.x - pose_out.pose.position.x)*(sub_pose_out.pose.position.x - pose_out.pose.position.x) + (sub_pose_out.pose.position.y - pose_out.pose.position.y)*(sub_pose_out.pose.position.y - pose_out.pose.position.y)));
    if (action_data_.status_list.empty()) 
    {
        ROS_INFO("syokai");
        idoutyou_ = sqrt(-2.0 * param1 * param1 * log(-(random_value - 1.0)));
        // 乱数エンジンの初期化
        std::random_device rd;
        std::mt19937 gen(rd());
        ROS_INFO("FRAME_ROBOT_BASE=%s",FRAME_ROBOT_BASE);

        // -180 から 180 までの範囲で一様分布の乱数を生成
        std::uniform_real_distribution<> dis(-180.0, 180.0);
        // 生成された度数法の角度をラジアンに変換して返す
        double angle_degrees = dis(gen);
        double random_angle = angle_degrees * M_PI / 180.0;
        x_ = idoutyou_*cos(random_angle);
        y_ = idoutyou_*sin(random_angle);
        move_pose_x_ = pose_out.pose.position.x + x_;
        move_pose_y_ = pose_out.pose.position.y + y_;
        move_pose_ = sqrt(x_*x_ + y_*y_);
        pre_dis = sqrt((sub_pose_out.pose.position.x - 0.0)*(sub_pose_out.pose.position.x - 0.0) + (sub_pose_out.pose.position.y - 0.0)*(sub_pose_out.pose.position.y - 0.0));
        newposdis = sqrt((move_pose_x_ - 0.0)*(move_pose_x_ - 0.0) + (move_pose_y_ - 0.0)*(move_pose_y_ - 0.0));
        x_1_ = (1.0 / sqrt(2.0 * 3.141592 * (heikinnti)*(heikinnti))) * exp(-((pre_dis - bunnsann)*(pre_dis - bunnsann)) / (2.0 * (heikinnti)*(heikinnti)));
        x_2_ = (1.0 / sqrt(2.0 * 3.141592 * (heikinnti)*(heikinnti))) * exp(-((newposdis - bunnsann)*(newposdis - bunnsann)) / (2.0 * (heikinnti)*(heikinnti)));
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
        if( SSS_ > random_value )
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
            std::cout << "x: " << sub_goal.pose.position.x << std::endl;
            std::cout << "y: " << sub_goal.pose.position.y << std::endl;
            goalpublisher();
        }
    }
    else if (!action_data_.status_list.empty() && action_data_.status_list[0].status == 3)
    {
        ROS_INFO("seikou");
        idoutyou_ = sqrt(-2.0 * param1 * param1 * log(-(random_value - 1.0)));
        // 乱数エンジンの初期化
        std::random_device rd;
        std::mt19937 gen(rd());

        // -180 から 180 までの範囲で一様分布の乱数を生成
        std::uniform_real_distribution<> dis(-180.0, 180.0);
        // 生成された度数法の角度をラジアンに変換して返す
        double angle_degrees = dis(gen);
        double random_angle = angle_degrees * M_PI / 180.0;
        x_ = idoutyou_*cos(random_angle);
        y_ = idoutyou_*sin(random_angle);
        move_pose_x_ = pose_out.pose.position.x + x_;
        move_pose_y_ = pose_out.pose.position.y + y_;
        move_pose_ = sqrt(x_*x_ + y_*y_);
        pre_dis = sqrt((sub_pose_out.pose.position.x - pose_out.pose.position.x)*(sub_pose_out.pose.position.x - pose_out.pose.position.x) + (sub_pose_out.pose.position.y - pose_out.pose.position.y)*(sub_pose_out.pose.position.y - pose_out.pose.position.y));
        newposdis = sqrt((move_pose_x_ - pose_out.pose.position.x)*(move_pose_x_ - pose_out.pose.position.x) + (move_pose_y_ - pose_out.pose.position.y)*(move_pose_y_ - pose_out.pose.position.y));
        x_1_ = (1.0 / sqrt(2.0 * 3.141592 * (heikinnti)*(heikinnti))) * exp(-((pre_dis - bunnsann)*(pre_dis - bunnsann)) / (2.0 * (heikinnti)*(heikinnti)));
        x_2_ = (1.0 / sqrt(2.0 * 3.141592 * (heikinnti)*(heikinnti))) * exp(-((newposdis - bunnsann)*(newposdis - bunnsann)) / (2.0 * (heikinnti)*(heikinnti)));
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
            std::cout << "x: " << sub_goal.pose.position.x << std::endl;
            std::cout << "y: " << sub_goal.pose.position.y << std::endl;
            goalpublisher();
        }
        
    }
    else if ( kyori > 3 && !action_data_.status_list.empty() && action_data_.status_list[0].status == 1)//action_data_.status_list[0].status == 0 || 
    {
        ROS_INFO("retsart!!");
        std::random_device rd;
        std::mt19937 gen(rd());

        // -180 から 180 までの範囲で一様分布の乱数を生成
        std::uniform_real_distribution<> dis(-180.0, 180.0);
        // 生成された度数法の角度をラジアンに変換して返す
        double angle_degrees = dis(gen);
        double random_angle = angle_degrees * M_PI / 180.0;
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

        // ロボットの位置を更新
        sub_goal.header.frame_id = FRAME_ROBOT_BASE;
        sub_goal.header.stamp = ros::Time::now();
        sub_goal.pose.position.x = pose_out.pose.position.x;
        sub_goal.pose.position.y = pose_out.pose.position.y;
        sub_goal.pose.position.z = 0.0;
        sub_goal.pose.orientation.x = quaternion.x;
        sub_goal.pose.orientation.y = quaternion.y;
        sub_goal.pose.orientation.z = quaternion.z;
        sub_goal.pose.orientation.w = quaternion.w;
        std::cout << "x: " << sub_goal.pose.position.x << std::endl;
        std::cout << "y: " << sub_goal.pose.position.y << std::endl;
        goalpublisher();
    }
    // pre_action_data_.status_list[0].status = action_data_.status_list[0].status;
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
    ros::Subscriber encoder_sub = nh.subscribe("/main/odom", 10, encoderCallback);
    ros::Subscriber goal_action_sub = nh.subscribe("move_base/status", 10, goalactionCallback);
    newgoal_pub = nh.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 10);
    ros::Subscriber sub_encoder_sub = nh.subscribe("odom", 10, sub_encoderCallback);
    
    ros::spin();
    return 0;
}