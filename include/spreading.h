#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <potbot_lib/Utility.h>
#include <tf/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/PolygonStamped.h>
#include <visualization_msgs/MarkerArray.h>
#include <string>
#include <math.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/Point.h>
#include <random>     // 乱数生成器
#include <cmath>

//クラスの定義
class SpreadingClass{
    private:
		ros::NodeHandle nh_;
		ros::Subscriber sub_my_encoder;//, sub_main_position;
        ros::Publisher pub_cmd_vel;//, pub_pan_, pub_tilt_, marker_pub_;
        tf2_ros::Buffer tf_buffer_;

        std::string FRAME_ROBOT_BASE;
        double delta=0, heikinnti=0;
        geometry_msgs::PointStamped odom_point_;
        geometry_msgs::PoseStamped target_point_;
        nav_msgs::Odometry odomdata_;
        geometry_msgs::PoseStamped pre_point_;
        geometry_msgs::PoseStamped pose_out_,newpose_;
        geometry_msgs::TransformStamped transformStamped_;

        void __setLaunchParam();
        void __encoderCallback(const nav_msgs::Odometry::ConstPtr& msg);
        void __spreading_order();
        
    public:
        SpreadingClass();
        ~SpreadingClass();
        
        void mainloop();
        
};