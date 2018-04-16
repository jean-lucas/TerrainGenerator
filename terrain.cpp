#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>
#include "terrain.h"
#include "Vector3d.cpp"
#include "Heightmap.cpp"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif




static float rotY = 0;
static float rotX = 0;

//solid (0), wireframe (1), solid+wireframe(2)
static int viewMode = 0;

void makeTerrain();
void makeTerrain2d();






// if lightstatus is on, we setup the lights
// two lights are created on opposite sides of the map
void setupLights() {
	
	lightOne = { 
		{mapSize, mapSize/2, 0},  //position
		{0, 0.5, 0.5, 1},		  //ambient
	 	{0.9, 0.9, 0.9, 1},	      //diffuse
	 	{0, 0.5, 0.5, 1}		  //specular
	};

	lightTwo = {
		{0, mapSize/2, mapSize},
		{0.2, 0, 0.2, 1},
	 	{0.5, 0.5, 0, 1},
	 	{0, 0.5, 0, 1}	
	};
	

    glLightfv(GL_LIGHT0, GL_POSITION, lightOne.pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightOne.diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightOne.amb);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightOne.spec);

    glLightfv(GL_LIGHT1, GL_POSITION, lightTwo.pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightTwo.diff);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightTwo.amb);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightTwo.spec);
}

void toggleLight() {

	if (lightStatus) {
		glEnable(GL_LIGHTING);
    	glEnable(GL_LIGHT0);
    	glEnable(GL_LIGHT1);
    	setupLights();
	}

	else {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}
}



void turnOnMaterial() {

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  yellowRubber.amb); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  yellowRubber.diff); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  yellowRubber.spec); 
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  yellowRubber.shine);
}



void keyboard(unsigned char c, int x, int y) {

	switch (c) {
		case 'q':
		case 'Q':
		case 27:
			exit(0);
			break;

		case 'm':
		case 'M':
			turnOnMaterial();
			break;

		case 'L':
		case 'l':
			lightStatus = !lightStatus;
			toggleLight();
			break;


		//light movement
		case 'c':
		case 'C':
			if (glutGetModifiers() == GLUT_ACTIVE_ALT) 
				lightTwo.pos[0] += 8;
			else 
				lightOne.pos[0] += 8;
			break;

		case 'v':
		case 'V':
			if (glutGetModifiers() == GLUT_ACTIVE_ALT) 
				lightTwo.pos[0] -= 8;
			else 
				lightOne.pos[0] -= 8;
			break;

		case 'd':
		case 'D':
			if (glutGetModifiers() == GLUT_ACTIVE_ALT) 
				lightTwo.pos[1] += 8;
			else 
				lightOne.pos[1] += 8;
			break;

		case 'f':
			if (glutGetModifiers() == GLUT_ACTIVE_ALT) {
				printf("called\n");
				lightTwo.pos[1] -= 8;
			}
			else {
				lightOne.pos[1] -= 8;
			}
			break;


		//toggle wireframe mode
		case 'W':
		case 'w':
			viewMode = (viewMode +1 ) % 3;
			if (viewMode == 1 )
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else if (viewMode == 0) 
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else { }

			
			break;

		case 'T':
		case 't':
			if (primitiveDrawType == 't')
				primitiveDrawType = 'q';
			else
				primitiveDrawType = 't';
			break;




		case '-': // zoom out
		case '_':
			camPos[1] += 300 * 0.05;
			camPos[2] += (300 * 1.5) * 0.05;
			break;

		case '=': // zoom in
		case '+':
			camPos[1] -= 300 * 0.05;
			camPos[2] -= (300 * 1.5) * 0.05;
			break;

		case 'R':
		case 'r':
			heightmapObj = Heightmap(mapSize, mapSize, algorithm);
			map = heightmapObj.getMap();
			makeTerrain2d();
			break;

	}
}



void keys(int key, int x, int y) {
	
	//for rotating the terrain map
	switch(key) {
		
		case GLUT_KEY_LEFT:
			rotY += 10;
			break;

		case GLUT_KEY_RIGHT:
			rotY -= 10;
			break;

		case GLUT_KEY_UP:
			if (rotX <= 130)
				rotX+= 10;
			break;

		case GLUT_KEY_DOWN:
			if (rotX >= -10)
				rotX-= 10;
			break;
	}
}




// Colour your terrain according to topographic maps. The lowest parts should be shades of green,
// and higher parts should become progressively more orange/red. The highest points (mountains)
// should be gray.
void setColour(float height) {

	float r,g,b;
	g = 1 -height;
	r = height;
	b = (b+r)/2.0;


	glColor3f(r,g,b);
}




//create a 2d representation of the terrain, from a top-down perspective
void makeTerrain2d() {

	//the pixel  size of each square will be depended on
	//the size of the panel in order to scale appropriately on screen
	int pixelSize = MAX_TERRAIN_SIZE/mapSize;


	int x,z;
	float color;
	for (x = 0; x < mapSize-1; x++) {
		for (z = 0; z < mapSize-1; z++) {

			Vector3d v = map.at(x).at(z);

			color = v.getY() / heightmapObj.getMaxHeight();
			glColor3f(0,color,color/2);
			

			glBegin(GL_QUADS);

				glVertex3f(map.at(x).at(z).getX(),map.at(x).at(z).getZ(),0);
				glVertex3f(map.at(x).at(z+1).getX(),map.at(x).at(z+1).getZ(),0);
				glVertex3f(map.at(x+1).at(z+1).getX(),map.at(x+1).at(z+1).getZ(),0);
				glVertex3f(map.at(x+1).at(z).getX(),map.at(x+1).at(z).getZ(),0);

			glEnd();

		}
	}
}







