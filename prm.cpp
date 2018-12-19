#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <time.h>       /* time */
#include "node_class.h"
#include <vector>

using namespace cv;

/// Global variables
Mat src, erosion_dst;

int erosion_elem = 0;
int erosion_size = 5;

int const max_elem = 2;
int const max_kernel_size = 21;

int rows;
int cols;


/** Function Headers */
void Erosion( int, void* );
float EuclideanDistance (int, int);
void ascending_order(int**, int);
bool obstacle (int, int);


int main( int argc, char** argv )
{

 	srand (time(NULL));
 	int ale;
 	int l,m;
 	char cimg;
 	
 	
 	char* imageName = argv[1];

	src = imread(imageName, 0); //loading an image = greyscale
	
	if( argc != 2 || !src.data ) {
   		printf( " No image data \n" );
		return -1;
	}

	imshow( "src", src );
  	Erosion( 0, 0 );
  
 	 

	rows = erosion_dst.rows;
	cols = erosion_dst.cols;
	
	int points = rows*cols/1000;
	int pointsaf = 0;
	int aux;
	int pix_node[points];	
	
	//cvtColor(erosion_dst, cimg, CV_GRAY2RGB);
	
	for (int j = 0; j < points; j++)
	{
		ale = rand() %(rows*cols);
		l = ale/cols; 		
		m = (ale - l*cols);
		aux =  erosion_dst.at<uchar>(l,m);

		if (aux == 254)
		{
			erosion_dst.at<uchar>(l,m) = 150;
			pix_node[pointsaf] = j;
			pointsaf ++; 	
		}
	}	
	


	//nodes number
	int numk = 4;	
	int start = 29; //pixel start
	int goal = 32; //pixel goal
	int min, j, current,l1,m1;
	int** vec;
	int ad;
	std::vector<Node_class> node_vec;
	
	//matrice with number of pixel and distance for each node
	vec = new int* [2];
	for (int i=0; i<2; i++){
		vec[i] = new int[pointsaf-1];
	} 

	//search for the the n closest points	
	for (int n = 0; n < rows*cols; n++) // go through the matriz (image)
	{
		l = n/cols; 		
		m = (n - l*cols);
		
		aux = erosion_dst.at<uchar>(l,m);

		if (aux == 150)//detect a point
		{
			Node_class new_node;
			new_node.set_x(n);
			node_vec.push_back(new_node);
			
			for (int i = 0; i < rows*cols; i++)// go through the matriz (image)
			{
				for(ad = 0; ad < pointsaf-1; ad++)
				{
					if ((aux == 150) && (i != n))//detect a point
					{	
						vec[0][ad] = EuclideanDistance(n,i);
						vec[1][ad] = i; 
						ad++;
	
					} 
				}
			
			}std::cout << "I'm here \n";
			//add list and draw line
			//no_cur.neighbors.insert(min);
			ascending_order(vec,(pointsaf-1));
	
			for (int f = 0; f < numk; f++)
			{
				if( !obstacle(n, vec[1][f]) )
				{
					new_node.set_neig(vec[1][f], vec[0][f]);
					
					//print line in the image
					l1 = vec[1][f]/cols; 		
					m1 = (vec[1][f] - l*cols);
					Point op1,op2;		
						op1.x = l;
						op1.y = m;
						op2.x = l1;
						op2.y = m1;			
					line(erosion_dst,op1,op2,150,1);
				}
			}	
	
		}
	
	}
	
	
	//optimal path
	current = start;

	while (current != goal)
	{		

	}	
	
	imshow( "prm", erosion_dst );

  	waitKey(0);
	  
  	return 0;
}



//*  @function Erosion 
void Erosion( int, void* )
{
  int erosion_type;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );

  /// Apply the erosion operation
  erode( src, erosion_dst, element );
  imshow( "Erosion Demo", erosion_dst );
}



float EuclideanDistance (int p1, int p2)
{
	int p1x, p1y, p2x, p2y;
	p1x = p1/cols; 		
	p1y = (p1 - p1x*cols); 
	
	p2x = p2/cols; 		
	p2y = (p2 - p2x*cols);

	return sqrt(pow((p1x - p2x), 2) + pow((p1y - p2y), 2));
}

void ascending_order(int** numero, int total)
{
 
	int i, j, aux1, aux2;
 
          for( i=0; i<total; i++ )
          {
                  for( j=i+1; j<total; j++ ){
                       if( numero[0][i] > numero[0][j] )
                       {
                          
                           aux1 = numero[0][i];
                           aux2 = numero[1][i];
                           
                           numero[0][i] = numero[0][j];
                           numero[1][i] = numero[1][j];
                           
                           numero[0][j] = aux1;
                           numero[1][j] = aux2;
                       
                       }
                  }
           }
           
}

bool obstacle (int p1, int p2)
{
	int p1x, p1y, p2x, p2y, m, fun, aux;
	
	p1x = p1/cols; 		
	p1y = (p1 - p1x*cols); 
	
	p2x = p2/cols; 		
	p2y = (p2 - p2x*cols);
	
	m =  (p2y - p1y)/(p2x - p1x); //coefficient
	
	for (int j = 0; j <= p2x; j++) //all points in the function -> change x
	{
		fun = m*(j - p1x) + p1y; //found y
		aux = erosion_dst.at<uchar>(j,fun); //see the pixel x,y
		
		if (aux != 254)
			return true; //obstacle detected
	} 	

	return false;
}


