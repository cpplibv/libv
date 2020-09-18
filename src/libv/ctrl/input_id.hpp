// Project: libv.ctrl, File: src/libv/ctrl/input_id.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// libv
#include <libv/utility/enum.hpp>
#include <libv/utility/extract_bits.hpp>
// std
#include <cassert>
#include <iosfwd>
// pro
#include <libv/ctrl/enum.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

class InputID {
private:
	using Rep = uint64_t;

private:
	Rep value = 0;

	/// | src type | src index | input cluster | zero padding | input index |
	/// | 8 bit    | 8 bit     | 8 bit         | 8 bit        | 32 bit      |

	[[nodiscard]] static constexpr inline Rep to_rep(
			SourceType source_type,
			SourceIndex source_index,
			InputCluster input_cluster,
			uint32_t input_index) noexcept {

		return
				(static_cast<Rep>(libv::to_value(source_type  )) << 56ull) +
				(static_cast<Rep>(libv::to_value(source_index )) << 48ull) +
				(static_cast<Rep>(libv::to_value(input_cluster)) << 40ull) +
				(static_cast<Rep>(               input_index   ) <<  0ull);
	}

	[[nodiscard]] static constexpr inline SourceType rep_to_type(Rep rep) noexcept {
		return SourceType{libv::extract_high_bits<uint8_t, 0>(rep)};
	}
	[[nodiscard]] static constexpr inline SourceIndex rep_to_source(Rep rep) noexcept {
		return SourceIndex{libv::extract_high_bits<uint8_t, 8>(rep)};
	}
	[[nodiscard]] static constexpr inline InputCluster rep_to_cluster(Rep rep) noexcept {
		return InputCluster{libv::extract_high_bits<uint8_t, 16>(rep)};
	}
	[[nodiscard]] static constexpr inline int32_t rep_to_input_s(Rep rep) noexcept {
		return libv::extract_high_bits<int32_t, 32>(rep);
	}
	[[nodiscard]] static constexpr inline uint32_t rep_to_input_u(Rep rep) noexcept {
		return libv::extract_high_bits<uint32_t, 32>(rep);
	}
	[[nodiscard]] static constexpr inline Rep rep_swap_input(Rep rep, int32_t in) noexcept {
		rep &= ~Rep{0xFFFFFFFF};
		rep |= static_cast<Rep>(in) << 0ull;
		return rep;
	}

	// -------------------------------------------------------------------------------------------------

private:
	constexpr inline InputID(SourceType source_type, SourceIndex source_index, InputCluster input_cluster, uint32_t input_index) noexcept :
		value(to_rep(source_type, source_index, input_cluster, input_index)) { }

	constexpr inline InputID(SourceType source_type) noexcept :
		InputID(source_type, SourceIndex{0}, InputCluster{0}, 0) { }

	constexpr inline InputID(SourceType source_type, uint32_t input_index) noexcept :
		InputID(source_type, SourceIndex{0}, InputCluster{0}, input_index) { }

	constexpr inline InputID(SourceType source_type, int32_t input_index) noexcept :
		InputID(source_type, SourceIndex{0}, InputCluster{0}, static_cast<uint32_t>(input_index)) { }

	constexpr inline InputID(SourceType source_type, SourceIndex source_index, int32_t input_index) noexcept :
		InputID(source_type, source_index, InputCluster{0}, static_cast<uint32_t>(input_index)) { }

	constexpr inline InputID(SourceType source_type, SourceIndex source_index, InputCluster input_cluster, int32_t input_index) noexcept :
		InputID(source_type, source_index, input_cluster, static_cast<uint32_t>(input_index)) { }

public:
	explicit constexpr inline InputID() noexcept :
		InputID(SourceType::invalid) { }
	constexpr inline InputID(const InputID& other) noexcept = default;
	constexpr inline InputID(InputID&& other) noexcept = default;
	constexpr inline InputID& operator=(const InputID& other) & noexcept = default;
	constexpr inline InputID& operator=(InputID&& other) & noexcept = default;
	inline ~InputID() = default;

public:
	explicit constexpr inline InputID(Keycode keycode) noexcept :
		InputID(SourceType::keycode, libv::to_value(keycode)) { }

