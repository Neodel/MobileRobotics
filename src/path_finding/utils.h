#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <limits>

struct Pos{
	
	// Attributes
	int i;
	int j;
	
	//Constructors
	Pos(){
		i = 0;
		j = 0;
	}
	Pos(const int& I, const int& J) {
		i = I;
		j = J;
	}
	
	void disp(){
		std::cout << "*** Pos : (" << i << "," << j << ")" << std::endl;
	}
	
};

struct MapPos{
	
	// Attributes
	float x;
	float y;
	
	//Constructors
	MapPos() {
		x = 0.0;
		y = 0.0;
	}
	MapPos(const float& X, const float& Y) {
		x = X;
		y = Y;
	}
	
	// Methods
	double norm() {
		return sqrt(x*x + y*y);
	}
	void disp(){
		std::cout << "*** Map Pos : (" << x << "," << y << ") m" << std::endl;
	}
};

struct Dynamic{
	
	// Attributes
	float resolution;
	float xOrigin;
	float yOrigin;
	
	// Construcors
	Dynamic() {
		resolution = 0.0;
		xOrigin = 0.0;
		yOrigin = 0.0;
	}
	
	Dynamic(const float& res, const float& xO, const float& yO) {
		resolution = res;
		xOrigin = xO;
		yOrigin = yO;
	}
	
	Dynamic(Dynamic const& mp) {
		resolution = mp.resolution;
		xOrigin = mp.xOrigin;
		yOrigin = mp.yOrigin;
	}
	
	// Methods
	void disp(){
		std::cout << "*** Dynamic : res = " << resolution << " , origin : x : " << xOrigin << " y : " << yOrigin << std::endl;
	}
	
	float getxOrigin() {return xOrigin; }
	float getyOrigin() {return yOrigin; }
	float getresolution() {return resolution; }
	
	
};

struct VectPosture{
	double x, y, z;
	double roll, pitch, yaw;
	
	
	VectPosture(tf::StampedTransform transform)
	{
		tf::Quaternion q(transform.getRotation().x(), transform.getRotation().y(), transform.getRotation().z(), transform.getRotation().w());
		
		x = transform.getOrigin().x();
		y = transform.getOrigin().y();
		z = transform.getOrigin().z();
		
		tf::Matrix3x3 m(q);
		m.getRPY(roll, pitch, yaw);
		
	}
};

#endif /* utils_h */
