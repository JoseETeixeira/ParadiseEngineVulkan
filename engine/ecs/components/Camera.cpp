#include "Camera.h"
#include "Transform.hpp"
#include "../Coordinator.hpp"

extern Coordinator gCoordinator;


void Camera::setCameraPos(const glm::vec3& v)
{
	pos = v;
}


void Camera::setCameraRotation(const glm::vec3& v)
{
	rotation = v;
}

void Camera::setCameraFocus(const glm::vec3& v)
{
	fp = v;
}

void Camera::setCameraUpVec(const glm::vec3& v)
{
	up = v;
}

void Camera::setAspectRatio(const float ar)
{
	this->ar = ar;
}

void Camera::setViewAngle(const float a)
{
	angle = a;
}

void Camera::setNearClipDist(const float d)
{
	near = d;
}

void Camera::setFarClipDist(const float d)
{
	far = d;
}

glm::mat4 Camera::getViewMat()
{
	return view;
}

float* Camera::getViewMatRef()
{
	return glm::value_ptr(view);
}

glm::mat4 Camera::getProjMat()
{
	return proj;
}

float* Camera::getProjMatRef()
{
	return glm::value_ptr(proj);
}

void Camera::genViewMat()
{
	view = glm::lookAt(pos, fp, up);
	view = glm::rotate(view,rotation.x,glm::vec3(1.0f,0.0f,0.0f));
	view = glm::rotate(view,rotation.y,glm::vec3(0.0f,1.0f,0.0f));
	view = glm::rotate(view,rotation.z,glm::vec3(0.0f,0.0f,1.0f));
	
}

void Camera::genProjMat()
{
	proj = glm::perspective(angle, ar, near, far);
	proj[1][1] *= -1.0;
}