// Project: libv.ctrl, File: src/libv/ctrl/input_id.cpp

// hpp
#include <libv/ctrl/input_id.hpp>
// libv
#include <libv/utility/utf8.hpp>
// std
#include <array>
#include <ostream>
// pro
#include <libv/ctrl/info.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

std::ostream& InputID::to_stream_symbol(std::ostream& os) const {
	return libv::ctrl::to_stream_symbol(os, *this);
}
std::ostream& InputID::to_stream_identifier(std::ostream& os) const{
	return libv::ctrl::to_stream_identifier(os, *this);
}
std::ostream& InputID::to_stream_name(std::ostream& os) const{
	return libv::ctrl::to_stream_name(os, *this);
}
std::ostream& InputID::to_stream_description(std::ostream& os) const{
	return libv::ctrl::to_stream_description(os, *this);
}

std::string InputID::to_string_symbol() const {
	return libv::ctrl::to_string_symbol(*this);
}
std::string InputID::to_string_identifier() const {
	return libv::ctrl::to_string_identifier(*this);
}
std::string InputID::to_string_name() const {
	return libv::ctrl::to_string_name(*this);
}
std::string InputID::to_string_description() const {
	return libv::ctrl::to_string_description(*this);
}

std::string InputID::to_string() const {
	return to_string_name();
}

// -------------------------------------------------------------------------------------------------

namespace  {

constexpr inline bool alias_modifier_keycode(const Modifier m, const Keycode k) {
	return
			(m == Modifier::alt && (k == Keycode::AltLeft || k == Keycode::AltRight)) ||
			(m == Modifier::control && (k == Keycode::ControlLeft || k == Keycode::ControlRight)) ||
			(m == Modifier::shift && (k == Keycode::ShiftLeft || k == Keycode::ShiftRight)) ||
			(m == Modifier::super && (k == Keycode::SuperLeft || k == Keycode::SuperRight));
}

} // namespace

// -------------------------------------------------------------------------------------------------

bool alias_match(const InputID a, const InputID b) noexcept {
	const auto match_analog_dimension = [](const auto dima, const auto dimb) noexcept {
		if (dima == dimb)
			return true;

		switch (dima) {
		case AnalogDimension::x: return dimb == AnalogDimension::x_plus || dimb == AnalogDimension::x_minus;
		case AnalogDimension::x_plus: return dimb == AnalogDimension::x;
		case AnalogDimension::x_minus: return dimb == AnalogDimension::x;
		case AnalogDimension::y: return dimb == AnalogDimension::y_plus || dimb == AnalogDimension::y_minus;
		case AnalogDimension::y_plus: return dimb == AnalogDimension::y;
		case AnalogDimension::y_minus: return dimb == AnalogDimension::y;
		}

		assert(false && "Invalid analog dimension");
		return false;
	};

	if (a == b)
		return true;

	switch (a.type()) {
	case SourceType::invalid:
		return b.type() == SourceType::invalid;

	case SourceType::codepoint:
		return false;
	case SourceType::keycode: {
		if (b.type() != SourceType::modifier)
			return false;
		return alias_modifier_keycode(b.modifier(), a.keycode());
	}
	case SourceType::scancode:
		return false;
	case SourceType::modifier:
		if (b.type() != SourceType::keycode)
			return false;
		return alias_modifier_keycode(a.modifier(), b.keycode());

	case SourceType::mouseButton:
		return false;
	case SourceType::mouseMovement:
		if (b.type() != SourceType::mouseMovement)
			return false;
		return match_analog_dimension(a.analogDimension(), b.analogDimension());

	case SourceType::mouseScroll:
		if (b.type() != SourceType::mouseScroll)
			return false;
		return match_analog_dimension(a.analogDimension(), b.analogDimension());

	case SourceType::gamepadButton:
		if (a.gamepadID() != GamepadID::any && b.gamepadID() != GamepadID::any)
			return false;
		return a.gamepadButton() == b.gamepadButton();

	case SourceType::gamepadAnalog:
		if (b.type() != SourceType::gamepadAnalog)
			return false;

		if (a.gamepadID() != b.gamepadID() && a.gamepadID() != GamepadID::any && b.gamepadID() != GamepadID::any)
			return false;

		return a.gamepadAnalogID() == b.gamepadAnalogID()
				&& match_analog_dimension(a.analogDimension(), b.analogDimension());

	case SourceType::joystickButton:
		if (a.joystickID() != JoystickID::any && b.joystickID() != JoystickID::any)
			return false;
		return a.joystickButton() == b.joystickButton();

	case SourceType::joystickAnalog:
		if (b.type() != SourceType::joystickAnalog)
			return false;

		if (a.joystickID() != b.joystickID() && a.joystickID() != JoystickID::any && b.joystickID() != JoystickID::any)
			return false;

		return a.joystickAnalogID() == b.joystickAnalogID()
				&& match_analog_dimension(a.analogDimension(), b.analogDimension());
	}

	assert(false && "Invalid source type");
	return false;
}

// -------------------------------------------------------------------------------------------------

int more_specialized(const InputID a, const InputID b) noexcept {
	const auto a_is_more_specialized = 1;
	const auto b_is_more_specialized = -1;

	if (a == b)
		return 0;

	switch (a.type()) {
	case SourceType::invalid: return 0;

	case SourceType::codepoint: return 0;
	case SourceType::keycode: {
		if (b.type() != SourceType::modifier)
			return 0;
		return alias_modifier_keycode(b.modifier(), a.keycode()) ? a_is_more_specialized : 0;
	}
	case SourceType::modifier:
		if (b.type() != SourceType::keycode)
			return 0;
		return alias_modifier_keycode(a.modifier(), b.keycode()) ? b_is_more_specialized : 0;
	case SourceType::scancode: return 0;

	case SourceType::mouseButton: return 0;
	case SourceType::mouseMovement: return 0;
	case SourceType::mouseScroll: return 0;

	case SourceType::gamepadButton:
		if (a.gamepadID() == b.gamepadID())
			return 0;

		if (a.gamepadButton() != b.gamepadButton())
			return 0;

		if (a.gamepadID() == GamepadID::any)
			return b_is_more_specialized;

		if (b.gamepadID() == GamepadID::any)
			return a_is_more_specialized;

		return 0;

	case SourceType::gamepadAnalog:
		if (a.gamepadID() == b.gamepadID())
			return 0;

		if (a.analogDimension() != b.analogDimension())
			return 0;

		if (a.gamepadID() == GamepadID::any)
			return b_is_more_specialized;

		if (b.gamepadID() == GamepadID::any)
			return a_is_more_specialized;

		return 0;

	case SourceType::joystickButton:
		if (a.joystickID() == b.joystickID())
			return 0;

		if (a.joystickButton() != b.joystickButton())
			return 0;

		if (a.joystickID() == JoystickID::any)
			return b_is_more_specialized;

		if (b.joystickID() == JoystickID::any)
			return a_is_more_specialized;

		return 0;

	case SourceType::joystickAnalog:
		if (a.joystickID() == b.joystickID())
			return 0;

		if (a.analogDimension() != b.analogDimension())
			return 0;

		if (a.joystickID() == JoystickID::any)
			return b_is_more_specialized;

		if (b.joystickID() == JoystickID::any)
			return a_is_more_specialized;

		return 0;
	}

	assert(false && "Invalid source type");
	return 0;
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
