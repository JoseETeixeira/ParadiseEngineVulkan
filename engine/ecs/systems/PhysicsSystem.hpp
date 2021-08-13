#pragma once

#include "../System.hpp"


class PhysicsSystem : public System
{
public:
	void Init();

	void Update(float dt);
};
