#ifndef __TERRAIN.H__
#define __TERRAIN.H__ 


#include <vector>
#include "Vector3d.h"
#include "Heightmap.h"


//which height generation algorithm to use
//0 for circle, 1 for fault
int algorithm;

// helper structs
struct lightStruct {
	float pos[3], amb[4], diff[4], spec[4];
};

struct materialStruct {
	float amb[4], diff[4], spec[4], shine;
};


//window ids
static int winOneId;
static int winTwoId;


//constant vars
const int DEFAULT_WINDOW_HEIGHT = 550;
const int DEFAULT_WINDOW_WIDTH = 750;
const int DEFAULT_WINDOW2_HEIGHT = 400;
const int DEFAULT_WINDOW2_WIDTH = 400;
const int DEFAULT_TERRAIN_SIZE = 150;
const int MIN_TERRAIN_SIZE = 50;
const int MAX_TERRAIN_SIZE = 1000;




//heightmap stuff
static Heightmap heightmapObj = Heightmap(-1, -1, 0);
static std::vector< std::vector<Vector3d> > map;


//camera movement
float yRot = 0;
float xRot = 0;
float camPos[3] = {0,0,0};


//user input
int mapSize;


//lights
static bool lightStatus = false;
static struct lightStruct lightOne;
static struct lightStruct lightTwo;

//material
static struct materialStruct yellowRubber = {
		{0.05, 0.05, 0.0},
		{0.5, 0.5, 0.4},
	 	{0.7, 0.7, 0.04},
	 	0.078125*128
	};

//draw type
//t for triangle strips, or q for quad strips
static char primitiveDrawType = 'q';




const char instructions[] = 
" ======	INSTRUCTIONS    ====== \n\n \
	'Q'  -   Quit program \n  \
	'L'  -	 Toggle Lights on/off \n \
	'M'  -   Enable Material (yellow rubber) \n\n \
	'C'  -   Move Light #1 to the left \n \
	'V'  -   Move Light #1 to the right \n \
	'D'  -   Move Light #1 up \n \
	'F'  - 	 Move Light #1 down \n \
	For light #2 movement use the same keys C,V,D,F but hold the ALT key\n\n \
	'W'  - 	 Alternate between solid shape, or wireframe\n \
	'T'  -   Toggle between QUADS or TRIANGLE primitives (must be in wireframe mode to see the difference)\n\n \
	'-' or '_'  -   Zoom out\n\
	'+' or '='  -   Zoom in\n\n \
	'R'  -   Resets a new random board (slow with large sizes)";
#endif