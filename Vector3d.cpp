#include "Vector3d.h"
#include <math.h>
#include <stdio.h>


Vector3d::Vector3d(){	
	x = 1.0f;
	y = 1.0f;
	z = 1.0f;
}

Vector3d::Vector3d(float inX, float inY, float inZ){	
	x = inX;
	y = inY;
	z = inZ;
}


void Vector3d::setX(float inX){
	x = inX;
}

void Vector3d::setY(float inY){
	y = inY;
}

void Vector3d::setZ(float inZ){
	z = inZ;
}

float Vector3d::getX(){
	return x;
}

float Vector3d::getY(){
	return y;
}

float Vector3d::getZ(){
	return z;
}


float Vector3d::calcMagnitude() {
	float m;
	m = sqrt(pow(x,2) + pow(y,2) + pow(z,2));
	return m;
}

Vector3d Vector3d::calcNormal(Vector3d v, Vector3d w) {
	
	float vx,vy,vz;
	Vector3d normalVector;

	vx = v.getY() * w.getZ() - v.getZ() * w.getY();
	vy = v.getZ() * w.getX() - v.getX() * w.getZ();
	vz = v.getX() * w.getY() - v.getY() * w.getX();

	normalVector = Vector3d(vx, vy, vz);

	return normalVector;

}

Vector3d Vector3d::subtract(Vector3d v) {
	float vx = v.getX() - x;
	float vy = v.getY() - y;
	float vz = v.getZ() - z;

	Vector3d newVec = Vector3d(vx,vy,vz);

	return newVec;

}

float Vector3d::calcDistance(Vector3d v) {
	float d;

	d = sqrt( pow(v.getX() - x, 2) + pow(v.getY() - y, 2) + pow( v.getZ() - z, 2));
	return d;

}


void Vector3d::normalizeVector() {
	x = x/calcMagnitude();
	y = y/calcMagnitude();
	z = z/calcMagnitude();
}
