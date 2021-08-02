// Project: libv.ctrl, File: src/libv/ctrl/parse.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ctrl/parse.hpp>
// ext
#include <boost/fusion/include/at_c.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/char/unicode.hpp>
// libv
#include <libv/utility/concat.hpp>
#include <libv/utility/utf8.hpp>
// std
#include <stdexcept>
// pro
#include <libv/ctrl/combination.hpp>
#include <libv/ctrl/enum.hpp>
#include <libv/ctrl/input.hpp>
#include <libv/ctrl/input_id.hpp>
#include <libv/ctrl/sequence.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

namespace {

namespace x3 = boost::spirit::x3;

// -------------------------------------------------------------------------------------------------

const auto fa_codepoint = [](auto& ctx) {
	auto it = _attr(ctx).begin();
	const auto end = _attr(ctx).end();
	uint32_t unicode = 0;

	auto valid = libv::next_unicode(it, end, unicode);
	valid = valid && it == end;

	_pass(ctx) = valid;
	_val(ctx) = InputID(Codepoint{unicode});
};

const auto fa_codepoint_esc = [](auto& ctx) {
	_val(ctx) = InputID(_attr(ctx));
};

const auto fa_scancode = [](auto& ctx) {
	_val(ctx) = InputID(Scancode{_attr(ctx)});
};

const auto fa_keycode = [](auto& ctx) {
	_val(ctx) = InputID(_attr(ctx));
};

const auto fa_modifier = [](auto& ctx) {
	_val(ctx) = InputID(Modifier{_attr(ctx)});
};

const auto fa_mouse_button = [](auto& ctx) {
	_val(ctx) = InputID(MouseButton{_attr(ctx)});
};

const auto fa_mouse_movement = [](auto& ctx) {
	_val(ctx) = InputID(MouseMovement{_attr(ctx)});
};

const auto fa_mouse_scroll = [](auto& ctx) {
	_val(ctx) = InputID(MouseScroll{_attr(ctx)});
};

const auto fa_gamepad_button_number = [](auto& ctx) {
	_val(ctx) = GamepadButton{_attr(ctx)};
};

const auto fa_gamepad_button = [](auto& ctx) {
	const uint8_t gamepadID = boost::fusion::at_c<0>(_attr(ctx));
	const GamepadButton button = boost::fusion::at_c<1>(_attr(ctx));

	_val(ctx) = InputID(GamepadID{gamepadID}, button);
};

const auto fa_gamepad_any_button = [](auto& ctx) {
	_val(ctx) = InputID(GamepadID::any, _attr(ctx));
};

const auto fa_gamepad_analog = [](auto& ctx) {
	const uint8_t gamepadID = boost::fusion::at_c<0>(_attr(ctx));
	const uint8_t analogID = boost::fusion::at_c<1>(_attr(ctx));
	const AnalogDimension dimension = boost::fusion::at_c<2>(_attr(ctx));

	_val(ctx) = InputID(GamepadID{gamepadID}, GamepadAnalogID{analogID}, dimension);
};

const auto fa_gamepad_any_analog = [](auto& ctx) {
	const uint8_t analogID = boost::fusion::at_c<0>(_attr(ctx));
	const AnalogDimension dimension = boost::fusion::at_c<1>(_attr(ctx));

	_val(ctx) = InputID(GamepadID::any, GamepadAnalogID{analogID}, dimension);
};

const auto fa_joystick_button = [](auto& ctx) {
	const uint8_t joystickID = boost::fusion::at_c<0>(_attr(ctx));
	const int32_t buttonID = boost::fusion::at_c<1>(_attr(ctx));

	_val(ctx) = InputID(JoystickID{joystickID}, JoystickButton{buttonID});
};

const auto fa_joystick_any_button = [](auto& ctx) {
	_val(ctx) = InputID(JoystickID::any, JoystickButton{_attr(ctx)});
};

const auto fa_joystick_analog = [](auto& ctx) {
	const uint8_t joystickID = boost::fusion::at_c<0>(_attr(ctx));
	const uint8_t analogID = boost::fusion::at_c<1>(_attr(ctx));
	const AnalogDimension dimension = boost::fusion::at_c<2>(_attr(ctx));

	_val(ctx) = InputID(JoystickID{joystickID}, JoystickAnalogID{analogID}, dimension);
};

const auto fa_joystick_any_analog = [](auto& ctx) {
	const uint8_t analogID = boost::fusion::at_c<0>(_attr(ctx));
	const AnalogDimension dimension = boost::fusion::at_c<1>(_attr(ctx));

	_val(ctx) = InputID(JoystickID::any, JoystickAnalogID{analogID}, dimension);
};

const auto fa_input = [](auto& ctx) {
	const InputID inputID = boost::fusion::at_c<0>(_attr(ctx));
	const auto dia = boost::fusion::at_c<1>(_attr(ctx));

	if (inputID.is_analog() && dia)
		_pass(ctx) = false;
	else
		_val(ctx) = Input{inputID, dia ? *dia : DigitalInputAction::auto_};
};

// -------------------------------------------------------------------------------------------------

const auto word_button = x3::rule<class word_button_, x3::unused_type>{} =
		x3::lit("button") | x3::lit("btn") | x3::lit("bt") | x3::lit("b");

const auto word_mouse = x3::rule<class word_mouse_, x3::unused_type>{} =
		x3::lit("mouse") | x3::lit("m");

const auto word_mouse_wheel = x3::rule<class word_mouse_wheel_, x3::unused_type>{} =
		x3::lit("scroll") | x3::lit("wheel") | x3::lit("s") | x3::lit("w");

const auto word_mouse_wheel_long = x3::rule<class word_mouse_wheel_long_, x3::unused_type>{} =
		x3::lit("scroll") | x3::lit("wheel");

const auto word_gamepad = x3::rule<class word_joystick_, x3::unused_type>{} =
		x3::lit("gamepad") | x3::lit("gp") | x3::lit("g");

const auto word_joystick = x3::rule<class word_joystick_, x3::unused_type>{} =
		x3::lit("joystick") | x3::lit("js") | x3::lit("j");

const auto word_analog = x3::rule<class word_analog_, x3::unused_type>{} =
		x3::lit("analog") | x3::lit("an") | x3::lit("a");

const auto word_lock = x3::rule<class word_lock_, x3::unused_type>{} =
		x3::lit("lock") | x3::lit("lck") | x3::lit("lk");

const auto word_x = x3::rule<class word_x_, x3::unused_type>{} =
		x3::lit("horizontal") | x3::lit("hori") | x3::lit("h") | x3::lit("x");

const auto word_y = x3::rule<class word_y_, x3::unused_type>{} =
		x3::lit("vertical") | x3::lit("vert") | x3::lit("v") | x3::lit("y");

const auto word_plus = x3::rule<class word_plus_, x3::unused_type>{} =
//		x3::lit("positive") | x3::lit("plus") | x3::lit("p") | x3::lit("+"); // p would miss m as counter part
		x3::lit("positive") | x3::lit("plus") | x3::lit("+");

const auto word_minus = x3::rule<class word_minus_, x3::unused_type>{} =
//		x3::lit("negative") | x3::lit("minus") | x3::lit("m") | x3::lit("-"); // m would collide with mouse
		x3::lit("negative") | x3::lit("minus") | x3::lit("-");

const auto word_left = x3::rule<class word_left_, x3::unused_type>{} =
		x3::lit("left") | x3::lit("l");

const auto word_right = x3::rule<class word_right_, x3::unused_type>{} =
		x3::lit("right") | x3::lit("r");

const auto word_middle = x3::rule<class word_middle_, x3::unused_type>{} =
		x3::lit("middle") | x3::lit("m");

const auto word_kp = x3::rule<class word_kp_, x3::unused_type>{} =
		x3::lit("numpad") | x3::lit("keypad") | x3::lit("kp");

const auto word_thumb = x3::rule<class word_middle_, x3::unused_type>{} =
		x3::lit("thumb") | x3::lit("t");

const auto word_bumper = x3::rule<class word_middle_, x3::unused_type>{} =
		x3::lit("bumper") | x3::lit("b");

const auto word_dpad = x3::rule<class word_middle_, x3::unused_type>{} =
		x3::lit("dpad") | x3::lit("dp") | x3::lit("d");

// -------------------------------------------------------------------------------------------------

struct key_named_ : x3::symbols<Keycode> { key_named_() { add
	("unknown",      Keycode::Unknown)
	("space",        Keycode::Space)
	("apostrophe",   Keycode::Apostrophe)
	("'",            Keycode::Apostrophe)
	("comma",        Keycode::Comma)
	(",",            Keycode::Comma)
	("minus",        Keycode::Minus)
	("-",            Keycode::Minus)
	// <editor-fold defaultstate="collapsed" desc="...">
	("period",       Keycode::Period)
	(".",            Keycode::Period)
	("slash",        Keycode::Slash)
	("/",            Keycode::Slash)
	("semicolon",    Keycode::Semicolon)
	(";",            Keycode::Semicolon)
	("equals",       Keycode::Equals)
	("=",            Keycode::Equals)
	("a",            Keycode::A)
	("b",            Keycode::B)
	("c",            Keycode::C)
	("d",            Keycode::D)
	("e",            Keycode::E)
	("f",            Keycode::F)
	("g",            Keycode::G)
	("h",            Keycode::H)
	("i",            Keycode::I)
	("j",            Keycode::J)
	("k",            Keycode::K)
	("l",            Keycode::L)
	("m",            Keycode::M)
	("n",            Keycode::N)
	("o",            Keycode::O)
	("p",            Keycode::P)
	("q",            Keycode::Q)
	("r",            Keycode::R)
	("s",            Keycode::S)
	("t",            Keycode::T)
	("u",            Keycode::U)
	("v",            Keycode::V)
	("w",            Keycode::W)
	("x",            Keycode::X)
	("y",            Keycode::Y)
	("z",            Keycode::Z)
	("[",            Keycode::BracketOpen)
	("backslash",    Keycode::Backslash)
	("\\",           Keycode::Backslash)
	("]",            Keycode::BracketClose)
	("backtick",     Keycode::Backtick)
	("`",            Keycode::Backtick)
	("world1",       Keycode::World1)
	("w1",           Keycode::World1)
	("world2",       Keycode::World2)
	("w2",           Keycode::World2)
	("escape",       Keycode::Escape)
	("esc",          Keycode::Escape)
	("enter",        Keycode::Enter)
	("tab",          Keycode::Tab)
	("backspace",    Keycode::Backspace)
	("insert",       Keycode::Insert)
	("delete",       Keycode::Delete)
	("right",        Keycode::Right)
	("left",         Keycode::Left)
	("down",         Keycode::Down)
	("up",           Keycode::Up)
	("home",         Keycode::Home)
	("end",          Keycode::End)
	("pause",        Keycode::Pause)
	("f1",           Keycode::F1)
	("f2",           Keycode::F2)
	("f3",           Keycode::F3)
	("f4",           Keycode::F4)
	("f5",           Keycode::F5)
	("f6",           Keycode::F6)
	("f7",           Keycode::F7)
	("f8",           Keycode::F8)
	("f9",           Keycode::F9)
	("f10",          Keycode::F10)
	("f11",          Keycode::F11)
	("f12",          Keycode::F12)
	("f13",          Keycode::F13)
	("f14",          Keycode::F14)
	("f15",          Keycode::F15)
	("f16",          Keycode::F16)
	("f17",          Keycode::F17)
	("f18",          Keycode::F18)
	("f19",          Keycode::F19)
	("f20",          Keycode::F20)
	("f21",          Keycode::F21)
	("f22",          Keycode::F22)
	("f23",          Keycode::F23)
	("f24",          Keycode::F24)
	("f25",          Keycode::F25)
	("menu",         Keycode::Menu)
	// </editor-fold>

