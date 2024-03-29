#pragma once

#include <stdio.h>
#define _USE_MATH_DEFINES // for C++. The define is used to load the constants
#include <math.h>
#include "assert.h"
#include "nuss_vector.h"

#define DegreeToRadians(x) (float)(((x) * M_PI / 180.0f))
#define RadianToDegrees(x) (float)(((x) * 180.0f / M_PI))

//float RandomFloat();



/********************************************************************************/



class Matrix4f
{

public:
	float* m[4];			// can be removed
	Vector4f vm[4];

	Matrix4f()
	{
		m[0] = (float*)&vm[0];
		m[1] = (float*)&vm[1];
		m[2] = (float*)&vm[2];
		m[3] = (float*)&vm[3];
		reset(0.0);

	}


	/********************************************************************************/



	Matrix4f(Vector4f v0, Vector4f v1, Vector4f v2, Vector4f v3)
	{
		vm[0] = v0;
		vm[1] = v1;
		vm[2] = v2;
		vm[3] = v3;
		m[0] = (float*)&vm[0];
		m[1] = (float*)&vm[1];
		m[2] = (float*)&vm[2];
		m[3] = (float*)&vm[3];

	}

	/********************************************************************************/
	// returns the data as a on dimension array of float

	float* data()
	{
		return (float*)(&vm);
	}


	/********************************************************************************/

	// static functions

	// reurns an identity matrix 
	static Matrix4f identity()
	{
		Matrix4f m1;
		m1.reset(0);
		m1.vm[0].x = m1.vm[1].y = m1.vm[2].z = m1.vm[3].w = 1;
		return(m1);

	}

	/*----------------------------------------------------------*/

	// creates a transpose of the matrix
	// retruns a transposed matrix
	// note it does not change the internal values of the matrix
	static Matrix4f transpose(Matrix4f m1)
	{
		Matrix4f m2;

		m2.vm[0] = Vector4f(m1.vm[0].x, m1.vm[1].x, m1.vm[2].x, m1.vm[3].x);
		m2.vm[1] = Vector4f(m1.vm[0].y, m1.vm[1].y, m1.vm[2].y, m1.vm[3].y);
		m2.vm[2] = Vector4f(m1.vm[0].z, m1.vm[1].z, m1.vm[2].z, m1.vm[3].z);
		m2.vm[3] = Vector4f(m1.vm[0].w, m1.vm[1].w, m1.vm[2].w, m1.vm[3].w);


		return(m2);

	}


	/*----------------------------------------------------------*/

	// return a scale matrix.  Note internal parameters are not changes
	// Note it resets the data first
	static Matrix4f scale(float scaleX, float scaleY, float scaleZ)
	{
		Matrix4f m1;
		m1 = identity();
		m1.vm[0].x = scaleX;
		m1.vm[1].y = scaleY;
		m1.vm[2].z = scaleZ;
		return m1;
	}


	/*----------------------------------------------------------*/

	// set the matrix as a pitch rotation matrix
	// if degree is true (!=0) then angle is given in degrees
	// otherwise angle is given in radians
	static Matrix4f rotateX(float angle, int degree)
	{
		Matrix4f m1;
		if (degree) angle = DegreeToRadians(angle);
		m1 = identity();
		m1.vm[1].y = m1.vm[2].z = cos(angle);
		m1.vm[1].z = -sin(angle);
		m1.vm[2].y = -m1.vm[1].z;
		return(m1);
	}


	/*----------------------------------------------------------*/

	// set the matrix as a yaw rotation matrix
	// if degree is true (!=0) then angle is given in degrees
	// otherwise angle is given in radians
	static Matrix4f rotateY(float angle, int degree)
	{
		Matrix4f m1;
		if (degree) angle = DegreeToRadians(angle);
		m1 = identity();
		m1.vm[0].x = m1.vm[2].z = cos(angle);
		m1.vm[0].z = sin(angle);
		m1.vm[2].x = -m1.vm[0].z;
		return(m1);
	}


	/*----------------------------------------------------------*/

	// set the matrix as a roll rotation matrix
	// if degree is true (!=0) then angle is given in degrees
	// otherwise angle is given in radians
	static Matrix4f rotateZ(float angle, int degree)
	{
		Matrix4f m1;
		if (degree) angle = DegreeToRadians(angle);
		m1 = identity();
		m1.vm[0].x = m1.vm[1].y = cos(angle);
		m1.vm[0].y = -sin(angle);
		m1.vm[1].x = -m1.vm[0].y;

		return(m1);
	}



