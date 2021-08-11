#pragma once

#include "../System.hpp"


class Event;


class CameraControlSystem : public System
{
public:
	void Init();

	void Update(float dt);

private:
	std::bitset<8> mButtons;

	void InputListener(Event& event);
};