	// extra aliases:
	("dash",         Keycode::Minus)
	("hyphen",       Keycode::Minus)
; }	} key_named;

const auto key_kp_base = x3::rule<class key_kp_base_, Keycode>{} =
		-x3::lit("num") >> x3::lit("0") >> x3::attr(Keycode::KPNum0) |
		-x3::lit("num") >> x3::lit("1") >> x3::attr(Keycode::KPNum1) |
		-x3::lit("num") >> x3::lit("2") >> x3::attr(Keycode::KPNum2) |
		-x3::lit("num") >> x3::lit("3") >> x3::attr(Keycode::KPNum3) |
		-x3::lit("num") >> x3::lit("4") >> x3::attr(Keycode::KPNum4) |
		-x3::lit("num") >> x3::lit("5") >> x3::attr(Keycode::KPNum5) |
		-x3::lit("num") >> x3::lit("6") >> x3::attr(Keycode::KPNum6) |
		-x3::lit("num") >> x3::lit("7") >> x3::attr(Keycode::KPNum7) |
		-x3::lit("num") >> x3::lit("8") >> x3::attr(Keycode::KPNum8) |
		-x3::lit("num") >> x3::lit("9") >> x3::attr(Keycode::KPNum9) |
		(x3::lit(".") | x3::lit("dot")) >> x3::attr(Keycode::KPDot) |
		(x3::lit("/") | x3::lit("slash")) >> x3::attr(Keycode::KPSlash) |
		(x3::lit("*") | x3::lit("asterisk")) >> x3::attr(Keycode::KPAsterisk) |
		(x3::lit("-") | x3::lit("minus")) >> x3::attr(Keycode::KPMinus) |
		(x3::lit("+") | x3::lit("plus")) >> x3::attr(Keycode::KPPlus) |
		(x3::lit("enter") >> x3::attr(Keycode::KPEnter)) |
		(x3::lit("=") | x3::lit("equal")) >> x3::attr(Keycode::KPEqual);

const auto key_num = x3::rule<class key_num_, Keycode>{} =
		-x3::lit("num") >> x3::lit("0") >> x3::attr(Keycode::Num0) |
		-x3::lit("num") >> x3::lit("1") >> x3::attr(Keycode::Num1) |
		-x3::lit("num") >> x3::lit("2") >> x3::attr(Keycode::Num2) |
		-x3::lit("num") >> x3::lit("3") >> x3::attr(Keycode::Num3) |
		-x3::lit("num") >> x3::lit("4") >> x3::attr(Keycode::Num4) |
		-x3::lit("num") >> x3::lit("5") >> x3::attr(Keycode::Num5) |
		-x3::lit("num") >> x3::lit("6") >> x3::attr(Keycode::Num6) |
		-x3::lit("num") >> x3::lit("7") >> x3::attr(Keycode::Num7) |
		-x3::lit("num") >> x3::lit("8") >> x3::attr(Keycode::Num8) |
		-x3::lit("num") >> x3::lit("9") >> x3::attr(Keycode::Num9);

const auto key_kp = x3::rule<class key_kp_, Keycode>{} =
		word_kp >> key_kp_base;

const auto key_pg = x3::rule<class key_pg_, Keycode>{} =
		(
			x3::lit("pg") | x3::lit("page")
		) >> (
			(x3::lit("up") >> x3::attr(Keycode::PageUp)) |
			(x3::lit("down") >> x3::attr(Keycode::PageDown))
		);

const auto key_sided_left = x3::rule<class key_sided_left_, Keycode>{} =
		(x3::lit("alt") >> x3::attr(Keycode::AltLeft)) |
		(x3::lit("control") >> x3::attr(Keycode::ControlLeft)) |
		(x3::lit("ctrl") >> x3::attr(Keycode::ControlLeft)) |
		(x3::lit("shft") >> x3::attr(Keycode::ShiftLeft)) |
		(x3::lit("shift") >> x3::attr(Keycode::ShiftLeft)) |
		(x3::lit("super") >> x3::attr(Keycode::SuperLeft));

const auto key_sided_right = x3::rule<class key_sided_right_, Keycode>{} =
		(x3::lit("alt") >> x3::attr(Keycode::AltRight)) |
		(x3::lit("control") >> x3::attr(Keycode::ControlRight)) |
		(x3::lit("ctrl") >> x3::attr(Keycode::ControlRight)) |
		(x3::lit("shft") >> x3::attr(Keycode::ShiftRight)) |
		(x3::lit("shift") >> x3::attr(Keycode::ShiftRight)) |
		(x3::lit("super") >> x3::attr(Keycode::SuperRight));

const auto key_sided = x3::rule<class key_sided_, Keycode>{} =
		word_left >> key_sided_left |
		word_right >> key_sided_right |
		key_sided_left >> word_left |
		key_sided_right >> word_right;

const auto key_multi_word = x3::rule<class key_multi_word_, Keycode>{} =
		x3::lit("bracket") >> x3::lit("open") >> x3::attr(Keycode::BracketOpen) |
		x3::lit("bracket") >> x3::lit("close") >> x3::attr(Keycode::BracketClose) |
		x3::lit("open") >> x3::lit("bracket") >> x3::attr(Keycode::BracketOpen) |
		x3::lit("close") >> x3::lit("bracket") >> x3::attr(Keycode::BracketClose) |
		x3::lit("caps") >>   word_lock >> x3::attr(Keycode::CapsLock) |
		x3::lit("scroll") >> word_lock >> x3::attr(Keycode::ScrollLock) |
		x3::lit("num") >>    word_lock >> x3::attr(Keycode::NumLock) |
		(x3::lit("print") | x3::lit("prt")) >> (x3::lit("screen") | x3::lit("scn") | x3::lit("sc")) >> x3::attr(Keycode::PrintScreen) |
		x3::lit("alt") >> x3::lit("gr") >> x3::attr(Keycode::AltRight);

// -------------------------------------------------------------------------------------------------

struct modifier_named_ : x3::symbols<Modifier> { modifier_named_() { add
	("alt",       Modifier::alt)
	("control",   Modifier::control)
	("ctrl",      Modifier::control)
	("shift",     Modifier::shift)
	("shft",      Modifier::shift)
	("super",     Modifier::super)
	("sper",      Modifier::super)
; }	} modifier_named;

// -------------------------------------------------------------------------------------------------

const auto analog_dimension_rule = x3::rule<class analog_dimension_rule_, AnalogDimension>{} =
		word_x >> (
				word_plus >> x3::attr(AnalogDimension::x_plus) |
				word_minus >> x3::attr(AnalogDimension::x_minus) |
				x3::attr(AnalogDimension::x)
		) |
		word_y >> (
				word_plus >> x3::attr(AnalogDimension::y_plus) |
				word_minus >> x3::attr(AnalogDimension::y_minus) |
				x3::attr(AnalogDimension::y)
		);

struct gamepad_button_named_ : x3::symbols<GamepadButton> { gamepad_button_named_() { add
	("a",        GamepadButton::A)
	("cross",    GamepadButton::A)
	("b",        GamepadButton::B)
	("circle",   GamepadButton::B)
	("x",        GamepadButton::X)
	("square",   GamepadButton::X)
	("y",        GamepadButton::Y)
	("triangle", GamepadButton::Y)
	("back",     GamepadButton::Back)
	("start",    GamepadButton::Start)
	("guide",    GamepadButton::Guide)
; }	} gamepad_button_named;

const auto gamepad_button_number = x3::rule<class gamepad_button_number_, GamepadButton>{} =
		x3::int32[fa_gamepad_button_number];

const auto aux_gamepad_button = x3::rule<class aux_gamepad_button_, GamepadButton>{} =
		word_left >> word_bumper >> x3::attr(GamepadButton::LeftBumper) |
		word_bumper >> word_left >> x3::attr(GamepadButton::LeftBumper) |
		word_right >> word_bumper >> x3::attr(GamepadButton::RightBumper) |
		word_bumper >> word_right >> x3::attr(GamepadButton::RightBumper) |

