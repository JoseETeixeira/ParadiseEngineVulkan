#pragma once

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



struct Camera
{
	Mat44 projectionTransform;
	glm::mat4 projection;


	static Mat44 MakeProjectionTransform(float fov, float nearClip, float farClip, unsigned int viewWidth, unsigned int viewHeight)
	{
		float zClipBias0 =
			(farClip + nearClip)
			/ (farClip - nearClip);

		float zClipBias1 =
			(2.0f * farClip * nearClip)
			/ (farClip - nearClip);

		float zoomX = 1.0f / tanf((fov / 2.0f) * (M_PI / 180.0f));
		float zoomY = (zoomX * static_cast<float>(viewWidth)) / static_cast<float>(viewHeight);

		Mat44 transform;
		transform.m[0][0] = zoomX;
		transform.m[1][1] = zoomY;
		transform.m[2][2] = -zClipBias0;
		transform.m[3][2] = -1;
		transform.m[2][3] = -zClipBias1;

		return transform;
	}

	static glm::mat4 makeprojection(float fov, float nearClip, float farClip, unsigned int viewWidth, unsigned int viewHeight, float aspect_ratio){
		glm::mat4 ret = glm::perspective(glm::radians(fov), aspect_ratio, nearClip, farClip);
		return ret;
	}
};
