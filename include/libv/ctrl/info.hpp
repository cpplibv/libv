// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <iosfwd>
#include <string>
// pro
#include <libv/ctrl/enum.hpp>
#include <libv/ctrl/fwd.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

/// Symbol: Shortest representation of an input, may includes symbols. Most suitable for HUD display. Parsable.
/// Identifier: Identifier name that only contains [0-9a-z_] characters. Most suitable for filename or resource mappings.
/// Name: Comfortable human readable name. Most suitable for configuration outputs and display bindings or binding tips. Parsable.
/// Description: Pretty long name, space separated and verbose. Most suitable for explanatory popups. Not parsable.

// -------------------------------------------------------------------------------------------------

std::ostream& to_stream_symbol(std::ostream& os, const InputID inputID);
std::ostream& to_stream_identifier(std::ostream& os, const InputID inputID);
std::ostream& to_stream_name(std::ostream& os, const InputID inputID);
std::ostream& to_stream_description(std::ostream& os, const InputID inputID);

std::string to_string_symbol(const InputID inputID);
std::string to_string_identifier(const InputID inputID);
std::string to_string_name(const InputID inputID);
std::string to_string_description(const InputID inputID);

std::ostream& to_stream_symbol(std::ostream& os, const Input inputID);
std::ostream& to_stream_name(std::ostream& os, const Input inputID);

std::string to_string_symbol(const Input inputID);
std::string to_string_name(const Input inputID);

std::ostream& to_stream_symbol(std::ostream& os, const Combination& inputID);
std::ostream& to_stream_name(std::ostream& os, const Combination& inputID);

std::string to_string_symbol(const Combination& inputID);
std::string to_string_name(const Combination& inputID);

std::ostream& to_stream_symbol(std::ostream& os, const Sequence& inputID);
std::ostream& to_stream_name(std::ostream& os, const Sequence& inputID);

std::string to_string_symbol(const Sequence& inputID);
std::string to_string_name(const Sequence& inputID);

// -------------------------------------------------------------------------------------------------

std::ostream& to_symbol(std::ostream& os, Keycode keycode);
std::ostream& to_symbol(std::ostream& os, Codepoint codepoint);
std::ostream& to_symbol(std::ostream& os, Scancode scancode);
std::ostream& to_symbol(std::ostream& os, Modifier modifier);
std::ostream& to_symbol(std::ostream& os, MouseButton button);
std::ostream& to_symbol(std::ostream& os, MouseMovement movement);
std::ostream& to_symbol(std::ostream& os, MouseScroll scroll);
std::ostream& to_symbol(std::ostream& os, JoystickID joystick, JoystickButton button);
std::ostream& to_symbol(std::ostream& os, JoystickID joystick, JoystickAnalogID id, AnalogDimension dim);

std::ostream& to_identifier(std::ostream& os, Keycode keycode);
std::ostream& to_identifier(std::ostream& os, Codepoint codepoint);
std::ostream& to_identifier(std::ostream& os, Scancode scancode);
std::ostream& to_identifier(std::ostream& os, Modifier modifier);
std::ostream& to_identifier(std::ostream& os, MouseButton button);
std::ostream& to_identifier(std::ostream& os, MouseMovement movement);
std::ostream& to_identifier(std::ostream& os, MouseScroll scroll);
std::ostream& to_identifier(std::ostream& os, JoystickID joystick, JoystickButton button);
std::ostream& to_identifier(std::ostream& os, JoystickID joystick, JoystickAnalogID id, AnalogDimension dim);

std::ostream& to_name(std::ostream& os, Keycode keycode);
std::ostream& to_name(std::ostream& os, Codepoint codepoint);
std::ostream& to_name(std::ostream& os, Scancode scancode);
std::ostream& to_name(std::ostream& os, Modifier modifier);
std::ostream& to_name(std::ostream& os, MouseButton button);
std::ostream& to_name(std::ostream& os, MouseMovement movement);
std::ostream& to_name(std::ostream& os, MouseScroll scroll);
std::ostream& to_name(std::ostream& os, JoystickID joystick, JoystickButton button);
std::ostream& to_name(std::ostream& os, JoystickID joystick, JoystickAnalogID id, AnalogDimension dim);

std::ostream& to_description(std::ostream& os, Keycode keycode);
std::ostream& to_description(std::ostream& os, Codepoint codepoint);
std::ostream& to_description(std::ostream& os, Scancode scancode);
std::ostream& to_description(std::ostream& os, Modifier modifier);
std::ostream& to_description(std::ostream& os, MouseButton button);
std::ostream& to_description(std::ostream& os, MouseMovement movement);
std::ostream& to_description(std::ostream& os, MouseScroll scroll);
std::ostream& to_description(std::ostream& os, JoystickID joystick, JoystickButton button);
std::ostream& to_description(std::ostream& os, JoystickID joystick, JoystickAnalogID id, AnalogDimension dim);

std::string to_symbol(Keycode keycode);
std::string to_symbol(Codepoint codepoint);
std::string to_symbol(Scancode scancode);
std::string to_symbol(Modifier modifier);
std::string to_symbol(MouseButton button);
std::string to_symbol(MouseMovement movement);
std::string to_symbol(MouseScroll scroll);
std::string to_symbol(JoystickID joystick, JoystickButton button);
std::string to_symbol(JoystickID joystick, JoystickAnalogID id, AnalogDimension dim);

std::string to_identifier(Keycode keycode);
std::string to_identifier(Codepoint codepoint);
std::string to_identifier(Scancode scancode);
std::string to_identifier(Modifier modifier);
std::string to_identifier(MouseButton button);
std::string to_identifier(MouseMovement movement);
std::string to_identifier(MouseScroll scroll);
std::string to_identifier(JoystickID joystick, JoystickButton button);
std::string to_identifier(JoystickID joystick, JoystickAnalogID id, AnalogDimension dim);

std::string to_name(Keycode keycode);
std::string to_name(Codepoint codepoint);
std::string to_name(Scancode scancode);
std::string to_name(Modifier modifier);
std::string to_name(MouseButton button);
std::string to_name(MouseMovement movement);
std::string to_name(MouseScroll scroll);
std::string to_name(JoystickID joystick, JoystickButton button);
std::string to_name(JoystickID joystick, JoystickAnalogID id, AnalogDimension dim);

std::string to_description(Keycode keycode);
std::string to_description(Codepoint codepoint);
std::string to_description(Scancode scancode);
std::string to_description(Modifier modifier);
std::string to_description(MouseButton button);
std::string to_description(MouseMovement movement);
std::string to_description(MouseScroll scroll);
std::string to_description(JoystickID joystick, JoystickButton button);
std::string to_description(JoystickID joystick, JoystickAnalogID id, AnalogDimension dim);

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