		word_left >> word_thumb >> x3::attr(GamepadButton::LeftThumb) |
		word_thumb >> word_left >> x3::attr(GamepadButton::LeftThumb) |
		word_right >> word_thumb >> x3::attr(GamepadButton::RightThumb) |
		word_thumb >> word_right >> x3::attr(GamepadButton::RightThumb) |

		word_dpad >> x3::lit("up") >> x3::attr(GamepadButton::DPadUp) |
		word_dpad >> x3::lit("right") >> x3::attr(GamepadButton::DPadRight) |
		word_dpad >> x3::lit("down") >> x3::attr(GamepadButton::DPadDown) |
		word_dpad >> x3::lit("left") >> x3::attr(GamepadButton::DPadLeft) |

		gamepad_button_named |
		gamepad_button_number;

// -------------------------------------------------------------------------------------------------

const auto gamepad_button_rule = x3::rule<class gamepad_button_rule_, InputID>{} =
		(
			word_gamepad >> x3::uint8 >>
			word_button >> aux_gamepad_button
		)[fa_gamepad_button];

const auto gamepad_any_button_rule = x3::rule<class gamepad_any_button_rule_, InputID>{} =
		(
			word_gamepad >>
			word_button >> aux_gamepad_button
		)[fa_gamepad_any_button];

const auto gamepad_analog_rule = x3::rule<class gamepad_analog_rule_, InputID>{} =
		(
			word_gamepad >> x3::uint8 >>
			word_analog >> x3::uint8 >>
			x3::lit(":") >> analog_dimension_rule
		)[fa_gamepad_analog];

const auto gamepad_any_analog_rule = x3::rule<class gamepad_any_analog_rule_, InputID>{} =
		(
			word_gamepad >>
			word_analog >> x3::uint8 >>
			x3::lit(":") >> analog_dimension_rule
		)[fa_gamepad_any_analog];

// -------------------------------------------------------------------------------------------------

const auto joystick_button_rule = x3::rule<class joystick_button_rule_, InputID>{} =
		(
			word_joystick >> x3::uint8 >>
			word_button >> x3::int32
		)[fa_joystick_button];

const auto joystick_any_button_rule = x3::rule<class joystick_any_button_rule_, InputID>{} =
		(
			word_joystick >>
			word_button >> x3::int32
		)[fa_joystick_any_button];

const auto joystick_analog_rule = x3::rule<class joystick_analog_rule_, InputID>{} =
		(
			word_joystick >> x3::uint8 >>
			word_analog >> x3::uint8 >>
			x3::lit(":") >> analog_dimension_rule
		)[fa_joystick_analog];

const auto joystick_any_analog_rule = x3::rule<class joystick_any_analog_rule_, InputID>{} =
		(
			word_joystick >>
			word_analog >> x3::uint8 >>
			x3::lit(":") >> analog_dimension_rule
		)[fa_joystick_any_analog];

// -------------------------------------------------------------------------------------------------

const auto mouse_move_rule = x3::rule<class mouse_move_rule_, InputID>{} =
		word_mouse >> word_x >> (
				word_plus >> x3::attr(MouseMovement::x_plus)[fa_mouse_movement] |
				word_minus >> x3::attr(MouseMovement::x_minus)[fa_mouse_movement] |
				x3::attr(MouseMovement::x)[fa_mouse_movement]
		) |
		word_mouse >> word_y >> (
				word_plus >> x3::attr(MouseMovement::y_plus)[fa_mouse_movement] |
				word_minus >> x3::attr(MouseMovement::y_minus)[fa_mouse_movement] |
				x3::attr(MouseMovement::y)[fa_mouse_movement]
		);

const auto mouse_scroll_rule = x3::rule<class mouse_scroll_rule_, InputID>{} =
		(word_mouse >> word_mouse_wheel | word_mouse_wheel_long) >> word_x >> (
				word_plus >> x3::attr(MouseScroll::x_plus)[fa_mouse_scroll] |
				word_minus >> x3::attr(MouseScroll::x_minus)[fa_mouse_scroll] |
				x3::attr(MouseScroll::x)[fa_mouse_scroll]
		) >> x3::eoi |
		(word_mouse >> word_mouse_wheel | word_mouse_wheel_long) >> -word_y >> (
				word_plus >> x3::attr(MouseScroll::y_plus)[fa_mouse_scroll] |
				word_minus >> x3::attr(MouseScroll::y_minus)[fa_mouse_scroll] |
				x3::attr(MouseScroll::y)[fa_mouse_scroll]
		) >> x3::eoi;

const auto mouse_button_rule = x3::rule<class mouse_button_rule_, InputID>{} =
		word_left   >> word_mouse >> -word_button >> x3::attr(MouseButton::Left  )[fa_mouse_button] |
		word_right  >> word_mouse >> -word_button >> x3::attr(MouseButton::Right )[fa_mouse_button] |
		word_middle >> word_mouse >> -word_button >> x3::attr(MouseButton::Middle)[fa_mouse_button] |
		word_mouse >> -word_button >> word_left   >> x3::attr(MouseButton::Left  )[fa_mouse_button] |
		word_mouse >> -word_button >> word_right  >> x3::attr(MouseButton::Right )[fa_mouse_button] |
		word_mouse >> -word_button >> word_middle >> x3::attr(MouseButton::Middle)[fa_mouse_button] |
		x3::int32[fa_mouse_button] >> word_mouse >> -word_button |
		word_mouse >> -word_button >> x3::int32[fa_mouse_button];

const auto codepoint_rule = x3::rule<class codepoint_rule_, InputID>{} =
		x3::lit("\"\\\"\"") >> x3::attr(Codepoint{'"'})[fa_codepoint_esc] |
		x3::lit("\"\\\'\"") >> x3::attr(Codepoint{'\''})[fa_codepoint_esc] |
		x3::lit("\'\\\"\'") >> x3::attr(Codepoint{'"'})[fa_codepoint_esc] |
		x3::lit("\'\\\'\'") >> x3::attr(Codepoint{'\''})[fa_codepoint_esc] |
		x3::lexeme[x3::lit("\"") >> (*~x3::char_('\"'))[fa_codepoint] >> x3::lit("\"")] |
		x3::lexeme[x3::lit("'") >> (*~x3::char_('\''))[fa_codepoint] >> x3::lit("'")];

const auto scancode_rule = x3::rule<class scancode_rule_, InputID>{} =
		(x3::lit("scancode") | x3::lit("scan") | x3::lit("sc")) >> x3::int32[fa_scancode];

const auto keycode_or_modifier_rule = x3::rule<class keycode_or_modifier_rule_, InputID>{} =
		key_num[fa_keycode] |
		key_kp[fa_keycode] |
		key_pg[fa_keycode] |
		key_multi_word[fa_keycode] |
		key_sided[fa_keycode] |
		modifier_named[fa_modifier] | // NOTE: modifier_named has to be after key_sided and before key_named
		key_named[fa_keycode];

// -------------------------------------------------------------------------------------------------

const auto input_id_rule = x3::rule<class input_id_rule_, InputID>{} =
		gamepad_any_analog_rule |
		gamepad_analog_rule |
		gamepad_any_button_rule |
		gamepad_button_rule |
		joystick_any_analog_rule |
		joystick_analog_rule |
		joystick_any_button_rule |
		joystick_button_rule |
		mouse_move_rule |
		mouse_scroll_rule |
		mouse_button_rule |
		scancode_rule |
		codepoint_rule |
		keycode_or_modifier_rule;

// -------------------------------------------------------------------------------------------------

const auto dia_rule = x3::rule<class dia_rule_, DigitalInputAction>{} =
		x3::lit("[") >> (
			x3::lit("auto") >> x3::attr(DigitalInputAction::auto_) |
			x3::lit("press") >> x3::attr(DigitalInputAction::press) |
			x3::lit("repeat") >> x3::attr(DigitalInputAction::repeat) |
			x3::lit("release") >> x3::attr(DigitalInputAction::release) |
			x3::lit("any") >> x3::attr(DigitalInputAction::any) |
			x3::lit("hold") >> x3::attr(DigitalInputAction::hold) |
			x3::lit("free") >> x3::attr(DigitalInputAction::free)
		) >> x3::lit("]");

const auto input_rule = x3::rule<class input_rule_, Input>{} =
		(input_id_rule >> -dia_rule)[fa_input];

// -------------------------------------------------------------------------------------------------

} // namespace