	/*----------------------------------------------------------*/

	// set the matrix as a roll pitch and yaw rotation matrix
	// the resulting matrix M=Mat(yaw)*mat(pitch)*mat(roll)
	// if degree is true (!=0) then all angles are given in degrees
	// otherwise angle is given in radians
	static Matrix4f rotateRollPitchYaw(float angleRoll, float anglePitch, float angleYaw, int isDegree)
	{
		Matrix4f m1;
		m1 = identity();
		m1 = rotateZ(angleRoll, isDegree) * rotateX(anglePitch, isDegree) * rotateY(angleYaw, isDegree);

		return(m1);

	}

	/*----------------------------------------------------------*/

	// set the matrix as a roll pitch and yaw rotation matrix
	// the resulting matrix M=Mat(yaw)*mat(pitch)*mat(roll)
	// if degree is true (!=0) then all angles are given in degrees
	// otherwise angle is given in radians
	static Matrix4f rotateXYZ(float angleRoll, float anglePitch, float angleYaw, int isDegree)
	{
		Matrix4f m1;
		m1 = identity();
		m1 = rotateX(anglePitch, isDegree) * rotateY(angleYaw, isDegree) * rotateZ(angleRoll, isDegree);

		return(m1);

	}


	/*----------------------------------------------------------*/

	// set the matrix as a rotation around a vector where only a rotation vector is given
	// if degree is true (!=0) then all angles are given in degrees
	// otherwise angle is given in radians
	static Matrix4f rotateVector(Vector3f u, float angle, int isDegree)
	{
		Matrix4f m1;
		m1 = identity();
		u.normalize();
		if (isDegree) angle = DegreeToRadians(angle);
		float x = u.x;
		float y = u.y;
		float z = u.z;
		float c = cosf(angle);
		float s = sinf(angle);

		m1.vm[0] = Vector4f(c + x * x * (1 - c), x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0);
		m1.vm[1] = Vector4f(x * y * (1 - c) + z * s, c + y * y * (1 - c), y * z * (1 - c) - x * s, 0);
		m1.vm[2] = Vector4f(z * x * (1 - c) - y * s, z * y * (1 - c) + x * s, c + z * z * (1 - c), 0);
		m1.vm[3] = Vector4f(0, 0, 0, 1);

		return(m1);

	}



	/*----------------------------------------------------------*/


	// set the matrix as a rotation around a vector where only a rotation vector is given
	// if degree is true (!=0) then all angles are given in degrees
	// otherwise angle is given in radians
	static Matrix4f rotateVector(Vector4f u, float angle, int degree)
	{
		Matrix4f m1;
		Vector3f v = Vector3f(u.x, u.y, u.z);
		m1 = rotateVector(v, angle, degree);

		return(m1);

	}


	/*----------------------------------------------------------*/

	static Matrix4f translation(float dx, float dy, float dz)
	{
		Matrix4f m1;

		m1 = identity();
		m1.vm[0].w = dx;
		m1.vm[1].w = dy;
		m1.vm[2].w = dz;
		return(m1);
	}



	//--------------------------------------------------

#if 0
	// commented out becuase translation is only 3D
	static Matrix4f translation(Vector4f v)
	{
		Matrix4f m1;

		m1 = identity();
		m1.vm[0].w = v.x;
		m1.vm[1].w = v.y;
		m1.vm[2].w = v.z;
		return(m1);
	}
#endif


	/*----------------------------------------------------------*/


	static Matrix4f translation(Vector3f v)
	{
		Matrix4f m1;

		m1 = identity();
		m1.vm[0].w = v.x;
		m1.vm[1].w = v.y;
		m1.vm[2].w = v.z;
		return(m1);
	}



	/*----------------------------------------------------------*/


