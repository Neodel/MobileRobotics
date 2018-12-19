#include "node_class.h"

Node_class::Node_class(int xa)
{
    _x = xa; //attributed
    std::vector<Node_class> _neighbors;
    
}

void Node_class::set_neig(int x, int dist)
{
	
	Node_class neighbor;
	neighbor.set_dist(dist);
	neighbor.set_x(x);
	_neighbors.push_back(neighbor);	
} 

void Node_class::set_dist(int d)
{
	_dist = d;
}

void Node_class::set_x(int x)
{
	_x = x;
}
