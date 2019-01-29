#include "Listener_tf.hpp"



Listener_tf::Listener_tf(ros::NodeHandle& n) {
	_client = n.serviceClient<nav_msgs::GetMap>("/dynamic_map");
	_flag = false;
	_finish = false;
	_mouse = n.subscribe<geometry_msgs::PointStamped>("/mouse_point", 10, &Listener_tf::setTarget, this);
}


void Listener_tf::setTarget(const geometry_msgs::PointStamped::ConstPtr& target) {
	_targetPos = MapPos(target->point.x,target->point.y);
	_finish = true;
	std::cout << "Target node setted." << std::endl;
}


bool Listener_tf::listen(ros::NodeHandle& n) {
	if (_client.call(_srv)) {
		nav_msgs::OccupancyGrid map = _srv.response.map;
		nav_msgs::MapMetaData mapData = map.info;
		
		// Get Map image
		cv::Mat imgMap(mapData.height, mapData.width, CV_8UC1, map.data.data());
		cv::Mat imgMapInv = cv::Scalar::all(255) - imgMap;
		imgMapInv.copyTo(_map);
		cv::flip(imgMapInv, _map, 0);
		
		// Get Dynamic infos
		_infoMap = Dynamic( static_cast<double>(mapData.resolution), mapData.origin.position.x, mapData.origin.position.y );
		
		// Get Robot Pos
		tf::StampedTransform stamp;
		bool posFound = false;
		while(!posFound) {
			try{
				_listener.lookupTransform("/map", "/base_link", ros::Time(0), stamp);
				posFound = true;
			}
			catch (tf::TransformException ex){
				ROS_ERROR("%s",ex.what());
				ros::Duration(1.0).sleep();
			}
		}
		_initPos = MapPos(stamp.getOrigin().x(), stamp.getOrigin().y());
		
		
		//Get Target Pos
		boost::shared_ptr<geometry_msgs::PointStamped const> sharedPtr;
		geometry_msgs::PointStamped ps;
		std::cout << "Set the target with rviz window" << std::endl;
		sharedPtr = ros::topic::waitForMessage<geometry_msgs::PointStamped>("/mouse_point", n);
		
		
		while (!_finish)
		{
			ros::spinOnce();
		}
		
		
		// Initialisation done
		_flag = true;
	}
	else
	{
		ROS_ERROR("ERROR : Listenning of MapListenner fucked up.");
		return false;
	}
	
}



void Listener_tf::showMap() {
	if(_flag) {
		cv::imshow( "Image Map", _map );
		cv::waitKey(0);
	}
	else std::cout << "No map received, can not show it." << std::endl;
}



void Listener_tf::dispData() {
	if(_flag) {
		std::cout << "Pos : " << _initPos.x << "," << _initPos.y << std::endl
		<< "Target : " << _targetPos.x << "," << _targetPos.y << std::endl
		<< "Resolution : " << _infoMap.resolution << std::endl
		<< "Origin = " << _infoMap.xOrigin << "," << _infoMap.yOrigin << std::endl;
	}
	else std::cout << "No transform received, can not show anything." << std::endl;
}