	//static Matrix4f cameraMatrix(Vector3f position, Vector3f lookAtVector, Vector3f upVector)
	static Matrix4f cameraMatrix(Vector3f position, Vector3f lookAtPoint, Vector3f upVector)
	{
		Matrix4f m1;
		Vector3f u, v, n;

		m1 = identity();
		n = position - lookAtPoint;
		n.normalize();
		upVector.normalize();

		u = Vector3f::cross(upVector, n);
		u.normalize();
		v = Vector3f::cross(n, u);

		m1.vm[0] = Vector4f(u, 0);
		m1.vm[1] = Vector4f(v, 0);
		m1.vm[2] = Vector4f(n, 0);
		m1.vm[3] = Vector4f(0, 0, 0, 1);
		m1.vm[0].w = Vector3f::dot(-u, position);
		m1.vm[1].w = Vector3f::dot(-v, position);
		m1.vm[2].w = Vector3f::dot(-n, position);
#if 0
		m1.vm[0].w = position.x; // Vector3f::dot(-u, position);
		m1.vm[1].w = position.y; // Vector3f::dot(-v, position);
		m1.vm[2].w = position.z; // Vector3f::dot(-n, position);
#endif

		return(m1);
	}


	/*----------------------------------------------------------*/


	//static Matrix4f cameraMatrix(Vector3f position, Vector3f lookAtVector, Vector3f upVector)
	static Matrix4f cameraMatrix(Vector4f position, Vector4f lookAtPoint, Vector4f upVector)
	{
		Matrix4f m1;
		Vector4f u, v, n;

		m1 = identity();
		n = position - lookAtPoint;
		n.w = 0;
		n.normalize();
		upVector.w = 0;
		upVector.normalize();

		u = Vector4f(Vector4f::cross(upVector, n), 0);
		u.normalize();
		v = Vector4f(Vector4f::cross(n, u), 0.0);

		m1.vm[0] = u;
		m1.vm[1] = v;
		m1.vm[2] = n;
		m1.vm[3] = Vector4f(0, 0, 0, 1);
		m1.vm[0].w = Vector4f::dot3(-u, position);
		m1.vm[1].w = Vector4f::dot3(-v, position);
		m1.vm[2].w = Vector4f::dot3(-n, position);

		return(m1);
	}


	/*----------------------------------------------------------*/

	static Matrix4f symmetricPerspectiveProjectionMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
	{
		Matrix4f m1;
		m1 = identity();
		float cot = 1 / tan(DegreeToRadians(fieldOfView / 2.0));
		m1.vm[0].x = cot / aspectRatio;
		m1.vm[1].y = cot;
		m1.vm[2].z = (float)(nearPlane + farPlane) / (nearPlane - farPlane);
		m1.vm[2].w = (float)2.0 * nearPlane * farPlane / (nearPlane - farPlane);
		m1.vm[3].z = -1;
		m1.vm[3].w = 0;

		return(m1);
	}


	/*----------------------------------------------------------*/


	static Matrix4f frustumProjectionMatrix(float winMinX, float winMinY, float winMaxX, float winMaxY, float nearPlane, float farPlane)
	{
		Matrix4f m1;
		m1 = identity();

		m1.vm[0].x = 2 * nearPlane / (winMaxX - winMinX);		// check why it is not "-2"
		m1.vm[0].z = (winMaxX + winMinX) / (winMaxX - winMinX);
		m1.vm[1].y = 2 * nearPlane / (winMaxY - winMinY);		// check why it is not "-2"
		m1.vm[1].z = (winMaxY + winMinY) / (winMaxY - winMinY);
		m1.vm[2].z = (nearPlane + farPlane) / (nearPlane - farPlane);
		m1.vm[2].w = (2 * nearPlane * farPlane) / (nearPlane - farPlane);
		m1.vm[3].z = -1;
		m1.vm[3].w = 0;
		// ADD CODE

		return(m1);
	}


	/*----------------------------------------------------------*/

	// print the matrix (prints the vectors)
	friend std::ostream& operator << (std::ostream& os, const Matrix4f& m1)

	{
		int i;
		for (i = 0; i <= 3; i++) {
			os << m1.vm[i] << std::endl;
		}
		return(os);
	}

	/********************************************************************************/



	// prints the matrix 
	void printMatrix()
	{

		for (int i = 0; i < 4; i++) {
			printf("row[%d]=", i);
			printf("(%f, %f , %f,  %f)\n", vm[i].x, vm[i].y, vm[i].z, vm[i].w);

		}
		return;

	}

	/********************************************************************************/



