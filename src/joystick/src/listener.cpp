#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"
#include <iostream>


/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
	ROS_INFO("I heard: [%d]", msg->buttons[0]);
}

int main(int argc, char **argv)
{
  	ros::init(argc, argv, "listener");
  	ros::NodeHandle n;
  	ros::Subscriber sub = n.subscribe("/joy", 1, chatterCallback);
  	ros::spin();

  	return 0;
}
