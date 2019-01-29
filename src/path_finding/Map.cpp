#include "Map.hpp"

Map::Map(Listener_tf& listener_tf) {
	// Binary
	listener_tf.getMap().copyTo(_referenceImage);
	
	// BGR
	cv::cvtColor(_referenceImage, _coloredImage, CV_GRAY2BGR);
	
	// Dimensions
	_height = _coloredImage.rows;
	_width = _coloredImage.cols;
	
	// Binarization
	for(int i = 0; i<_height; i++) {
		for(int j = 0; j < _width; j++) {
			if(_referenceImage.at<uchar>(i,j)>200) {
				_referenceImage.at<uchar>(i,j) = 255;
			}
			else {
				_referenceImage.at<uchar>(i,j) = 255;
			}
		}
	}
	
	// Map info init
	_infoMap = Dynamic(listener_tf.getInfoMap());
	_posRobot = convertMeters(listener_tf.getInitPos());
	_posTarget = convertMeters(listener_tf.getTargetPos());
	//std::cout << "pos robot : " << _posRobot.i << "," << _posRobot.j << std::endl
	//		<< "pos target : " << _posTarget.i << "," << _posTarget.j << std::endl;
	
	
	// Erosion
	int erosion_size = 10;
	int erosion_type = 0; // 0 = MORPH_RECT, 1 = MORPH_CROSS
	
	cv::Mat element = cv::getStructuringElement(erosion_type, cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ), cv::Point( erosion_size, erosion_size));
	
	cv::erode( _referenceImage, _referenceImage, element);
}

int Map::getVal(int i,const int j) {
	int val = (int)_referenceImage.at<uchar>(i,j);
	return val;
}

Pos Map::convertMeters(MapPos mp) {
	return Pos((_infoMap.yOrigin - mp.y)/_infoMap.resolution + _height, ( mp.x - _infoMap.xOrigin )/_infoMap.resolution);
}


bool Map::writeOutputPath(std::string const outputPath) {
	// Save infos
	std::ofstream f(outputPath, std::ios::out | std::ios::trunc);
	
	if(f) {
		f << _vOutputPath.size() << std::endl;
		for (auto&& v: _vOutputPath)
			f << v.x << " " <<  v.y << std::endl;
		
		f.close();
	}
	else {
		std::cerr << "Fail to open outputPath.txt !" << std::endl;
		return false;
	}
	
	return true;
}
