#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <vector>
#include <ctime>
#include <set>

#include "Map.hpp"
#include "utils.h"

#define NB_PTS_MAX 500
#define CONNEXITY_RATE 3 // distance between two points less than (1/CONNEXITY_RATE) of the map diagonal : connexity

class Tree{
	
private:
	Map _map;
	float _distRef;
	
	std::vector<Pos> _nodes;
	
	std::vector<std::pair<int,int>> *G; // graph
	std::vector<int> D; // min distance from the source
	
public:
	Tree(Map const& map);
	
	std::vector<Pos> computeShorestPath(const Pos& src, const Pos& dest); // main function
	
	void generateRandNodes();
	void selectNodes();
	bool isNear(const Pos& p1, const Pos& p2, const float& distRef );
	void initGraph(const Pos& source, const Pos& target);
	bool isWayClear(const Pos& p1, const Pos& p2);
	void dijkstra();
	std::vector<Pos> getShortestPath();
	
	
	void dispNodesInfos();
	
	std::vector<Pos> getNodes(){ return _nodes; }
	
	std::vector<std::pair<int,int>>* getGraph(){ return G; }
	
};

#endif /* Map_tree_hpp */
