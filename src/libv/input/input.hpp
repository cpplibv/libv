// Project: libv.input, File: src/libv/input/input.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/to_underlying.hpp>
// std
#include <cstdint>
#include <string_view>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

enum class MonitorEvent : int32_t {
	connected = 0x00040001,
	disconnected = 0x00040002,
};

enum class Action : int32_t {
	release = 0,
	press = 1,
	repeat = 2,
};

enum class KeyState : int32_t {
	released = 0,
	pressed = 1,
};

enum class KeyModifier : int32_t {
	none = 0x0000,

	shift = 0x0001,
 	control = 0x0002,
	alt = 0x0004,
	super = 0x0008,

	caps_lock = 0x0010,
	num_lock = 0x0020,
};

[[nodiscard]] constexpr inline KeyModifier operator|(KeyModifier lhs, KeyModifier rhs) noexcept {
	return KeyModifier{libv::to_value(lhs) | libv::to_value(rhs)};
}

[[nodiscard]] constexpr inline KeyModifier operator&(KeyModifier lhs, KeyModifier rhs) noexcept {
	return KeyModifier{libv::to_value(lhs) & libv::to_value(rhs)};
}

enum class MouseButton : int32_t {
	Button0 = 0,
	Button1 = 1,
	Button2 = 2,
	Button3 = 3,
	Button4 = 4,
	Button5 = 5,
	Button6 = 6,
	Button7 = 7,
	Button8 = 8,
	Button9 = 9,
	Button10 = 10,
	Button11 = 11,
	Button12 = 12,
	Button13 = 13,
	Button14 = 14,
	Button15 = 15,
	Button16 = 16,
	Button17 = 17,
	Button18 = 18,
	Button19 = 19,
	Button20 = 20,
	Button21 = 21,
	Button22 = 22,
	Button23 = 23,
	Button24 = 24,
	Button25 = 25,
	Button26 = 26,
	Button27 = 27,
	Button28 = 28,
	Button29 = 29,
	Button30 = 30,
	Button31 = 31,
	Button32 = 32,
	Button33 = 33,
	Button34 = 34,
	Button35 = 35,
	Button36 = 36,
	Button37 = 37,
	Button38 = 38,
	Button39 = 39,
	Button40 = 40,
	Button41 = 41,
	Button42 = 42,
	Button43 = 43,
	Button44 = 44,
	Button45 = 45,
	Button46 = 46,
	Button47 = 47,
	Button48 = 48,
	Button49 = 49,
	Button50 = 50,
	Button51 = 51,
	Button52 = 52,
	Button53 = 53,
	Button54 = 54,
	Button55 = 55,
	Button56 = 56,
	Button57 = 57,
	Button58 = 58,
	Button59 = 59,
	Button60 = 60,
	Button61 = 61,
	Button62 = 62,
	Button63 = 63,

	Left = Button0,
	Right = Button1,
	Middle = Button2,
};

enum class GamepadID : uint8_t {
	any = 255,
};

enum class GamepadAnalogID : uint8_t {
};

enum class GamepadButton : int32_t {
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	LeftBumper = 4,
	RightBumper = 5,
	Back = 6,
	Start = 7,
	Guide = 8,
	LeftThumb = 9,
	RightThumb = 10,
	DPadUp = 11,
	DPadRight = 12,
	DPadDown = 13,
	DPadLeft = 14,

	Cross = A,
	Circle = B,
	Square = X,
	Triangle = Y,
};

enum class JoystickID : uint8_t {
	any = 255,
};

enum class JoystickAnalogID : uint8_t {
};

enum class JoystickButton : int32_t {
	Button0 = 0,
	Button1 = 1,
	Button2 = 2,
	Button3 = 3,
	Button4 = 4,
	Button5 = 5,
	Button6 = 6,
	Button7 = 7,
	Button8 = 8,
	Button9 = 9,
	Button10 = 10,
	Button11 = 11,
	Button12 = 12,
	Button13 = 13,
	Button14 = 14,
	Button15 = 15,
};

enum class Keycode : int32_t {
	Unknown = -1,
	Space = 32,
	Apostrophe = 39, /// '
	Comma = 44, /// ,
	Minus = 45, /// -
	Period = 46, /// .
	Slash = 47, /// /
	Num0 = 48,
	Num1 = 49,
	Num2 = 50,
	Num3 = 51,
	Num4 = 52,
	Num5 = 53,
	Num6 = 54,
	Num7 = 55,
	Num8 = 56,
	Num9 = 57,
	Semicolon = 59, /// ;
	Equals = 61, /// =
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	BracketOpen = 91, /// [
	Backslash = 92, /// \ .
	BracketClose = 93, /// ]
	Backtick = 96, /// `
	World1 = 161, /// non-US #1
	World2 = 162, /// non-US #2
	Escape = 256,
	Enter = 257,
	Tab = 258,
	Backspace = 259,
	Insert = 260,
	Delete = 261,
	Right = 262,
	Left = 263,
	Down = 264,
	Up = 265,
	PageUp = 266,
	PageDown = 267,
	Home = 268,
	End = 269,
	CapsLock = 280,
	ScrollLock = 281,
	NumLock = 282,
	PrintScreen = 283,
	Pause = 284,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,
	F16 = 305,
	F17 = 306,
	F18 = 307,
	F19 = 308,
	F20 = 309,
	F21 = 310,
	F22 = 311,
	F23 = 312,
	F24 = 313,
	F25 = 314,
	KPNum0 = 320,
	KPNum1 = 321,
	KPNum2 = 322,
	KPNum3 = 323,
	KPNum4 = 324,
	KPNum5 = 325,
	KPNum6 = 326,
	KPNum7 = 327,
	KPNum8 = 328,
	KPNum9 = 329,
	KPDot = 330,
	KPSlash = 331,
	KPAsterisk = 332,
	KPMinus = 333,
	KPPlus = 334,
	KPEnter = 335,
	KPEqual = 336,
	ShiftLeft = 340,
	ControlLeft = 341,
	AltLeft = 342,
	SuperLeft = 343,
	ShiftRight = 344,
	ControlRight = 345,
	AltRight = 346,
	SuperRight = 347,
	Menu = 348,
};

enum class Scancode	: int32_t {};

std::string_view to_string(const Action value);
std::string_view to_string(const GamepadButton value);
std::string_view to_string(const Keycode value);
std::string_view to_string(const KeyState value);
std::string_view to_string(const MonitorEvent value);
std::string_view to_string(const MouseButton value);

// -------------------------------------------------------------------------------------------------

} // namespace input
} // namespace libv