	explicit constexpr inline InputID(Codepoint codepoint) noexcept :
		InputID(SourceType::codepoint, libv::to_value(codepoint)) { }

	explicit constexpr inline InputID(Scancode scancode) noexcept :
		InputID(SourceType::scancode, libv::to_value(scancode)) { }

	explicit constexpr inline InputID(Modifier modifier) noexcept :
		InputID(SourceType::modifier, libv::to_value(modifier)) { }

	explicit constexpr inline InputID(MouseButton button) noexcept :
		InputID(SourceType::mouseButton, libv::to_value(button)) { }

	explicit constexpr inline InputID(MouseScroll scroll) noexcept :
		InputID(SourceType::mouseScroll, libv::to_value(scroll)) { }

	explicit constexpr inline InputID(MouseMovement movement) noexcept :
		InputID(SourceType::mouseMovement, libv::to_value(movement)) { }

	explicit constexpr inline InputID(GamepadID gamepad, GamepadButton button) noexcept :
		InputID(SourceType::gamepadButton, SourceIndex{libv::to_value(gamepad)}, libv::to_value(button)) { }

	explicit constexpr inline InputID(GamepadID gamepad, GamepadAnalogID id, AnalogDimension dim) noexcept :
		InputID(SourceType::gamepadAnalog, SourceIndex{libv::to_value(gamepad)}, InputCluster{libv::to_value(id)}, libv::to_value(dim)) { }

	explicit constexpr inline InputID(JoystickID joystick, JoystickButton button) noexcept :
		InputID(SourceType::joystickButton, SourceIndex{libv::to_value(joystick)}, libv::to_value(button)) { }

	explicit constexpr inline InputID(JoystickID joystick, JoystickAnalogID id, AnalogDimension dim) noexcept :
		InputID(SourceType::joystickAnalog, SourceIndex{libv::to_value(joystick)}, InputCluster{libv::to_value(id)}, libv::to_value(dim)) { }

public:
	[[nodiscard]] constexpr inline bool is_analog() const noexcept {
		switch (type()) {
		case SourceType::invalid: return false;

		case SourceType::codepoint: return false;
		case SourceType::keycode: return false;
		case SourceType::scancode: return false;
		case SourceType::modifier: return false;

		case SourceType::mouseButton: return false;
		case SourceType::mouseMovement: return true;
		case SourceType::mouseScroll: return true;

		case SourceType::gamepadButton: return false;
		case SourceType::gamepadAnalog: return true;

		case SourceType::joystickButton: return false;
		case SourceType::joystickAnalog: return true;
		}

		assert(false && "Invalid source type");
		return false;
	}

	[[nodiscard]] constexpr inline bool is_button() const noexcept {
		switch (type()) {
		case SourceType::invalid: return false;

		case SourceType::codepoint: return true;
		case SourceType::keycode: return true;
		case SourceType::scancode: return true;
		case SourceType::modifier: return true;

		case SourceType::mouseButton: return true;
		case SourceType::mouseMovement: return false;
		case SourceType::mouseScroll: return false;

		case SourceType::gamepadButton: return true;
		case SourceType::gamepadAnalog: return false;

		case SourceType::joystickButton: return true;
		case SourceType::joystickAnalog: return false;
		}

		assert(false && "Invalid source type");
		return false;
	}

public:
	[[nodiscard]] constexpr inline SourceType type() const noexcept {
		return rep_to_type(value);
	}

	[[nodiscard]] constexpr inline Codepoint codepoint() const noexcept {
		assert(type() == SourceType::codepoint);
		return Codepoint{rep_to_input_u(value)};
	}

	[[nodiscard]] constexpr inline Keycode keycode() const noexcept {
		assert(type() == SourceType::keycode);
		return Keycode{rep_to_input_s(value)};
	}

	[[nodiscard]] constexpr inline Scancode scancode() const noexcept {
		assert(type() == SourceType::scancode);
		return Scancode{rep_to_input_s(value)};
	}

	[[nodiscard]] constexpr inline Modifier modifier() const noexcept {
		assert(type() == SourceType::modifier);
		return Modifier{rep_to_input_s(value)};
	}

