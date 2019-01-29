#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PointStamped.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Listener_tf.hpp"
#include "Map.hpp"
#include "Tree.hpp"
#include "utils.h"

#define PI 3.141597

std::vector<MapPos> interpolationTrajectory(std::vector<MapPos> vPosPts)
{
	std::vector<MapPos> vPosInterpol;
	
	// ajout du premier point
	if (vPosPts.size() > 0)
		vPosInterpol.push_back(vPosPts[0]);
	else
	{
		std::cout << "(WARNING) Empty list of map pos (in path_follower::interpolationTrajectory)" << std::endl;
		return vPosInterpol;
	}
	
	// pour chaque point de la carte
	for(size_t iPtMap(1) ; iPtMap < vPosPts.size() ; ++iPtMap) // entre n et n-1 : début = 1
	{
		
		double xlast = vPosPts[iPtMap-1].x;
		double ylast = vPosPts[iPtMap-1].y;
		double x = vPosPts[iPtMap].x;
		double y = vPosPts[iPtMap].y;
		
		
		// sampling
		float norm(std::sqrt((xlast - x)*(xlast - x) + (ylast - y)*(ylast - y)));
		
		int nbPtInterpol = norm/0.5; //Threeshold of interopation
		
		float dx(0);
		float dy(0);
		
		for(int it(0) ; it < nbPtInterpol ; ++it)
		{
			dx += (x - xlast)/(nbPtInterpol+1);
			dy += (y - ylast)/(nbPtInterpol+1);
			
			if (it == 0) // more point near the beginning
				vPosInterpol.push_back(MapPos(xlast+((x - xlast)/(nbPtInterpol+1)/2) ,
											  ylast+((y - ylast)/(nbPtInterpol+1)/2)));
			
			if(nbPtInterpol >= 3) // long distances -> less points
			{
				if(it%2 !=0)
					vPosInterpol.push_back(MapPos(xlast+dx , ylast+dy));
			}
			else{
				vPosInterpol.push_back(MapPos(xlast+dx , ylast+dy));
			}
			
			if (it == nbPtInterpol-1) // more point near the end
				vPosInterpol.push_back(MapPos(xlast+dx+ ((x - xlast)/(nbPtInterpol+1)/2) ,
											  ylast+dy+ ((y - ylast)/(nbPtInterpol+1)/2)));
		}
		
		vPosInterpol.push_back(vPosPts[iPtMap]);
	}
	
	
	return vPosInterpol;
}

geometry_msgs::PointStamped stampedNextPoint(MapPos mp)
{
	geometry_msgs::PointStamped rvizPt2Reach;
	
	rvizPt2Reach.header.stamp = ros::Time::now();
	rvizPt2Reach.header.frame_id = "map";
	
	rvizPt2Reach.point.x = mp.x;
	rvizPt2Reach.point.y = mp.y;
	rvizPt2Reach.point.z = 0.0;
	
	return rvizPt2Reach;
}



nav_msgs::Path setPath(std::vector<MapPos> path)
{
	nav_msgs::Path pathMsg;
	
	pathMsg.header.stamp = ros::Time::now();
	pathMsg.header.frame_id = "map";
	
	for(size_t ind(0); ind<path.size() ; ++ind)
	{
		geometry_msgs::PoseStamped ps;
		
		// Header
		ps.header.stamp = ros::Time::now();
		ps.header.frame_id = "map";
		
		// Postion
		ps.pose.position.x = path[ind].x;
		ps.pose.position.y = path[ind].y;
		ps.pose.position.z = 0;
		
		pathMsg.poses.push_back( ps );
	}
	
	return pathMsg;
}

/********* Main *********/

