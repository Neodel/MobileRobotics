#ifndef LISTENER_TF_HPP
#define LISTENER_TF_HPP

#include "ros/ros.h"
#include <cstdlib>
#include <nav_msgs/GetMap.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/MapMetaData.h>
#include <std_msgs/Header.h>
#include <tf/transform_listener.h>

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/shared_ptr.hpp>

#include "utils.h"



class Listener_tf{
	
private:
	cv::Mat _map;
	MapPos _initPos;
	MapPos _targetPos;
	Dynamic _infoMap;
	//ros::NodeHandle _nh;
	tf::TransformListener _listener;
	ros::Subscriber _mouse;
	nav_msgs::GetMap _srv;
	ros::ServiceClient _client;
	
	bool _flag;
	bool _finish;
	
public:
	// Constructor - Destructor
	Listener_tf(ros::NodeHandle& n);
	
	// Usefull func
	bool listen(ros::NodeHandle& n);
	
	// Getter - Setter
	cv::Mat getMap(){ return _map;}
	Dynamic getInfoMap(){ return _infoMap;}
	MapPos getInitPos(){ return _initPos;}
	MapPos getTargetPos(){ return _targetPos;}
	void setTarget(const geometry_msgs::PointStamped::ConstPtr& target);
	void finished(){ _finish = false;}
	bool getFinish(){ return _finish; }
	
	// To see something
	void showMap();
	void dispData();
};

#endif /* Listener_tf_hpp */
