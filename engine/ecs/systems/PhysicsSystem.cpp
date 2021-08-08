#include "PhysicsSystem.hpp"

#include "../components/Gravity.hpp"
#include "../components/RigidBody.hpp"
#include "../components/Thrust.hpp"
#include "../components/Transform.hpp"
#include "../Coordinator.hpp"


extern Coordinator gCoordinator;


void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(float dt)
{
	for (auto const& entity : mEntities)
	{
		auto& rigidBody = gCoordinator.GetComponent<RigidBody>(entity);
		auto& transform = gCoordinator.GetComponent<Transform>(entity);

		// Forces
		auto const& gravity = gCoordinator.GetComponent<Gravity>(entity);

		transform.position += rigidBody.velocity * dt;

		rigidBody.velocity += gravity.force * dt;
	}
}
