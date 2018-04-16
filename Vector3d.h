#ifndef VECTOR3D.H
#define VECTOR3D.H


//ideally this class is used to represt Vertex points in 3d
//to create a vector we can just call the subtract method on another object of this class.
class Vector3d {

	float x,y,z; 
	

	public:
		Vector3d(); // create a unit vector
		
		Vector3d(float inX, float inY, float inZ);

		//find the normal vector between two non-parallel vectors
		Vector3d calcNormal(Vector3d v, Vector3d w);



		Vector3d subtract(Vector3d v);


	
		void setX(float inX);
		void setY(float inY);
		void setZ(float inZ);

		float getX();
		float getY();
		float getZ();


		float calcMagnitude();
		float calcDistance(Vector3d v);

		void normalizeVector();
					
};



#endif