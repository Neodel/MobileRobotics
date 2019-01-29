#ifndef MAP_HPP
#define MAP_HPP

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>      //for imshow
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#include "Listener_tf.hpp"
#include "utils.h"

class Map{
	
private:
	cv::Mat _coloredImage;
	cv::Mat _referenceImage;
	int _height;
	int _width;
	Dynamic _infoMap;
	Pos _posRobot;
	Pos _posTarget;
	std::vector<MapPos> _vOutputPath;
	
	
public:
	
	// Constructor
	Map(Listener_tf& listener_tf);
	
	// Getter
	int getHeight(){return _height; }
	int getWidth(){return _width; }
	int getVal(int i, int j);
	Pos getPosRobot(){ return _posRobot; }
	Pos getPosTarget(){ return _posTarget; }
	Dynamic getinfoMap(){return _infoMap; }
	
	// Methods
	Pos convertMeters(MapPos mp);
	bool writeOutputPath(std::string const outputPath);
};

#endif
