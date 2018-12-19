#include <stdio.h>
#include <iostream>
#include <vector>

//Class node


class Node_class
{
	private:
    	int _x;
    	int _dist;
    	std::vector<Node_class> _neighbors;
    	
    public:  	
    	void set_neig(int, int);
    	void set_dist(int);
    	void set_x(int);
    	Node_class(){}
    	Node_class(int a);
};
