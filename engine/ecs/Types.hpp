#pragma once

#include <bitset>
#include <cstdint>


// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
	return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
	return fnv1a_32(s, count);
}


// ECS
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;


// Input
enum class InputButtons
{
	W,
	A,
	S,
	D,
	Q,
	E
};

// Input
enum class MouseButtons
{
	Left,
	Middle,
	Right
};




// Events
using EventId = std::uint32_t;
using ParamId = std::uint32_t;

#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

// TODO: Make these easier to define and use (macro?)
// TODO: Add some kind of enforcement/automation that a SetParam type and a GetParam type match

namespace Events::Window {
const EventId QUIT = "Events::Window::QUIT"_hash;
const EventId RESIZED = "Events::Window::RESIZED"_hash;
const EventId INPUT = "Events::Window::INPUT"_hash;
const EventId MOUSEMOVED = "Events::Window::MOUSEMOVED"_hash;
const EventId MOUSECLICKED = "Events::Window::MOUSECLICKED"_hash;
const EventId MOUSEWHEEL = "Events::Window::MOUSEWHEEL"_hash;
}

namespace Events::Window::Input {
const ParamId INPUT = "Events::Window::Input::INPUT"_hash;

}


namespace Events::Window::MouseClicked {
const ParamId MOUSEBTN = "Events::Window::MouseClicked::MOUSEBTN"_hash;
}

namespace Events::Window::MouseWheel {
const ParamId MOUSEWHEEL = "Events::Window::MouseWheel::MOUSEWHEEL"_hash;
}

namespace Events::Window::MouseMoved {
const ParamId MOUSEX = "Events::Window::MouseMoved::MOUSEX"_hash;
const ParamId MOUSEY = "Events::Window::MouseMoved::MOUSEY"_hash;
}

namespace Events::Window::Resized {
const ParamId WIDTH = "Events::Window::Resized::WIDTH"_hash;
const ParamId HEIGHT = "Events::Window::Resized::HEIGHT"_hash;
}