// =================================================================================================

InputID parse_input_id_or(const std::string_view str, const InputID& fallback) {
	const auto result = parse_input_id_optional(str);
	return result ? *result : fallback;
}

InputID parse_input_id_or_throw(const std::string_view str) {
	const auto result = parse_input_id_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat("\"", str, "\" is not a valid InputID"));
}

std::optional<InputID> parse_input_id_optional(const std::string_view str) {
	InputID result;

	auto it = str.begin();
	// NOTE: x3::unicode::space is required for certain invalid UTF8 inputs as x3::space skipper would assert
	const auto passed = x3::phrase_parse(it, str.end(), x3::no_case[input_id_rule], x3::unicode::space, result);
	const auto success = passed && it == str.end();

	if (success)
		return result;
	else
		return std::nullopt;
}

// -------------------------------------------------------------------------------------------------

Input parse_input_or(const std::string_view str, const Input& fallback) {
	const auto result = parse_input_optional(str);
	return result ? *result : fallback;
}

Input parse_input_or_throw(const std::string_view str) {
	const auto result = parse_input_optional(str);
	if (result)
		return *result;
	else
		throw std::invalid_argument(libv::concat("\"", str, "\" is not a valid Input"));
}

std::optional<Input> parse_input_optional(const std::string_view str) {
	Input result;

	auto it = str.begin();
	// NOTE: x3::unicode::space is required for certain invalid UTF8 inputs as x3::space skipper would assert
	const auto passed = x3::phrase_parse(it, str.end(), x3::no_case[input_rule], x3::unicode::space, result);
	const auto success = passed && it == str.end();

	if (success)
		return result;
	else
		return std::nullopt;
}

