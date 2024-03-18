#pragma once
#include "includes.h"

class Camera {
public:
	Camera(void);
	~Camera(void);

	int roll(float angle);
	int pitch(float angle);
	int yaw(float angle);

	Vector3f getPosition(void);
	Vector3f getLookAtPoint(void);
	Vector3f getUpVector(void);
	float getSpeed(void);

	int changePositionDelta(float dx, float dy, float dz);
	int changePositionDelta(Vector3f* dv);
	int changePositionDelta(Vector3f dv);
	int changeAbsPosition(float x, float y, float z);
	int changeAbsPosition(Vector3f* v);
	int changeAbsPosition(Vector3f v);
	void setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector);

	Vector3f moveForward(float numUnits);	
	Vector3f moveBackward(float numUnits);  
	Vector3f moveRight(float numUnits);
	Vector3f moveLeft(float numUnits);

	int updateSpeed(float speed);
	int changeLookAtVector(float x, float y, float z);
	int zoomIn(void);
	int zoomIn(float zoom);
	int zoomOut(void);
	int zoomOut(float zoom);
	Matrix4f getViewMatrix(Matrix4f* viewMatrix);
	Matrix4f getProjectionMatrix(Matrix4f* projMatrix);

	int setPerspectiveView(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

private:
	int updateOrientation(Vector3f rotVector, float angleRad);
	void updateViewMatrix(void);

	Matrix4f viewMat;
	Matrix4f projMat;
	Matrix4f rotMat;
	Vector3f position;		
	Vector3f upVector;		
	Vector3f lookAtVector;	

	float speed;
	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
};