int main(int argc, char **argv) {
	
	// Initialisation
	ros::init(argc, argv, "my_servicesp");
	ros::NodeHandle n;
	ros::Publisher ptPub = n.advertise<geometry_msgs::PointStamped>("/point_to_reach", 10);
	ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
	ros::Publisher pathPub = n.advertise<nav_msgs::Path>("/shortest_path", 10);
	tf::TransformListener listener_tf;
	
	// Set the map
	Listener_tf listener(n);
	
	// Set up arguments
	while(n.ok()) {
		listener.listen(n);
		// Create map
		Map mapSim(listener);
		// Create map tree
		Tree tree(mapSim);
		// Compute shortest path:
		Pos src(mapSim.getPosRobot());
		Pos dest(mapSim.getPosTarget());
		std::vector<Pos> pixPath = tree.computeShorestPath(src, dest);
		// Convert in meter
		std::vector<MapPos> outputPath;
		for(auto&& v : pixPath) {
			outputPath.push_back(MapPos(mapSim.getinfoMap().getxOrigin() + v.j*1.0*mapSim.getinfoMap().getresolution(), mapSim.getinfoMap().getyOrigin() + mapSim.getinfoMap().getresolution()*(mapSim.getHeight() - v.i )*1.0));
		}
		
		// Compute the trajectory interpolation
		std::vector<MapPos> mPath = interpolationTrajectory(outputPath);
		
		// Set the gazebo path
		nav_msgs::Path gazeboPath = setPath(mPath);
		
		
		// **************************** Robot Control ****************************
		
		
		int iPt2Reach(0); // indice point à atteindre
		double lastErrAngle(0);
		
		
		
		ros::Rate rate(30.0); // frequence de rafraichissement
		std::cout << "Path following..." << std::endl;
		while (listener.getFinish() ) {
			// Affichage du path
			pathPub.publish(gazeboPath); //displayPath(mPath);
			// Lecture posture robot
			tf::StampedTransform transform;
			try{
				listener_tf.lookupTransform("/map", "/base_link", ros::Time(0), transform);
			}
			catch (tf::TransformException &ex){
				ROS_ERROR("%s",ex.what());
				ros::Duration(1.0).sleep();
				continue;
			}
			
			VectPosture vPosRob(transform);
			
			
			// Test passage au point suivant
			if( sqrt((vPosRob.x-mPath[iPt2Reach].x)*(vPosRob.x-mPath[iPt2Reach].x)+(vPosRob.y-mPath[iPt2Reach].y)*(vPosRob.y-mPath[iPt2Reach].y))< 0.1 && iPt2Reach < (mPath.size()-1)) {
				// si on est proche du point à atteindre, et qu'on n'est pas au dernier point
				iPt2Reach++;
			}
			
			if( sqrt((vPosRob.x-mPath[iPt2Reach].x)*(vPosRob.x-mPath[iPt2Reach].x)+(vPosRob.y-mPath[iPt2Reach].y)*(vPosRob.y-mPath[iPt2Reach].y)) < 0.1 && iPt2Reach == (mPath.size()-1) && listener.getFinish()) {
				listener.finished();
				std::cout << "Target reached." << std::endl;
			}
			
			// Affichage du point dans Rviz
			ptPub.publish(stampedNextPoint(mPath[iPt2Reach]));
			
			
			// Calcul du vecteur cible : entre le robot et le prochain point à atteindre :
			MapPos vectToReach( mPath[iPt2Reach].x - vPosRob.x, mPath[iPt2Reach].y - vPosRob.y );
			
			// Calcul angle vecteur cible
			double angle2Reach = std::acos( (vectToReach.x)*1.0/vectToReach.norm() ); //angle orienté v2reach,x0
			if( vectToReach.y < 0  ) // produit vectoriel < 0 -> sin negatif
				angle2Reach= -angle2Reach;
			
			
			// Calcul erreurs
			double errAngle = angle2Reach - vPosRob.yaw ;
			if (errAngle > PI )
			{
				//std::cout << "TOO HIGH : err (avant) = " << errAngle << " err angle après : " << errAngle -2.0*PI << std::endl;
				errAngle -= 2.0*PI;
			}
			if (errAngle < -PI )
			{
				//std::cout << "TOO LOW : err (avant) = " << errAngle << " err angle après : " << errAngle + 2.0*PI << std::endl;
				errAngle += 2.0*PI;
			}
			double errPos = vectToReach.norm();
			
			// PID
			geometry_msgs::Twist twist;
			float kpv = 1;
			float kpw = 1;
			float kdw = 0; // kd a set pour passer sur le vrai robot
			
			float cmdv = kpv*errPos;
			if ( cmdv > 0.5)
				cmdv = 0.5; // Max sped of the robot
			
			twist.linear.x = cmdv;
			twist.angular.z = kpw*errAngle + kdw*(errAngle - lastErrAngle);
			
			cmd_pub.publish(twist);
			
			lastErrAngle = errAngle;
			rate.sleep(); // gère le rafraîchissement
		}
		
		std::cout << "Ending ..." << std::endl;
		for (int iEnd(0); iEnd < 120 ; iEnd++)
		{
			geometry_msgs::Twist twist;
			twist.linear.x = 0;
			twist.angular.z = 0;
			
			cmd_pub.publish(twist);
			rate.sleep();
		}
		
		std::cout << "Ending ok. Another target could be selected." << std::endl;
		
	} // end while(n.ok())
	
	
	return 0;
	
}