	// prints the matrix 
	void printMatrix(char* msg)
	{
		printf("%s\n", msg);
		for (int i = 0; i < 4; i++) {
			printf("row[%d]=", i);
			printf("(%f, %f , %f,  %f)\n", vm[i].x, vm[i].y, vm[i].z, vm[i].w);

		}
		return;

	}


	/***********************************************************************/

	// resets all the values of the matrix to "value"
	inline void reset(float value)
		// sets all 16 values of the matrix to value 
	{
		int i;
		for (i = 0; i <= 3; i++) {
			vm[i] = Vector4f(value, value, value, value);
		}

	}



	/****************************************************************************************/

	// matrix operators 
	inline Matrix4f operator*(const Matrix4f& rhs) const
	{
		Matrix4f m1, m2;
		int i, j;

		m2 = transpose(rhs);
		//m1.m = (float *) m1.vm;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				//m1.m[i * 4 + j] = Vector4f::dot(vm[i], m2.vm[j]);
				m1.m[i][j] = Vector4f::dot(vm[i], m2.vm[j]);
			}
		}

		return(m1);
	}


	/****************************************************************************************/



	inline Matrix4f operator*(const float f) const
	{
		Matrix4f m1;
		int i;

		for (i = 0; i < 4; i++) {
			m1.vm[i] = vm[i] * f;
		}

		return(m1);
	}

	/****************************************************************************************/



	friend Matrix4f operator*(const float f, Matrix4f m1)
	{

		return(m1 * f);
	}

	/****************************************************************************************/


	inline Matrix4f operator+(const Matrix4f& rhs) const
	{
		Matrix4f m1, m2;
		int i;
		for (i = 0; i < 4; i++) {
			m1.vm[i] = vm[i] + rhs.vm[i];
		}

		return(m1);
	}

	/****************************************************************************************/


	inline Matrix4f operator-(const Matrix4f& rhs) const
	{
		Matrix4f m1, m2;
		int i;
		for (i = 0; i < 4; i++) {
			m1.vm[i] = vm[i] - rhs.vm[i];
		}

		return(m1);
	}



	/****************************************************************************************/



	// multiply the matrix by a vector 
	Vector4f operator*(const Vector4f& v) const
	{
		Vector4f r;

		r.x = Vector4f::dot(vm[0], v);
		r.y = Vector4f::dot(vm[1], v);
		r.z = Vector4f::dot(vm[2], v);
		r.w = Vector4f::dot(vm[3], v);
		return r;
	}

	/****************************************************************************************/



	// multiply the matrix by a vector 
	Vector4f operator*(const Vector3f& v) const
	{
		Vector4f u;
		Vector4f r;

		u = Vector4f(v, 1);
		r.x = Vector4f::dot(vm[0], u);
		r.y = Vector4f::dot(vm[1], u);
		r.z = Vector4f::dot(vm[2], u);
		r.w = Vector4f::dot(vm[3], u);
		return r;
	}



};



/* 3D MATRIX Operations*/


/********************************************************************************/



class Matrix3f
{

public:
	float* m;			// can be removed
	Vector3f vm[3];

	Matrix3f()
	{

		reset(0.0);

	}


	/********************************************************************************/



	Matrix3f(Vector3f v0, Vector3f v1, Vector3f v2)
	{
		vm[0] = v0;
		vm[1] = v1;
		vm[2] = v2;
		m = (float*)vm;

	}

	/********************************************************************************/
	// returns the data as a on dimension array of float

	float* data()
	{
		return (float*)(&vm);
	}


	/********************************************************************************/

	// static functions

	// reurns an identity matrix 
	static Matrix3f identity()
	{
		Matrix3f m1;
		m1.reset(0);
		m1.vm[0].x = m1.vm[1].y = m1.vm[2].w = 1;
		return(m1);

	}



	/*----------------------------------------------------------*/

	// creates a transpose of the matrix
	// retruns a transposed matrix
	// note it does not change the internal values of the matrix
	static Matrix3f transpose(Matrix3f m1)
	{
		Matrix3f m2;

		m2.vm[0] = Vector3f(m1.vm[0].x, m1.vm[1].x, m1.vm[2].x);
		m2.vm[1] = Vector3f(m1.vm[0].y, m1.vm[1].y, m1.vm[2].y);
		m2.vm[2] = Vector3f(m1.vm[0].w, m1.vm[1].w, m1.vm[2].w);


		return(m2);

	}


