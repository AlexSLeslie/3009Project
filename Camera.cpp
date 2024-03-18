# include "Camera.h"

Camera::Camera(void) :
	position(0.0f, 0.0f, 0.0f),
	lookAtVector(0.0f, 0.0f, 100.0f),
	upVector(0.0f, 1.0f, 0.0f),
	speed(0),
	nearPlane(0.1),
	farPlane(1000),
	fieldOfView(60),
	aspectRatio(1.0f) {

	viewMat = Matrix4f::cameraMatrix(position, lookAtVector, upVector);
	projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
}

Camera::~Camera(void) {}

int Camera::roll(float angle) {
	Vector4f u = Matrix4f::rotateVector(-lookAtVector, angle, 1) * upVector;
	upVector = u.to3D();
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

	return 0;
}

int Camera::pitch(float angle){

	// get rotation axis
	Vector3f rotVector = Vector3f::cross(lookAtVector, upVector);	// DN 26/10/2015

	// create rotation matrix
	upVector = (Matrix4f::rotateVector(rotVector, angle, 1) * upVector).to3D();
	lookAtVector = (Matrix4f::rotateVector(rotVector, angle, 1) * lookAtVector).to3D();

	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

	return 0;
}

int Camera::yaw(float angle){

	lookAtVector = (Matrix4f::rotateVector(upVector, angle, 1) * lookAtVector).to3D();

	return 0;
}

Vector3f Camera::getPosition(void) {
	Vector3f v = position;	
	return (v);
}

Vector3f Camera::getLookAtPoint(void) { return (position + lookAtVector); }

Vector3f Camera::getUpVector(void) { return upVector; }

void Camera::updateViewMatrix(void){ viewMat = Matrix4f::cameraMatrix(position, lookAtVector, upVector); }

int Camera::changePositionDelta(float dx, float dy, float dz) {

	position.x += dx;
	position.y += dy;
	position.z += dz;

	//viewMat = Matrix4f::cameraMatrix(position, lookAtVector, upVector);
	updateViewMatrix();

	return 0;
}

int Camera::changePositionDelta(Vector3f* dv) {
	position.x += dv->x;
	position.y += dv->y;
	position.z += dv->z;

	updateViewMatrix();

	return 0;
}

int Camera::changePositionDelta(Vector3f dv) {
	position.x += dv.x;
	position.y += dv.y;
	position.z += dv.z;

	updateViewMatrix();

	return 0;
}

int Camera::changeAbsPosition(float x, float y, float z) {
	
	position.x = x;
	position.y = y;
	position.z = z;

	viewMat = Matrix4f::cameraMatrix(position, lookAtVector, upVector);

	return 0;
}

int Camera::changeAbsPosition(Vector3f* v) {

	// update the view matrix
	position.x = v->x;
	position.y = v->y;
	position.z = v->z;

	viewMat = Matrix4f::cameraMatrix(position, lookAtVector, upVector);

	return 0;
}

Vector3f Camera::moveForward(float numUnits) {
	
	position += lookAtVector * numUnits;
	updateViewMatrix();

	return (position);	
}

Vector3f Camera::moveBackward(float numUnits)
{
	position -= lookAtVector * numUnits;
	updateViewMatrix();

	return(position);
}

int Camera::updateOrientation(Vector3f rotVector, float angleRad)
{

	Vector3f xaxis(0.0, 0.0, 0.0);
	Vector4f u;

	// create rotation matrix
	rotMat = Matrix4f::rotateVector(rotVector, angleRad, 0);
	// rotate the camera (up vector and/or looAtVector)
	u = rotMat * upVector;
	upVector = u.to3D();
	u = rotMat * lookAtVector;
	lookAtVector = u.to3D();	// DN 26/10/2015

	// update the upVector
	xaxis = Vector3f::cross(upVector, lookAtVector);
	upVector = Vector3f::cross(lookAtVector, xaxis);
	upVector.normalize();
	lookAtVector.normalize();
	return 0;
}

Matrix4f Camera::getViewMatrix(Matrix4f* viewMatrix)
{
	Vector3f lookAtPoint;
	Matrix4f m;

	lookAtPoint = position + lookAtVector;
	m = Matrix4f::cameraMatrix(position, lookAtPoint, upVector);
	if (viewMatrix != NULL) *viewMatrix = m;

	return(m);
}

void Camera::setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector)
{
	this->position = position;
	this->upVector = upVector;
	lookAtVector = lookAtPoint - position;
	lookAtVector.normalize();
	upVector.normalize();

	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);
}

int Camera::updateSpeed(float speed)
{
	this->speed += speed;
	return 0;
}

float Camera::getSpeed(void) { return speed; }

int Camera::changeLookAtVector(float x, float y, float z)
{
	lookAtVector.x = x;
	lookAtVector.y = y;
	lookAtVector.z = z;
	// update the view matrix
	viewMat = Matrix4f::cameraMatrix(position, position + lookAtVector, upVector);

	return 0;
}

Vector3f Camera::moveRight(float numUnits)
{
	// compute the the moving direction
	Vector3f moveVector = Vector3f::cross(upVector, lookAtVector);
	moveVector.normalize();

	changePositionDelta(moveVector * numUnits);


	// move vector is a cross product 
	// moveVector = Vector3f::cross(upVector, lookAtVector);


	// update the position along the vector

	// update the view matrix

	return (position);	// DN 26/10/2015
}

Vector3f Camera::moveLeft(float numUnits)
{
	Vector3f moveVector = Vector3f::cross(upVector, lookAtVector);
	moveVector.normalize();

	changePositionDelta(-(moveVector * numUnits));

	return(position);
}

int Camera::zoomIn(void)
{
	zoomIn(1);
	return 0;
}

int Camera::zoomIn(float zoom)
{
	// update the size of the field of view


	// make sure that the field of view is not too small

	// update the projection matrix
	projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
	return 0;
}

int Camera::zoomOut(void)
{
	zoomOut(1);
	return 0;
}

int Camera::zoomOut(float zoom)
{
	// update the size of the field of view


	// make sure that the field of view is not too large

	// update the projection matrix
	projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
	return 0;
}

int Camera::setPerspectiveView(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);

	return 0;
}

Matrix4f Camera::getProjectionMatrix(Matrix4f* projMatrix)
{
	Matrix4f m;

	m = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
	if (projMatrix != NULL) *projMatrix = m;

	return(m);
}
