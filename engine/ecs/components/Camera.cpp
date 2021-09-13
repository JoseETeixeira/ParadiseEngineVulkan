#include "Camera.h"
#include "Transform.hpp"
#include "../Coordinator.hpp"

extern Coordinator gCoordinator;

#define GLM_FORCE_RADIANS
Camera::Camera()
{
	ar = 4.0f / 3.0f;
	#ifdef GLM_FORCE_RADIANS
		angle = 0.785398f; //45 degrees
	#else
		angle = 45.0f; //45 degrees
	#endif
	near = 0.1f;
	far = 100.0f;
}

void Camera::setCameraPos(const glm::vec3& v)
{
	pos = v;
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
}

void Camera::genProjMat()
{
	proj = glm::perspective(angle, ar, near, far);
}