#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <Eigen/Dense>
#include <cmath>

// 連続カラーマップ（MATLAB jet 風）
std_msgs::ColorRGBA jetColorSmooth(double value)
{
    std_msgs::ColorRGBA c;
    c.a = 1.0;
    if (value < 0) value = 0;
    if (value > 1) value = 1;

    // 0→1 の範囲を 6 区間に分けて滑らかに補間
    if (value < 0.167) {          // 青→水色
        double t = value / 0.167;
        c.r = 0.0;
        c.g = t;
        c.b = 1.0;
    } else if (value < 0.333) {   // 水色→緑
        double t = (value - 0.167)/0.166;
        c.r = 0.0;
        c.g = 1.0;
        c.b = 1.0 - t;
    } else if (value < 0.5) {     // 緑→黄
        double t = (value - 0.333)/0.167;
        c.r = t;
        c.g = 1.0;
        c.b = 0.0;
    } else if (value < 0.667) {   // 黄→オレンジ
        double t = (value - 0.5)/0.167;
        c.r = 1.0;
        c.g = 1.0 - t*0.5;
        c.b = 0.0;
    } else if (value < 0.833) {   // オレンジ→赤
        double t = (value - 0.667)/0.166;
        c.r = 1.0;
        c.g = 0.5 - 0.5*t;
        c.b = 0.0;
    } else {                       // 赤濃いめ
        double t = (value - 0.833)/0.167;
        c.r = 1.0;
        c.g = 0.0;
        c.b = 0.0;
    }

    return c;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "mahalanobis_surface_multicolor_dense");
    ros::NodeHandle nh;
    ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>("mahalanobis_surface_multicolor_dense", 1);

    ros::Rate rate(1.0);

    Eigen::Vector2d mu(0.0,0.0);
    double sigma_q = 2.25;
    Eigen::Matrix2d Sigma;
    Sigma << 9, 0.0,
             0.0, 4;
    Eigen::Matrix2d invSigma = Sigma.inverse();

    double step = 0.01;  // 🔹 点をさらに密に
    double range = 5.0;

    while(ros::ok())
    {
        visualization_msgs::Marker marker;
        marker.header.frame_id = "map";
        marker.header.stamp = ros::Time::now();
        marker.ns = "mahalanobis_surface_multicolor_dense";
        marker.id = 0;
        marker.type = visualization_msgs::Marker::SPHERE_LIST;
        marker.action = visualization_msgs::Marker::ADD;
        marker.scale.x = 0.015;  // 点をさらに小さく
        marker.scale.y = 0.015;
        marker.scale.z = 0.015;

        for(double x=-range; x<=range; x+=step){
            for(double y=-range; y<=range; y+=step){
                Eigen::Vector2d d(x,y);
                double q = d.transpose()*invSigma*d;
                double z = exp(-std::pow(q - 2.25,2)/(2*sigma_q*sigma_q));
                double z_scaled = z*5.0;

                geometry_msgs::Point p;
                p.x = x;
                p.y = y;
                p.z = z_scaled;

                std_msgs::ColorRGBA c = jetColorSmooth(z);

                marker.points.push_back(p);
                marker.colors.push_back(c);
            }
        }

        marker_pub.publish(marker);
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