// -------------------------------------------------------------------------------------------------

namespace {

static constexpr auto seek_delim = [](const auto begin, const auto end, const auto delim) {
	auto it = begin;
	bool skipped = false;

	while (true) {
		while (it != end && *it != delim)
			++it;

		if (it == end)
			break;

		auto it_before = it == begin ? it : it - 1;
		auto it_after = it + 1 == end ? it : it + 1;

		if (skipped || *it_before != '\'' || *it != delim || *it_after != '\'')
			break;

		// Special case hit, skip token and loop back around
		++it;
		skipped = true;
	}

	return it;
};

static constexpr auto next_delim = [](const auto begin, const auto end, const auto delim) {
	auto it = begin;
	while (it != end && *it != delim)
		++it;
	return it;
};

static constexpr auto sv = [](const auto begin, const auto end) {
	return std::string_view(&*begin, std::distance(begin, end));
};

} // namespace

// -------------------------------------------------------------------------------------------------

Combination parse_combination_or(const std::string_view str, const Combination& fallback) {
	auto result = parse_combination_optional(str);
	return result ? std::move(*result) : fallback;
}

Combination parse_combination_or_throw(const std::string_view str) {
	auto result = parse_combination_optional(str);
	if (result)
		return std::move(*result);
	else
		throw std::invalid_argument(libv::concat("\"", str, "\" is not a valid Combination"));
}