void makeTerrain() {

	float x,z,color;

	//we require a different draw mode toggle
	//quad strips and triangle strips

	for (x = 0; x < mapSize-1; x++) {
		for (z = 0; z < mapSize-1; z++) {

			setColour(map.at(x).at(z).getY() / heightmapObj.getMaxHeight());

			//calc normal betweeen two verticies of a face
			Vector3d v1 = map.at(x).at(z).subtract(map.at(x).at(z+1));
			Vector3d v2 = map.at(x).at(z).subtract(map.at(x+1).at(z));	
			Vector3d normal = v1.calcNormal(v1,v2);
			normal.normalizeVector();


			if (primitiveDrawType == 'q') {
				glBegin(GL_QUADS);

				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(map.at(x).at(z).getX(),map.at(x).at(z).getY(),map.at(x).at(z).getZ());

				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(map.at(x).at(z+1).getX(),map.at(x).at(z+1).getY(),map.at(x).at(z+1).getZ());

				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(map.at(x+1).at(z+1).getX(),map.at(x+1).at(z+1).getY(),map.at(x+1).at(z+1).getZ());

				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(map.at(x+1).at(z).getX(),map.at(x+1).at(z).getY(),map.at(x+1).at(z).getZ());			

				glEnd();
			}

			else {

				glBegin(GL_TRIANGLES);

				
				//FIRST TRIANGLE IN A SQUAURE
				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(map.at(x).at(z).getX(),map.at(x).at(z).getY(),map.at(x).at(z).getZ());

				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(map.at(x).at(z+1).getX(),map.at(x).at(z+1).getY(),map.at(x).at(z+1).getZ());

				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(map.at(x+1).at(z).getX(),map.at(x+1).at(z).getY(),map.at(x+1).at(z).getZ());			

				//SECOND TRIANGLE IN A SQUARE
				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(map.at(x+1).at(z).getX(),map.at(x+1).at(z).getY(),map.at(x+1).at(z).getZ());

				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(map.at(x).at(z+1).getX(),map.at(x).at(z+1).getY(),map.at(x).at(z+1).getZ());

				glNormal3f(normal.getX(), normal.getY(), normal.getZ());
				glVertex3f(map.at(x+1).at(z+1).getX(),map.at(x+1).at(z+1).getY(),map.at(x+1).at(z+1).getZ());
			
				glEnd();	
			}
		}
	}
}






//init method for the main window
void init() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 1, 900); 
	
	camPos[0] = mapSize;		
	camPos[1] = mapSize;
	camPos[2] = mapSize;

	glEnable(GL_DEPTH_TEST);

	// glFrontFace(GL_CCW); 
	// glCullFace(GL_BACK); 
	// glEnable(GL_CULL_FACE);

	toggleLight();
}


//display method for the main window
void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightOne.pos);
	glLightfv(GL_LIGHT1, GL_POSITION, lightTwo.pos);

	glRotatef(rotY, 0, 1, 0);
	glRotatef(rotX, 1, 0 ,0);
	glTranslatef(-mapSize/2,0,-mapSize/2);
	makeTerrain();

	glutSwapBuffers();
}



//init method for the second window
void initWindow2() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, mapSize, 0, mapSize ); 


}

//display method for the seconf window
void displayWindow2() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	makeTerrain2d();	


	glutSwapBuffers();
}




void idle() {
	//update both windows
	glutSetWindow(winOneId);
	glutPostRedisplay(); 

	glutSetWindow(winTwoId);
	glutPostRedisplay(); 
}





void callbackListeners() {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keys);
	glutIdleFunc(idle);
}

void callbackListenersWindow2() {
	glutDisplayFunc(displayWindow2);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keys);
	glutIdleFunc(idle);

}

void getUserInput() {

	printf("Enter size of Terrain from 50 - 300: \n");
	scanf("%d", &mapSize);

	printf("Which algorithm to use for height generation:\n\n \
	0 : Circle algorithm (best for large terrains 100 and up)\n \
	1 : Fault algorithm  (best for small terrains 100 and down) \n");
	scanf("%d", &algorithm);

	if (mapSize < MIN_TERRAIN_SIZE || mapSize > MAX_TERRAIN_SIZE) {
		printf("Invalid terrain size given. Default size of %d set.\n", DEFAULT_TERRAIN_SIZE );
		mapSize = DEFAULT_TERRAIN_SIZE;
	}

	if (algorithm != 0 && algorithm != 1) {
		printf("Invalid algorithm mode given, using default circle algorithm\n");
		algorithm = 0;
	}

	//initiate heightmap and retrieve it
	heightmapObj = Heightmap(mapSize, mapSize,algorithm);
	map = heightmapObj.getMap();

	//print instructions
	printf("%s\n", instructions);
}






	
int main (int argc, char** argv ) {
	
	glutInit(&argc, argv);

	getUserInput();

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);


    //create second window
    glutInitWindowSize(DEFAULT_WINDOW2_WIDTH,DEFAULT_WINDOW2_HEIGHT);
    glutInitWindowPosition(100 + DEFAULT_WINDOW_WIDTH, 100);
    winTwoId = glutCreateWindow("2D Terrain");
    callbackListenersWindow2();
    initWindow2();
    
    //create first window
    glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    glutInitWindowPosition(100,100);
    winOneId = glutCreateWindow("Terrain"); 
    callbackListeners();
    init();


    glutMainLoop();

	return 0;
}