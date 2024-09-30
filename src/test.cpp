// #include <ros/ros.h>
// #include <tf2_ros/transform_listener.h>
// #include <geometry_msgs/TransformStamped.h>
// int main(int argc, char** argv) {
//     ros::init(argc, argv, "tf_buffer_example");
//     ros::NodeHandle nh;

//     // Create a TF buffer and listener
//     tf2_ros::Buffer tfBuffer;
//     tf2_ros::TransformListener tfListener(tfBuffer);

//     ros::Rate rate(10.0);
//     while (nh.ok()) {
        
//         for(const auto& sp : Cluster_Minpts_.pt)
//     {
//         source_point.header = source_header;
//         source_point.point = sp;
//         // ROS_INFO("Transformed Point: x=%f, y=%f, z=%f", source_point.point.x, source_point.point.y, source_point.point.z);
        
//         try
//         {
//             target_point = potbot_lib::utility::get_tf(tf_buffer_, source_point, FRAME_ROBOT_BASE);

//         // ターゲットフレームでの座標を表示
//         // ROS_INFO("Transformed Point: x=%f, y=%f, z=%f", target_point.point.x, target_point.point.y, target_point.point.z);
//         } catch (tf::TransformException& ex) {
//             ROS_ERROR("%s", ex.what());
//             return;
//         }

//         transformed_points.push_back(target_point.point);

            
            

//         rate.sleep();
//     }

//     }
//     return 0;
// }