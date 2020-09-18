// Project: libv.ctrl, File: src/libv/ctrl/input.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// std
#include <iosfwd>
#include <string>
// pro
#include <libv/ctrl/enum.hpp>
#include <libv/ctrl/input_id.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

struct Input {
	InputID id;
	DigitalInputAction dia = DigitalInputAction::auto_;

public:
	explicit constexpr inline Input() noexcept = default;

public:
	explicit constexpr inline Input(InputID id, DigitalInputAction dia) noexcept :
		id(id),
		dia(dia) {}

public:
	explicit constexpr inline Input(Keycode keycode, DigitalInputAction dia = DigitalInputAction::auto_) noexcept :
		Input(InputID{keycode}, dia) { }

	explicit constexpr inline Input(Codepoint codepoint, DigitalInputAction dia = DigitalInputAction::auto_) noexcept :
		Input(InputID{codepoint}, dia) { }

	explicit constexpr inline Input(Scancode scancode, DigitalInputAction dia = DigitalInputAction::auto_) noexcept :
		Input(InputID{scancode}, dia) { }

	explicit constexpr inline Input(Modifier modifier, DigitalInputAction dia = DigitalInputAction::auto_) noexcept :
		Input(InputID{modifier}, dia) { }

	explicit constexpr inline Input(MouseButton button, DigitalInputAction dia = DigitalInputAction::auto_) noexcept :
		Input(InputID{button}, dia) { }

	explicit constexpr inline Input(MouseScroll scroll) noexcept :
		Input(InputID{scroll}, DigitalInputAction::auto_) { }

	explicit constexpr inline Input(MouseMovement movement) noexcept :
		Input(InputID{movement}, DigitalInputAction::auto_) { }

	explicit constexpr inline Input(GamepadID gamepad, GamepadButton button, DigitalInputAction dia = DigitalInputAction::auto_) noexcept :
		Input(InputID{gamepad, button}, dia) { }

	explicit constexpr inline Input(GamepadID gamepad, GamepadAnalogID id, AnalogDimension dim) noexcept :
		Input(InputID{gamepad, id, dim}, DigitalInputAction::auto_) { }

	explicit constexpr inline Input(JoystickID joystick, JoystickButton button, DigitalInputAction dia = DigitalInputAction::auto_) noexcept :
		Input(InputID{joystick, button}, dia) { }

	explicit constexpr inline Input(JoystickID joystick, JoystickAnalogID id, AnalogDimension dim) noexcept :
		Input(InputID{joystick, id, dim}, DigitalInputAction::auto_) { }

public:
	[[nodiscard]] friend constexpr inline bool operator<(const Input& lhs, const Input& rhs) noexcept {
		return lhs.id < rhs.id || (lhs.id == rhs.id && lhs.dia < rhs.dia);
	}
	[[nodiscard]] friend constexpr inline bool operator==(const Input& lhs, const Input& rhs) noexcept {
		return lhs.id == rhs.id && lhs.dia == rhs.dia;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(const Input& lhs, const Input& rhs) noexcept {
		return !(lhs == rhs);
	}

public:
	std::ostream& to_stream_symbol(std::ostream& os) const;
	std::ostream& to_stream_name(std::ostream& os) const;

	[[nodiscard]] std::string to_string_symbol() const;
	[[nodiscard]] std::string to_string_name() const;

	/// Shortcut to member function to_stream_name
	[[nodiscard]] std::string to_string() const;

	/// Shortcut to member function to_stream_name
	friend inline std::ostream& operator<<(std::ostream& os, const Input& var) {
		return var.to_stream_name(os);
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
