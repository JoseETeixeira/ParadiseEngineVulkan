#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "../math/Mat44.hpp"
#include <cmath>

#if defined(_WIN32)
	#include <corecrt_math_defines.h>
#endif


#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Camera {
public:
	//Construction/Destruction
	Camera();
	//Nothing to clean up

	//View settings
	void setCameraPos(const glm::vec3& v);
	void setCameraFocus(const glm::vec3& v);
	void setCameraUpVec(const glm::vec3& v);

	//Projection settings
	void setAspectRatio(const float ar);
	void setViewAngle(const float a);
	void setNearClipDist(const float d);
	void setFarClipDist(const float d);

	//Get matrix data
	glm::mat4 getViewMat();
	float* getViewMatRef();
	glm::mat4 getProjMat();
	float* getProjMatRef();

	//Create matrices
	void genViewMat();
	void genProjMat();

	//View matrix variables
	glm::vec3 pos; ///< position of the camera
	glm::vec3 fp; ///< focal point of the camera
	glm::vec3 up; ///< the up direction for the camera

	//Projection matrix variables
	float ar; ///< aspect ratio
	float angle; ///< angle in radians
	float near; ///< near clipping distance
	float far; ///< far clipping distance

	//functional matrices
	glm::mat4 view; ///< View matrix for OpenGL
	glm::mat4 proj; ///< Projection matrix for OpenGL
};


#endif