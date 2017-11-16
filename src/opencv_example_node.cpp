#include "ros/ros.h"
#include <iostream>
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/Image.h"
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>

using namespace std;
using namespace cv;

// void imageCallback(const sensor_msgs::ImageConstPtr& msg)
// {
//   imshow("image", cv_bridge::toCvShare(msg, "bgr8")->image);
// 
//   cv::Mat image = cv_bridge::toCvShare(msg, "bgr8")->image;
//   cv::GaussianBlur( image, image, cv::Size( 21, 21 ), 5, 5 );
//   cv::namedWindow( "Gaussian Blur", cv::WINDOW_AUTOSIZE );// Create a window for display.
//   cv::imshow( "Gaussian Blur", image );
// 
//   cv::Mat gray;
//   /// Convert the image to grayscale
//   cv::cvtColor( image, gray, CV_BGR2GRAY );
// 
//   cv::namedWindow( "Gray", cv::WINDOW_AUTOSIZE );// Create a window for display.
//   cv::imshow( "Gray", gray );
// 
//   cv::Mat detectedEdges;
// 
//   int lowThreshold=10;
//   int ratio = 3;
//   int kernel_size = 3;
// 
//   cv::Canny( gray, detectedEdges, lowThreshold, lowThreshold*ratio, kernel_size );
//   cv::namedWindow( "edges", cv::WINDOW_AUTOSIZE );// Create a window for display.
//   cv::imshow( "edges", detectedEdges );                   // Show our image inside it.
//   
//   waitKey(30);
// }
// 
// int main(int argc, char **argv)
// {
//   //initialize node
//   ros::init(argc, argv, "cv_example");
// 
//   // node handler
//   ros::NodeHandle n;
//   
//   // subsribe topic
//   ros::Subscriber sub = n.subscribe("/cv_camera/image_raw", 1000, imageCallback);
// 
//   //ros loop
//   ros::spin();
// 
//   return 0;
// }

cv::Mat detectedEdges;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  cv::Mat image = cv_bridge::toCvShare(msg, "bgr8")->image;
  cv::GaussianBlur( image, image, cv::Size( 21, 21 ), 5, 5 );
  
  cv::Mat gray;
  /// Convert the image to grayscale
  cv::cvtColor( image, gray, CV_BGR2GRAY );

  int lowThreshold=10;
  int ratio = 3;
  int kernel_size = 3;

  cv::Canny( gray, detectedEdges, lowThreshold, lowThreshold*ratio, kernel_size );
  
  waitKey(30);
}

int main(int argc, char **argv)
{
  //initialize node
  ros::init(argc, argv, "cv_example");

  // node handler
  ros::NodeHandle n;
  
  // subsribe topic
  ros::Subscriber sub = n.subscribe("/cv_camera/image_raw", 1000, imageCallback);

  // publish
  image_transport::ImageTransport it(n);
  image_transport::Publisher pub = it.advertise("camera/imageEdges", 1);
  
  sensor_msgs::ImagePtr msg;
  
  ros::Rate loop_rate(5);
  while (n.ok()) {
    // Check if grabbed frame is actually full with some content
    if(!detectedEdges.empty()) {
      msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", detectedEdges).toImageMsg();
      pub.publish(msg);
      cv::waitKey(1);
    }
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}