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
	nearPlane = d;
}

void Camera::setFarClipDist(const float d)
{
	farPlane = d;
}

glm::mat4 Camera::getViewMat()
{
	return view;
}

float* Camera::getViewMatRef()
{
	return glm::value_ptr(view);
}

float* Camera::getInverseViewMatRef()
{
	glm::mat4 inverse = glm::inverse(view);
	return glm::value_ptr(inverse);
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

		glm::mat4 rotM = glm::mat4(1.0f);
		glm::mat4 transM;

		rotM = glm::rotate(rotM, glm::radians(rotation.x ), glm::vec3(1.0f, 0.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		rotM = glm::rotate(rotM, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 translation = pos;


		transM = glm::translate(glm::mat4(1.0f), translation);

		if (type == CameraType::firstperson)
		{
			view = rotM * transM;
		}
		else
		{
			view = transM * rotM;
		}

	
}

void Camera::genProjMat()
{
	proj = glm::perspective(angle, ar, nearPlane, farPlane);
	proj[1][1] *= -1.0f;
	


}

void Camera::rotate(glm::vec3 delta)
	{
		this->rotation += delta;
		genViewMat();
	}


void Camera::translate(glm::vec3 delta)
	{
		this->pos += delta;
		genViewMat();
	}