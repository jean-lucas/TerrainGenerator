#include "Heightmap.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>



//single constructor
// if rows and cols = -1, use the default values;
Heightmap::Heightmap(int rows, int cols, int algorithm) {


	if (rows == -1 && cols == -1) {
		rows = DEFAULT_NUMROWS;
		cols = DEFAULT_NUMCOLS;
	}

	numRows = rows;
	numCols = cols;

	algorithmToUse = algorithm;

	
	//intilize map with vector3d objects	
	initMap();
	
	if (algorithmToUse == CIRCLE_ALGORITHM) {
		//number of circles to consider
		numCircles = floor(rows/3) + floor(cols/3) - 10;

		//radius of circle size
		circleSize = rows/5;

		//generate circle centers for where we will apply the circle algorithm
		initCircleCenters();
	
		//apply the circles algorithm on the map!
		heightTransformCircleAlg();
	}
	else {
		//use fault algorithm for height generation instead
		heightTransformFaultAlg();
	}
}

void Heightmap::initCircleCenters() {

	srand(time(NULL));
	//first we will generate the indexes of the circle centers randomly
	//given our array size, and the number of circles we need
	int n = 1;
	int r;
	while (n <= numCircles) {
		r = rand() % (numRows * numCols);

		if (std::find(circleCentersIndex.begin(), circleCentersIndex.end(), r) != circleCentersIndex.end()) 
			continue; //center already exists in our list

		else { 
			circleCentersIndex.push_back(r);
			n++;	
		}
	}
}

void Heightmap::initMap() {

	//now initialize each vector3d;
    for (int i = 0; i < numRows; i++) {

    	std::vector<Vector3d> inner;
    	for (int j = 0; j < numCols; j++) {
    		Vector3d v = Vector3d(i,0,j);
    		inner.push_back(v);
    	}
    	map.push_back(inner);
    }


}

//source for algorithm http://www.lighthouse3d.com/opengl/terrain/index.php?circles
void Heightmap::heightTransformCircleAlg() {
	
	//random seed to generate heights randomly
	srand(time(NULL));


	//init current max height to 0;
	maxHeightInMap = 0;



	//for each of our circles centers, apply the circles algorithm for each xz point
	for (int c = 0; c < numCircles; c++) {

		//index of circle center converted to a 2d mapping
		int ci = circleCentersIndex.at(c) / numRows;
		int cj = circleCentersIndex.at(c) % numCols;
		Vector3d circleCenter = map.at(ci).at(cj) ;


		//generate rand height from 5 - 19
		heightDisplacement = 5 + (rand() % 15 );

		for (int i = 0 ; i < numRows; i++) {
			for (int j = 0 ; j < numCols; j++) {

				Vector3d v = map.at(i).at(j);

				float pointDistance = 2 * v.calcDistance(circleCenter) / circleSize;

				if (fabs(pointDistance) <= 1.0) {
					float newY  = heightDisplacement/2.0 + (cos(pointDistance*PI)*heightDisplacement/2.0);
					newY += v.getY();
					v.setY(newY);

					if (newY > maxHeightInMap) {
						maxHeightInMap = newY;
					}
				}

				map.at(i).at(j) = v;
			}
		}
	}

}

//source for algorithm http://www.lighthouse3d.com/opengl/terrain/index.php?fault
void Heightmap::heightTransformFaultAlg() {

	//random seed to generate heights randomly
	srand(time(NULL));



	//generate random float intervals (source : http://stackoverflow.com/questions/686353)
	float initDisplacement =  0.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2.5));
	float finalDisplacement = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/initDisplacement));
	float currDisplacement;

	maxHeightInMap = initDisplacement;

	//points for making up the randon line
	float p1x,p1z,p2x,p2z;


	int numberOfIterations = numCols*(1.3) ;
	float n = numberOfIterations/2.0;

	for (int i = 0; i < numberOfIterations; i++) {

		p1x = rand() % numRows;
		p2x = rand() % numRows;
		p1z = rand() % numRows;
		p2z = rand() % numRows;


		if (i <  n) 
			currDisplacement = initDisplacement  + (i/n)*(finalDisplacement - initDisplacement);
		else 
			currDisplacement = finalDisplacement;

		for (int x = 0; x < numRows; x++) {
			for (int z = 0; z < numCols; z++) {
				Vector3d v = map.at(x).at(z);

				float s = ((p2x - p1x)*(v.getZ() - p1z) - (p2z - p1z)*(v.getX() - p1x));
				
				if (s > 0) {
					v.setY(v.getY() + currDisplacement);
				}
				else {
					if (v.getY() - currDisplacement >= 0)
						v.setY(v.getY() - currDisplacement);
				}
				
				map.at(x).at(z) = v;

				//update max height
				if (v.getY() > maxHeightInMap)
					maxHeightInMap = v.getY();
			}
		}
	}
}	


std::vector< std::vector<Vector3d> > Heightmap::getMap() {
	return map;
}

float Heightmap::getMaxHeight() {
	return maxHeightInMap;
}