	[[nodiscard]] constexpr inline MouseButton mouseButton() const noexcept {
		assert(type() == SourceType::mouseButton);
		return MouseButton{rep_to_input_s(value)};
	}

	[[nodiscard]] constexpr inline MouseScroll mouseScroll() const noexcept {
		assert(type() == SourceType::mouseScroll);
		return MouseScroll{rep_to_input_s(value)};
	}

	[[nodiscard]] constexpr inline MouseMovement mouseMovement() const noexcept {
		assert(type() == SourceType::mouseMovement);
		return MouseMovement{rep_to_input_s(value)};
	}

	[[nodiscard]] constexpr inline GamepadID gamepadID() const noexcept {
		assert(type() == SourceType::gamepadButton || type() == SourceType::gamepadAnalog);
		return GamepadID{libv::to_value(rep_to_source(value))};
	}

	[[nodiscard]] constexpr inline GamepadAnalogID gamepadAnalogID() const noexcept {
		assert(type() == SourceType::gamepadAnalog);
		return GamepadAnalogID{libv::to_value(rep_to_cluster(value))};
	}

	[[nodiscard]] constexpr inline GamepadButton gamepadButton() const noexcept {
		assert(type() == SourceType::gamepadButton);
		return GamepadButton{rep_to_input_s(value)};
	}

	[[nodiscard]] constexpr inline JoystickID joystickID() const noexcept {
		assert(type() == SourceType::joystickButton || type() == SourceType::joystickAnalog);
		return JoystickID{libv::to_value(rep_to_source(value))};
	}

	[[nodiscard]] constexpr inline JoystickAnalogID joystickAnalogID() const noexcept {
		assert(type() == SourceType::joystickAnalog);
		return JoystickAnalogID{libv::to_value(rep_to_cluster(value))};
	}

	[[nodiscard]] constexpr inline JoystickButton joystickButton() const noexcept {
		assert(type() == SourceType::joystickButton);
		return JoystickButton{rep_to_input_s(value)};
	}

	[[nodiscard]] constexpr inline AnalogDimension analogDimension() const noexcept {
		assert(is_analog());
		return AnalogDimension{rep_to_input_s(value)};
	}

public:
	constexpr inline void analogDimension(AnalogDimension dim) noexcept {
		assert(is_analog());
		value = rep_swap_input(value, libv::to_value(dim));
	}

public:
	[[nodiscard]] constexpr inline Rep rep() const noexcept {
		return value;
	}

public:
	[[nodiscard]] friend constexpr inline bool operator==(const InputID& lhs, const InputID& rhs) noexcept {
		return lhs.value == rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(const InputID& lhs, const InputID& rhs) noexcept {
		return !(lhs == rhs);
	}
	[[nodiscard]] friend constexpr inline bool operator<(const InputID& lhs, const InputID& rhs) noexcept {
		return lhs.value < rhs.value;
	}

public:
	std::ostream& to_stream_symbol(std::ostream& os) const;
	std::ostream& to_stream_identifier(std::ostream& os) const;
	std::ostream& to_stream_name(std::ostream& os) const;
	std::ostream& to_stream_description(std::ostream& os) const;

	[[nodiscard]] std::string to_string_symbol() const;
	[[nodiscard]] std::string to_string_identifier() const;
	[[nodiscard]] std::string to_string_name() const;
	[[nodiscard]] std::string to_string_description() const;

	/// Shortcut to member function to_stream_name
	[[nodiscard]] std::string to_string() const;

	/// Shortcut to member function to_stream_name
	friend inline std::ostream& operator<<(std::ostream& os, const InputID& var) {
		return var.to_stream_name(os);
	}

public:
//	auto aliases() {
//		// TODO P5: List of every inputID that could result in a match (will be useful for GUI)
//	}
};

/// This function does not account for scancode or codepoint aliasing
[[nodiscard]] bool alias_match(const InputID a, const InputID b) noexcept;

/// This function does not account for analog dimension, scancode or codepoint aliasing
/// @return 0 If A and B are equal or not aliases, -1 if A is more specialized, +1 if B is more specialized
[[nodiscard]] int more_specialized(const InputID a, const InputID b) noexcept;

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
