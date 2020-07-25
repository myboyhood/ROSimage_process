#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/CompressedImage.h>
#include <iostream>


using namespace cv;

Mat mat_img;
sensor_msgs::ImageConstPtr msg_pub;

void compress_cb(const sensor_msgs::CompressedImageConstPtr& msg)
{   ROS_INFO("image callback");
    try
    {
        mat_img = imdecode(Mat(msg->data),1);//convert compressed image data to cv::Mat;
        std::cout << "decode" << std::endl;
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert to image!");
    }
}


int main(int argc, char** argv) {
    ros::init(argc, argv, "image_convert");
    ros::NodeHandle nh;
    ros::Rate loop_rate(30);

    ros::Subscriber image_sub = nh.subscribe("/usb_cam/image_raw/compressed",1,compress_cb);
    ros::Publisher image_pub = nh.advertise<sensor_msgs::Image>("/usb_cam/image_raw",1);
//    Mat testimg = imread("/home/wzy/catkin_ws/src/compressimage_to_raw/src/1.jpg");
//    imshow("test",testimg);
//    waitKey(500);


    while(ros::ok())
    {   ros::spinOnce();
        msg_pub = cv_bridge::CvImage(std_msgs::Header(), "bgr8", mat_img).toImageMsg();
        image_pub.publish(msg_pub);
        std::cout << "msg_pub" << std::endl;

        loop_rate.sleep();
    }
    return 0;
}
