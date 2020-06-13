// File: inputs.hpp Author: Vader Created on 2017. január 24., 3:11

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <array>
#include <string>
#include <variant>
#include <vector>
// pro
#include <libv/input/event_fwd.hpp>
#include <libv/input/input.hpp>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

struct EventChar {
	uint32_t unicode;
	std::array<char, 5> utf8; /// Null terminated utf8 representation of the unicode character (one to four octet + terminating zero)

	EventChar(uint32_t unicode);
	std::string toPrettyString() const;
};

struct EventDrop {
	std::vector<std::string> strings;

	std::string toPrettyString() const;
};

struct EventKey {
	libv::input::Keycode keycode;
	libv::input::Scancode scancode;
	libv::input::Action action;

	std::string toPrettyString() const;
};

struct EventMouseButton {
	libv::input::MouseButton button;
	libv::input::Action action;

	std::string toPrettyString() const;
};

struct EventMouseEnter {
	bool entered;

	inline EventMouseEnter(int entered) :
		entered(entered != 0) { }

	std::string toPrettyString() const;
};

struct EventMousePosition {
	libv::vec2d position;

	inline EventMousePosition(double xpos, double ypos) :
		position(xpos, ypos) { }
	inline EventMousePosition(libv::vec2d position) :
		position(position) { }

	std::string toPrettyString() const;
};

struct EventMouseScroll {
	libv::vec2d offset;

	inline EventMouseScroll(double xoffset, double yoffset) :
		offset(xoffset, yoffset) { }
	inline EventMouseScroll(libv::vec2d offset) :
		offset(offset) { }

	std::string toPrettyString() const;
};

struct EventGamepadAnalog {
	libv::input::GamepadID gamepadID;
	libv::input::GamepadAnalogID analogID;
	libv::vec2d position;

	std::string toPrettyString() const;
};

struct EventGamepadButton {
	libv::input::GamepadID gamepadID;
	libv::input::GamepadButton button;
	libv::input::Action action;

	std::string toPrettyString() const;
};

struct EventJoystickAnalog {
	libv::input::JoystickID joystickID;
	libv::input::JoystickAnalogID analogID;
	libv::vec2d position;

	std::string toPrettyString() const;
};

struct EventJoystickButton {
	libv::input::JoystickID joystickID;
	libv::input::JoystickButton button;
	libv::input::Action action;

	std::string toPrettyString() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
