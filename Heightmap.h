#ifndef __HEIGHTMAP.H__
#define __HEIGHTMAP.H__

#include "Vector3d.h"
#include <vector>

class Heightmap {

	static const int DEFAULT_NUMROWS  = 100;
	static const int DEFAULT_NUMCOLS  = 100;
	static const float PI 			  = 3.141592654f;
	static const int CIRCLE_ALGORITHM = 0;
	static const int FAULT_ALGORITHM  = 1;
	int algorithmToUse;

	int numRows = DEFAULT_NUMROWS;
	int numCols = DEFAULT_NUMCOLS;

	int numCircles = 1; //default number of circles

	float circleSize;	// diameter of circle to consider for algorithm
	float heightDisplacement;  //what height to displace a given point

	float maxHeightInMap; //a trackking variable to know the max heights


	std::vector<int> circleCentersIndex;
	std::vector< std::vector<Vector3d> > map;


	void initCircleCenters();
	void initMap();
	void heightTransformCircleAlg();
	void heightTransformFaultAlg();




	public:
		Heightmap(int rows, int cols, int algorithm);

		std::vector< std::vector<Vector3d> > getMap();

		float getMaxHeight();



};

#endif