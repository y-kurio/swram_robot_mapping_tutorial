#include <create_goal.h>


void goalactionCallback(const actionlib_msgs::GoalStatusArray::ConstPtr& msg)
{
    action_data_ = *msg;
}

geometry_msgs::PointStamped get_tf(const tf2_ros::Buffer &buffer, const geometry_msgs::PointStamped& point_in, const std::string target_frame_id)
        {
            geometry_msgs::PoseStamped pose_stamped_in;
            pose_stamped_in.header = point_in.header;
            pose_stamped_in.pose = get_pose(point_in.point,0,0,0);
            geometry_msgs::PoseStamped pose_out = get_tf(buffer, pose_stamped_in, target_frame_id);
            geometry_msgs::PointStamped point_out;
            point_out.header = pose_out.header;
            point_out.point = pose_out.pose.position;

            return point_out;
        }

void encoderCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    odomdata_ = *msg;
    // 0からRAND_MAXまでの整数を生成し、0.0〜1.0の範囲にスケール
    double random_value = static_cast<double>(rand()) / RAND_MAX;
    ROS_INFO("Random value: %f", random_value);
    double heikinnti = param1;
    double bunnsann = param2;
    
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
        ROS_INFO("Random value: %f", random_value);
        ROS_WARN_STREAM("get_tf TF2 exception: " << ex.what());
    }
    
    tf2::doTransform(pre_point, pose_out, transformStamped);
    
        // ROS_INFO("FRAME_ROBOT_BASE=%s",FRAME_ROBOT_BASE);
        std::cout << "x: " << pose_out.pose.position.x << std::endl;
        std::cout << "y: " << pose_out.pose.position.y << std::endl;
        std::cout << "goalaction: " << action_data_.status_list[0].status << std::endl;
    if (action_data_.status_list[0].status == 3)
    {
        ROS_INFO("seikou");
        idoutyou_ = sqrt(-2.0 * param1 * param1 * log(-(random_value - 1.0)));
        x_ = idoutyou_*random_value;
        y_ = idoutyou_*sqrt(1.0-(random_value*random_value));
        move_pose_x_ = pose_out.pose.position.x + x_;
        move_pose_y_ = pose_out.pose.position.y + y_;
        move_pose_ = sqrt(x_*x_ + y_*y_);
        pre_dis = sqrt((pose_out.pose.position.x - 0.0)*(pose_out.pose.position.x - 0.0) + (pose_out.pose.position.y - 0.0)*(pose_out.pose.position.y - 0.0));
        newposdis = sqrt((move_pose_x_ - 0.0)*(move_pose_x_ - 0.0) + (move_pose_y_ - 0.0)*(move_pose_y_ - 0.0));
        x_1_ = (1.0 / sqrt(2.0 * 3.141592 * (heikinnti)*(heikinnti))) * exp(-((pre_dis - bunnsann)*(pre_dis - bunnsann)) / (2.0 * (heikinnti)*(heikinnti)));
        x_2_ = (1.0 / sqrt(2.0 * 3.141592 * (heikinnti)*(heikinnti))) * exp(-((newposdis - bunnsann)*(newposdis - bunnsann)) / (2.0 * (heikinnti)*(heikinnti)));
        SSS_ = x_2_ / x_1_;
                
        //  SSSの条件分岐
        if( SSS_ > random_value )
        {
            // ロボットの位置を更新
            goal = x_;
            currentpos_x = x_;
            currentpos_y = y_;
        }
        else
        {
            // x_ = idoutyou_*random_value;
            // y_ = idoutyou_*sqrt(1.0-(random_value*random_value));
            // move_pose_x_ = pose_out.pose.position.x + x_;
            // move_pose_y_ = pose_out.pose.position.y + y_;
            // move_pose_ = sqrt(x_*x_ + y_*y_);
            // pre_dis = sqrt((pose_out.pose.position.x - 0.0)*(pose_out.pose.position.x - 0.0) + (pose_out.pose.position.y - 0.0)*(pose_out.pose.position.y - 0.0));
            // newposdis = sqrt((move_pose_x_ - 0.0)*(move_pose_x_ - 0.0) + (move_pose_y_ - 0.0)*(move_pose_y_ - 0.0));
            // x_1_ = (1.0 / sqrt(2.0 * 3.141592 * (heikinnti)*(heikinnti))) * exp(-((pre_dis - bunnsann)*(pre_dis - bunnsann)) / (2.0 * (heikinnti)*(heikinnti)));
            // x_2_ = (1.0 / sqrt(2.0 * 3.141592 * (heikinnti)*(heikinnti))) * exp(-((newposdis - bunnsann)*(newposdis - bunnsann)) / (2.0 * (heikinnti)*(heikinnti)));
            goal = x_;
            currentpos_x = x_;
            currentpos_y = y_;
        }
    }

    // idoutyou_ = sqrt(-2 * param1 * param1 * log(-(random_value - 1)));
    // x_ = idoutyou_*random_value;
    // y_ = idoutyou_*sqrt(1.0-random_value^2);
    // move_pose_x_ = pose_out.pose.position.x + x_;
    // move_pose_y_ = pose_out.pose.position.x + y_;
    // move_pose_ = sqrt(x_^2 + y_^2);
    // pre_dis = sqrt((pose_out.pose.position.x - centerRow)^2 + (pose_out.pose.position.y - centerCol)^2);
    // newposdis = sqrt((x_ - centerRow)^2 + (y_ - centerCol)^2);
    // x_1_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((pre_dis - param2)^2) / (2 * (param1)^2));
    // x_2_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((newposdis - param2)^2) / (2 * (param1)^2));
    // SSS_ = x_2_ / x_1_;
            
    // //  SSSの条件分岐
    // if( SSS_ > random_value )
    // {
    //     // ロボットの位置を更新
    //     goal = x_;
    //     currentpos_x = x_;
    // }
    // else
    // {
    //     x_ = random;
    //     y_ = sqrt(1-x^2);
    //     move_pose_x_ = currentpos_x + x_;
    //     move_pose_y_ = currentpos_y + y_;
    //     move_pose_ = sqrt(x_^2 + y_^2);
    //     pre_dis = sqrt((pose_out.pose.position.x - centerRow)^2 + (pose_out.pose.position.y - centerCol)^2);
    //     newposdis = sqrt((x_ - centerRow)^2 + (y_ - centerCol)^2);
    //     x_1_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((pre_dis - param2)^2) / (2 * (param1)^2));
    //     x_2_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((newposdis - param2)^2) / (2 * (param1)^2));
    // }
        

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
    
    // エンコーダデータをサブスクライブ
    ros::Subscriber encoder_sub = nh.subscribe("/robot_2/odom", 10, encoderCallback);
    ros::Subscriber goal_action_sub = nh.subscribe("/robot_2/move_base/status", 10, goalactionCallback);
    ros::publisher sub_goal_pub = nh.advertice
    
    ros::spin();
    return 0;
}