	/*----------------------------------------------------------*/

	// return a scale matrix.  Note internal parameters are not changes
	// Note it resets the data first
	static Matrix3f scale(float scaleX, float scaleY)
	{
		Matrix3f m1;
		m1 = identity();
		m1.vm[0].x = scaleX;
		m1.vm[1].y = scaleY;
		return m1;
	}

	/*----------------------------------------------------------*/

	// return a scale matrix.  Note internal parameters are not changes
	// Note it resets the data first
	static Matrix3f shearX(float shear)
	{
		Matrix3f m1;
		m1 = identity();
		m1.vm[0].y = shear;
		return m1;
	}
	/*----------------------------------------------------------*/

	// return a scale matrix.  Note internal parameters are not changes
	// Note it resets the data first
	static Matrix3f shearY(float shear)
	{
		Matrix3f m1;
		m1 = identity();
		m1.vm[1].x = shear;
		return m1;
	}


	/*----------------------------------------------------------*/

	// set the matrix as a pitch rotation matrix
	// if degree is true (!=0) then angle is given in degrees
	// otherwise angle is given in radians
	static Matrix3f rotate(float angle, int ifDegrees)
	{
		Matrix3f m1;
		if (ifDegrees) angle = DegreeToRadians(angle);
		m1 = identity();
		m1.vm[0].x = m1.vm[1].y = cos(angle);
		m1.vm[0].y = -sin(angle);
		m1.vm[1].x = -m1.vm[0].y;
		return(m1);
	}




	/*----------------------------------------------------------*/

	static Matrix3f translation(float dx, float dy)
	{
		Matrix3f m1;

		m1 = identity();
		m1.vm[0].w = dx;
		m1.vm[1].w = dy;
		return(m1);
	}





	/*----------------------------------------------------------*/


	static Matrix3f translation(Vector2f v)
	{
		Matrix3f m1;

		m1 = identity();
		m1.vm[0].w = v.x;
		m1.vm[1].w = v.y;
		return(m1);
	}



	/*----------------------------------------------------------*/

	// print the matrix (prints the vectors)
	friend std::ostream& operator << (std::ostream& os, const Matrix3f& m1)

	{
		int i;
		for (i = 0; i < 3; i++) {
			os << m1.vm[i] << std::endl;
		}
		return(os);
	}


	/***********************************************************************/

	// resets all the values of the matrix to "value"
	inline void reset(float value)
		// sets all 9 values of the matrix to value 
	{
		int i;
		for (i = 0; i <= 2; i++) {
			vm[i] = Vector3f(value, value, value);
		}

	}



	/****************************************************************************************/

	// matrix operators 
	inline Matrix3f operator*(const Matrix3f& rhs) const
	{
		Matrix3f m1, m2;
		int i, j;

		m2 = transpose(rhs);
		m1.reset(0);
		m1.m = (float*)m1.vm;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				m1.m[i * 3 + j] = Vector3f::dot(vm[i], m2.vm[j]);
			}
		}

		return(m1);
	}


	/****************************************************************************************/



	inline Matrix3f operator*(const float f) const
	{
		Matrix3f m1;
		int i;

		for (i = 0; i < 3; i++) {
			m1.vm[i] = vm[i] * f;
		}

		return(m1);
	}

	/****************************************************************************************/



	friend Matrix3f operator*(const float f, Matrix3f m1)
	{

		return(m1 * f);
	}

	/****************************************************************************************/


	inline Matrix3f operator+(const Matrix3f& rhs) const
	{
		Matrix3f m1;
		int i;
		for (i = 0; i < 3; i++) {
			m1.vm[i] = vm[i] + rhs.vm[i];
		}

		return(m1);
	}

	/****************************************************************************************/


	inline Matrix3f operator-(const Matrix3f& rhs) const
	{
		Matrix3f m1;
		int i;
		for (i = 0; i < 3; i++) {
			m1.vm[i] = vm[i] - rhs.vm[i];
		}

		return(m1);
	}



	/****************************************************************************************/



	// multiply the matrix by a vector 
	Vector3f operator*(const Vector3f& v) const
	{
		Vector3f r;

		r.x = Vector3f::dot(vm[0], v);
		r.y = Vector3f::dot(vm[1], v);
		r.w = Vector3f::dot(vm[2], v);
		return r;
	}




};