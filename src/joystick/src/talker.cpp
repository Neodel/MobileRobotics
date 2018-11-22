#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"

#include <iostream>


int main(int argc, char **argv)
{
	ros::init(argc, argv, "talker");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);
	ros::Rate loop_rate(10);

	int count = 0;
  	while (ros::ok())
	{
    	/**
     	* This is a message object. You stuff it with data, and then publish it.
     	*/
    	geometry_msgs::Twist T;
    	T.linear.x = 1;
    	T.linear.y = 0;
		T.linear.z = 0;

    	//ROS_INFO("%s", T);
	    //ROS_INFO_STREAM("Twist with info stream " << T );
    	
    	std::cout << "Twist  : "<< T << std::endl; 

    	/**
     	* The publish() function is how you send messages. The parameter
     	* is the message object. The type of this object must agree with the type
     	* given as a template parameter to the advertise<>() call, as was done
     	* in the constructor above.
     	*/
    	chatter_pub.publish(T);

    	ros::spinOnce();

    	loop_rate.sleep();
    	++count;
  	}
  	return 0;
}