std::optional<Combination> parse_combination_optional(const std::string_view str) {
	//libv.ctrl: I think it is possible to improvement performance by around 15%
	//			The new parser is correct, but it often hits the failure case
	//
	//	With forward parsing (New)
	//	Try eager eval from the front and consume two token, then fallback to one
	//	w1_parse: 1529, w1_parse_fail: 30, w2_parse: 708, w2_parse_fail: 571
	//		27% Overhead and most of this is on longer strings
	//		But has no correctness issue
	//
	//	With backward parsing (Old) (pre 1ce9dee)
	//	Try lazy eval from the back consume one token, then fallback to two
	//	w1_parse: 1658, w1_parse_fail: 149, w2_parse: 128, w2_parse_fail: 15
	//		9% Overhead and most of this is on shorter strings
	//		Issue with:
	//				Apostrophe + "+" == "+" + Apostrophe
	//				Apostrophe + "+" + "+" == "+" + "+" + Apostrophe
	//				Apostrophe + "+" + A == "+" + Apostrophe + A

	boost::container::small_vector<Input, 3> inputs;

	const auto begin = str.begin();
	const auto end = str.end();
	auto it = begin;

	while (it != end) {
		const auto w1_begin = it;
		const auto w1_end = next_delim(w1_begin, end, '+');
		const auto w1 = sv(w1_begin, w1_end);

		if (w1_end != end) {
			const auto w2_begin = it;
			const auto w2_end = next_delim(std::next(w1_end), end, '+');
			const auto w2 = sv(w2_begin, w2_end);
			auto parsed_w2 = parse_input_optional(w2);

			if (parsed_w2) {
				inputs.emplace_back(*parsed_w2);
				if (w2_end == end)
					break;
				it = std::next(w2_end);
				if (it == end)
					return std::nullopt; // Skipping the token caused exhaustion, there is a missing segment
				continue;
			}
		}

		auto parsed_w1 = parse_input_optional(w1);

		if (parsed_w1) {
			inputs.emplace_back(*parsed_w1);
			if (w1_end == end)
				break;
			it = std::next(w1_end);
			if (it == end)
				return std::nullopt; // Skipping the token caused exhaustion, there is a missing segment
			continue;
		}

		return std::nullopt;
	}

	if (inputs.empty())
		return std::nullopt;

	// Fill result
	Combination result;
	for (auto& input : inputs)
		result.add_input(std::move(input));

	return result;
}

