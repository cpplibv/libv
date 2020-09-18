// Project: libv.input, File: src/libv/input/event.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/input/event.hpp>
// libv
#include <libv/utility/enum.hpp>
#include <libv/utility/utf8.hpp>
// ext
#include <boost/algorithm/string/join.hpp>
#include <fmt/format.h>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

EventChar::EventChar(uint32_t unicode) :
	unicode(unicode) {
	libv::unicode_to_utf8(unicode, utf8.data());
}

std::string EventChar::toPrettyString() const {
	return fmt::format("Char: unicode = {}, utf8 = {}", unicode, utf8.data());
}

std::string EventDrop::toPrettyString() const {
	return fmt::format("Drop: size = {}, contents: \n\"{}\"",
			strings.size(), boost::algorithm::join(strings, "\"\n\""));
}

std::string EventKey::toPrettyString() const {
	return fmt::format("Key: key = {} ({}), scancode = {}, action = {} ({})",
			libv::input::to_string(keycode), libv::to_value(keycode), scancode, libv::input::to_string(action), libv::to_value(action));
}

std::string EventMouseButton::toPrettyString() const {
	return fmt::format("Mouse Button: button = {} ({}), action = {} ({})",
			libv::input::to_string(button), libv::to_value(button), libv::input::to_string(action), libv::to_value(action));
}

std::string EventMouseEnter::toPrettyString() const {
	return fmt::format("Mouse Enter: entered = {}", entered);
}

std::string EventMousePosition::toPrettyString() const {
	return fmt::format("Mouse Position: position = ({}, {})", position.x, position.y);
}

std::string EventMouseScroll::toPrettyString() const {
	return fmt::format("Mouse Scroll: offset = ({}, {})", offset.x, offset.y);
}

std::string EventGamepadAnalog::toPrettyString() const {
	return fmt::format("Gamepad Analog: gamepad = {}, analog = {}, position = ({}, {})",
			libv::to_value(gamepadID), libv::to_value(analogID), position.x, position.y);
}

std::string EventGamepadButton::toPrettyString() const {
	return fmt::format("Gamepad Button: gamepad = {}, button = {} ({}), action = {} ({})",
			libv::to_value(gamepadID), libv::input::to_string(button), libv::to_value(button), libv::input::to_string(action), libv::to_value(action));
}

std::string EventJoystickAnalog::toPrettyString() const {
	return fmt::format("Joystick Analog: joystick = {}, analog = {}, position = ({}, {})", libv::to_value(joystickID), libv::to_value(analogID), position.x, position.y);
}

std::string EventJoystickButton::toPrettyString() const {
	return fmt::format("Joystick Button: joystick = {}, button = {}, action = {} ({})",
			libv::to_value(joystickID), libv::to_value(button), libv::input::to_string(action), libv::to_value(action));
}

// -------------------------------------------------------------------------------------------------

} // namespace input
} // namespace libv
