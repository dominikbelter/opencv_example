#include "ros/ros.h"
#include <iostream>
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/Image.h"
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>

using namespace std;
using namespace cv;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  cv::Mat image = cv_bridge::toCvShare(msg, "bgr8")->image;
  imshow( "image", image );
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

  ros::spin();

  return 0;
}
