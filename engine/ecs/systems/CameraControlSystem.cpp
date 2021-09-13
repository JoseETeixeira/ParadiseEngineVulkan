#include "CameraControlSystem.hpp"

#include "../components/Camera.h"
#include "../components/Transform.hpp"
#include "../Coordinator.hpp"


extern Coordinator gCoordinator;


void CameraControlSystem::Init()
{
	gCoordinator.AddEventListener(METHOD_LISTENER(Events::Window::INPUT, CameraControlSystem::InputListener));
}

void CameraControlSystem::Update(float dt)
{
	for (auto& entity : mEntities)
	{
		auto& cam = gCoordinator.GetComponent<Camera>(entity);
		auto& transform = gCoordinator.GetComponent<Transform>(entity);

		glm::vec3 camFront;
		camFront.x = -cos(glm::radians(transform.rotation.x)) * sin(glm::radians(transform.rotation.y));
		camFront.y = sin(glm::radians(transform.rotation.x));
		camFront.z = cos(glm::radians(transform.rotation.x)) * cos(glm::radians(transform.rotation.y));
		camFront = glm::normalize(camFront);

		float speed = 1.0f;


		if (mButtons.test(static_cast<std::size_t>(InputButtons::W)))
		{
			transform.position += camFront * dt *speed;
		}

		else if (mButtons.test(static_cast<std::size_t>(InputButtons::S)))
		{
			transform.position -= camFront * dt *speed;
		}


		if (mButtons.test(static_cast<std::size_t>(InputButtons::A)))
		{
			transform.position -= glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * dt *speed;
		}

		else if (mButtons.test(static_cast<std::size_t>(InputButtons::D)))
		{
			transform.position += glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * dt *speed;
		}

		cam.setCameraPos(transform.position);
		cam.setCameraUpVec(glm::vec3(0.0f,1.0f,0.0f));

		cam.genViewMat();
		cam.genProjMat();
	}
}

void CameraControlSystem::InputListener(Event& event)
{
	mButtons = event.GetParam<std::bitset<8>>(Events::Window::Input::INPUT);
}
