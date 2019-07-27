// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <string_view>
// pro
#include <libv/input/input.hpp>


namespace libv {
namespace control {

// -------------------------------------------------------------------------------------------------

// Keyboard

enum class Codepoint : uint32_t {
};

using Keycode = libv::input::Key;

using Scancode = libv::input::Scancode;

enum class Modifier : int32_t {
	shift,
	control,
	alt,
	super,
};

// Mouse

using MouseButton = libv::input::MouseButton;

/// NOTE: Compatible with AnalogDimension
enum class MouseMovement : int32_t {
	x,
	x_plus,
	x_minus,
	y,
	y_plus,
	y_minus,
};

/// NOTE: Compatible with AnalogDimension
enum class MouseScroll : int32_t {
	x,
	x_plus,
	x_minus,
	y,
	y_plus,
	y_minus,
};

// Joystick / Gamepad

enum class AnalogDimension : int32_t {
	x,
	x_plus,
	x_minus,
	y,
	y_plus,
	y_minus,
};

using GamepadID = libv::input::GamepadID;

using GamepadAnalogID = libv::input::GamepadAnalogID;

using GamepadButton = libv::input::GamepadButton;

using JoystickID = libv::input::JoystickID;

using JoystickAnalogID = libv::input::JoystickAnalogID;

using JoystickButton = libv::input::JoystickButton;

// -------------------------------------------------------------------------------------------------

enum class SourceType : uint8_t {
	invalid = 0,

	modifier,
	codepoint,
	keycode,
	scancode,

	mouseButton,
	mouseScroll,
	mouseMovement,

	gamepadButton,
	gamepadAnalog,

	joystickButton,
	joystickAnalog,
};

enum class SourceIndex : uint8_t {
};

enum class InputCluster : uint8_t {
};

// -------------------------------------------------------------------------------------------------

using ButtonAction = libv::input::Action;

enum class DigitalInputAction : int32_t {
	auto_,

	press,
	repeat,
	release,
	any,

	hold,
	free,
};

//// Used for binding information
//enum class InputEventType : int32_t {
//	analog,
//	digital,
//	mouse,
//};

// -------------------------------------------------------------------------------------------------

enum class OperationBinary : int32_t {
	enable,
	disable,
	toggle,

	state,
	inverted,
};

// -------------------------------------------------------------------------------------------------

std::string_view to_string(const DigitalInputAction& var);
std::string_view to_string(const OperationBinary& var);

// -------------------------------------------------------------------------------------------------

} // namespace control
} // namespace libv
