// Project: libv.input, File: src/libv/input/event.hpp

#pragma once

// fwd
#include <libv/input/event_fwd.hpp>
// libv
#include <libv/math/vec.hpp>
// std
#include <string>
#include <vector>
// pro
#include <libv/input/input.hpp>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

struct EventChar {
	uint32_t unicode;
	char utf8[5]; /// Null terminated utf8 representation of the unicode character (one to four octet + terminating zero)

	EventChar(uint32_t unicode);
	[[nodiscard]] std::string toPrettyString() const;
};

struct EventDrop {
	std::vector<std::string> strings;

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventKey {
	libv::input::Keycode keycode;
	libv::input::Scancode scancode;
	libv::input::Action action;

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventMouseButton {
	libv::input::MouseButton button;
	libv::input::Action action;

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventMouseEnter {
	bool entered;

	inline EventMouseEnter(int entered) :
		entered(entered != 0) { }

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventMousePosition {
	libv::vec2d position;

	inline EventMousePosition(double xpos, double ypos) :
		position(xpos, ypos) { }
	inline EventMousePosition(libv::vec2d position) :
		position(position) { }

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventMouseScroll {
	libv::vec2d offset;

	inline EventMouseScroll(double xoffset, double yoffset) :
		offset(xoffset, yoffset) { }
	inline EventMouseScroll(libv::vec2d offset) :
		offset(offset) { }

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventGamepadAnalog {
	libv::input::GamepadID gamepadID;
	libv::input::GamepadAnalogID analogID;
	libv::vec2d position;

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventGamepadButton {
	libv::input::GamepadID gamepadID;
	libv::input::GamepadButton button;
	libv::input::Action action;

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventJoystickAnalog {
	libv::input::JoystickID joystickID;
	libv::input::JoystickAnalogID analogID;
	libv::vec2d position;

	[[nodiscard]] std::string toPrettyString() const;
};

struct EventJoystickButton {
	libv::input::JoystickID joystickID;
	libv::input::JoystickButton button;
	libv::input::Action action;

	[[nodiscard]] std::string toPrettyString() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