// -------------------------------------------------------------------------------------------------

Sequence parse_sequence_or(const std::string_view str, const Sequence& fallback) {
	auto result = parse_sequence_optional(str);
	return result ? std::move(*result) : fallback;
}

Sequence parse_sequence_or_throw(const std::string_view str) {
	auto result = parse_sequence_optional(str);
	if (result)
		return std::move(*result);
	else
		throw std::invalid_argument(libv::concat("\"", str, "\" is not a valid Sequence"));
}

std::optional<Sequence> parse_sequence_optional(const std::string_view str) {
	if (str.empty())
		return std::nullopt;

	Sequence result;

	// Forward iteration
	auto it = str.begin();
	while (it != str.end()) {
		const auto segment_begin = it;

		// First attempt parsing
		it = seek_delim(segment_begin, str.end(), ',');
		const auto window = sv(segment_begin, it);
		auto parsed_combination = parse_combination_optional(window);

		// Second attempt parsing with including the last delimiter as part of the segment
		if (!parsed_combination && it != str.end()) {
			it++;
			it = seek_delim(it, str.end(), ',');

			const auto fallback_window = sv(segment_begin, it);
			parsed_combination = parse_combination_optional(fallback_window);
		}

		if (parsed_combination)
			result.add_combination(std::move(*parsed_combination));
		else
			// Both first and second pass failed
			return std::nullopt;

		if (it != str.end()) {
			// Skip the used delimiter token
			it++;

			if (it == str.end())
				// Skipping the token caused exhaustion, there is a missing segment
				return std::nullopt;
		}
	}

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
