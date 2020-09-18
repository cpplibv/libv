// Project: libv.ctrl, File: src/libv/ctrl/parse.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// std
#include <optional>
#include <string_view>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

///
/// --- Input sequence syntax ---
///
/// DigitalInputAction ::= "[" $("press" | "repeat" | "release" | "any" | "hold" | "free") "]"
/// Dimension          ::= $("X" | "X+" | "X-" | "Y" | "Y+" | "Y-")
///
/// Codepoint       ::= '"' $codepoint '"'
/// Keycode         ::= $keycode
/// Scancode        ::= "sc" $scancode
/// MouseButton     ::= "Mouse" $button
/// MouseMove       ::= "Mouse" Dimension
/// MouseScroll     ::= "Scroll" Dimension
/// GamepadButton   ::= "Gamepad" [$device_id] "Button" $button
/// GamepadAnalog   ::= "Gamepad" [$device_id] "Analog" $analog_id ":" Dimension
/// JoystickButton  ::= "Joystick" [$device_id] "Button" $button
/// JoystickAnalog  ::= "Joystick" [$device_id] "Analog" $analog_id ":" Dimension
///
/// InputID         ::= Codepoint | Keycode | Scancode | MouseButton | MouseMove | MouseScroll
///							| GamepadButton | GamepadAnalog | JoystickButton | JoystickAnalog
/// Input           ::= InputID [DigitalInputAction]
/// Combination     ::= Input ("+" Input)*
/// Sequence        ::= Combination ("," Combination)*
///
/// Examples:
///		W
///		W [hold]
///		W [press]
///		Ctrl + Z
///		Ctrl + "Z"
///		Ctrl + "Z" [repeat]
///		Mouse Right [hold]
///		Mouse X
///		Mouse X+ + Ctrl
///		Mouse Right [hold] + Mouse X
///		Right Mouse [hold] + Left Mouse [hold] + Mouse Y
///		Ctrl + U, U
///

// -------------------------------------------------------------------------------------------------

InputID parse_input_id_or(const std::string_view str, const InputID& fallback);
InputID parse_input_id_or_throw(const std::string_view str);
std::optional<InputID> parse_input_id_optional(const std::string_view str);

Input parse_input_or(const std::string_view str, const Input& fallback);
Input parse_input_or_throw(const std::string_view str);
std::optional<Input> parse_input_optional(const std::string_view str);

Combination parse_combination_or(const std::string_view str, const Combination& fallback);
Combination parse_combination_or_throw(const std::string_view str);
std::optional<Combination> parse_combination_optional(const std::string_view str);

Sequence parse_sequence_or(const std::string_view str, const Sequence& fallback);
Sequence parse_sequence_or_throw(const std::string_view str);
std::optional<Sequence> parse_sequence_optional(const std::string_view str);

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
