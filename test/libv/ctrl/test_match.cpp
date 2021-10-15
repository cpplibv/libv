// Project: libv.ctrl, File: test/libv/ctrl/test_match.cpp

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/ctrl/input_id.hpp>


// -------------------------------------------------------------------------------------------------

namespace lc = libv::ctrl;

TEST_CASE("InputID should match any aliases", "[libv.control.inputid]") {

	const auto test = [](const auto... lhs){
		return [=](const auto... rhs){
			return lc::alias_match(lc::InputID{lhs...}, lc::InputID{rhs...});
		};
	};

	// Invalid
	CHECK(test()());
	CHECK(not test()(lc::Keycode::A));
	CHECK(not test()(lc::Codepoint{'A'}));

	// Codepoint
	CHECK(test(lc::Codepoint{'A'})(lc::Codepoint{'A'}));
	CHECK(not test(lc::Codepoint{'A'})(lc::Codepoint{'B'}));

	// Keycode
	CHECK(test(lc::Keycode::A)(lc::Keycode::A));
	CHECK(test(lc::Keycode::SuperRight)(lc::Keycode::SuperRight));
	CHECK(not test(lc::Keycode::A)(lc::Keycode::B));
	CHECK(not test(lc::Keycode::SuperRight)(lc::Keycode::SuperLeft));
	CHECK(not test(lc::Keycode::ControlLeft)(lc::Keycode::ControlRight));
	CHECK(not test(lc::Keycode::ControlRight)(lc::Keycode::ControlLeft));
	CHECK(not test(lc::Keycode::ShiftLeft)(lc::Keycode::ShiftRight));
	CHECK(not test(lc::Keycode::ShiftRight)(lc::Keycode::ShiftLeft));
	CHECK(not test(lc::Keycode::AltLeft)(lc::Keycode::AltRight));
	CHECK(not test(lc::Keycode::AltRight)(lc::Keycode::AltLeft));
	CHECK(not test(lc::Keycode::SuperLeft)(lc::Keycode::SuperRight));
	CHECK(not test(lc::Keycode::SuperRight)(lc::Keycode::SuperLeft));

	// Scancode
	CHECK(test(lc::Scancode{150})(lc::Scancode{150}));
	CHECK(not test(lc::Scancode{150})(lc::Scancode{151}));

	// Modifier
	CHECK(test(lc::Keycode::ControlLeft)(lc::Keycode::ControlLeft));
	CHECK(test(lc::Keycode::ControlLeft)(lc::Modifier::control));
	CHECK(test(lc::Keycode::ControlRight)(lc::Keycode::ControlRight));
	CHECK(test(lc::Keycode::ControlRight)(lc::Modifier::control));
	CHECK(test(lc::Modifier::control)(lc::Keycode::ControlLeft));
	CHECK(test(lc::Modifier::control)(lc::Keycode::ControlRight));
	CHECK(test(lc::Modifier::control)(lc::Modifier::control));

	CHECK(test(lc::Keycode::ShiftLeft)(lc::Keycode::ShiftLeft));
	CHECK(test(lc::Keycode::ShiftLeft)(lc::Modifier::shift));
	CHECK(test(lc::Keycode::ShiftRight)(lc::Keycode::ShiftRight));
	CHECK(test(lc::Keycode::ShiftRight)(lc::Modifier::shift));
	CHECK(test(lc::Modifier::shift)(lc::Keycode::ShiftLeft));
	CHECK(test(lc::Modifier::shift)(lc::Keycode::ShiftRight));
	CHECK(test(lc::Modifier::shift)(lc::Modifier::shift));

	CHECK(test(lc::Keycode::AltLeft)(lc::Keycode::AltLeft));
	CHECK(test(lc::Keycode::AltLeft)(lc::Modifier::alt));
	CHECK(test(lc::Keycode::AltRight)(lc::Keycode::AltRight));
	CHECK(test(lc::Keycode::AltRight)(lc::Modifier::alt));
	CHECK(test(lc::Modifier::alt)(lc::Keycode::AltLeft));
	CHECK(test(lc::Modifier::alt)(lc::Keycode::AltRight));
	CHECK(test(lc::Modifier::alt)(lc::Modifier::alt));

	CHECK(test(lc::Keycode::SuperLeft)(lc::Keycode::SuperLeft));
	CHECK(test(lc::Keycode::SuperLeft)(lc::Modifier::super));
	CHECK(test(lc::Keycode::SuperRight)(lc::Keycode::SuperRight));
	CHECK(test(lc::Keycode::SuperRight)(lc::Modifier::super));
	CHECK(test(lc::Modifier::super)(lc::Keycode::SuperLeft));
	CHECK(test(lc::Modifier::super)(lc::Keycode::SuperRight));
	CHECK(test(lc::Modifier::super)(lc::Modifier::super));

	CHECK(not test(lc::Modifier::control)(lc::Modifier::alt));
	CHECK(not test(lc::Modifier::control)(lc::Keycode::AltRight));

	// MouseButton
	CHECK(test(lc::MouseButton::Button0)(lc::MouseButton::Left));
	CHECK(test(lc::MouseButton::Button1)(lc::MouseButton::Right));
	CHECK(test(lc::MouseButton::Button2)(lc::MouseButton::Middle));
	CHECK(not test(lc::MouseButton::Button0)(lc::MouseButton::Button1));

	// Mouse Movement
	CHECK(test(lc::MouseMovement::x)(lc::MouseMovement::x));
	CHECK(test(lc::MouseMovement::x)(lc::MouseMovement::x_minus));
	CHECK(test(lc::MouseMovement::x)(lc::MouseMovement::x_plus));
	CHECK(test(lc::MouseMovement::x_minus)(lc::MouseMovement::x));
	CHECK(test(lc::MouseMovement::x_minus)(lc::MouseMovement::x_minus));
	CHECK(test(lc::MouseMovement::x_plus)(lc::MouseMovement::x));
	CHECK(test(lc::MouseMovement::x_plus)(lc::MouseMovement::x_plus));
	CHECK(not test(lc::MouseMovement::x_minus)(lc::MouseMovement::x_plus));
	CHECK(not test(lc::MouseMovement::x_plus)(lc::MouseMovement::x_minus));

	// Mouse Scroll
	CHECK(test(lc::MouseScroll::x)(lc::MouseScroll::x));
	CHECK(test(lc::MouseScroll::x)(lc::MouseScroll::x_minus));
	CHECK(test(lc::MouseScroll::x)(lc::MouseScroll::x_plus));
	CHECK(test(lc::MouseScroll::x_minus)(lc::MouseScroll::x));
	CHECK(test(lc::MouseScroll::x_minus)(lc::MouseScroll::x_minus));
	CHECK(test(lc::MouseScroll::x_plus)(lc::MouseScroll::x));
	CHECK(test(lc::MouseScroll::x_plus)(lc::MouseScroll::x_plus));
	CHECK(not test(lc::MouseScroll::x_minus)(lc::MouseScroll::x_plus));
	CHECK(not test(lc::MouseScroll::x_plus)(lc::MouseScroll::x_minus));

	// Gamepad Button
	CHECK(test(lc::GamepadID{10}, lc::GamepadButton::A)(lc::GamepadID{10}, lc::GamepadButton::Cross));
	CHECK(test(lc::GamepadID{10}, lc::GamepadButton::B)(lc::GamepadID{10}, lc::GamepadButton::Circle));
	CHECK(test(lc::GamepadID{10}, lc::GamepadButton::X)(lc::GamepadID{10}, lc::GamepadButton::Square));
	CHECK(test(lc::GamepadID{10}, lc::GamepadButton::Y)(lc::GamepadID{10}, lc::GamepadButton::Triangle));

	CHECK(test(lc::GamepadID{10}, lc::GamepadButton::A)(lc::GamepadID{10}, lc::GamepadButton::A));
	CHECK(test(lc::GamepadID{10}, lc::GamepadButton::B)(lc::GamepadID{10}, lc::GamepadButton::B));
	CHECK(not test(lc::GamepadID{10}, lc::GamepadButton::A)(lc::GamepadID{10}, lc::GamepadButton::B));

	CHECK(not test(lc::GamepadID{11}, lc::GamepadButton::A)(lc::GamepadID{10}, lc::GamepadButton::A));
	CHECK(not test(lc::GamepadID{11}, lc::GamepadButton::B)(lc::GamepadID{10}, lc::GamepadButton::B));
	CHECK(not test(lc::GamepadID{11}, lc::GamepadButton::A)(lc::GamepadID{10}, lc::GamepadButton::B));

	CHECK(test(lc::GamepadID{10}, lc::GamepadButton::A)(lc::GamepadID::any, lc::GamepadButton::A));
	CHECK(test(lc::GamepadID{10}, lc::GamepadButton::B)(lc::GamepadID::any, lc::GamepadButton::B));
	CHECK(not test(lc::GamepadID{10}, lc::GamepadButton::A)(lc::GamepadID::any, lc::GamepadButton::B));

	CHECK(test(lc::GamepadID::any, lc::GamepadButton::A)(lc::GamepadID{10}, lc::GamepadButton::A));
	CHECK(test(lc::GamepadID::any, lc::GamepadButton::B)(lc::GamepadID{10}, lc::GamepadButton::B));
	CHECK(not test(lc::GamepadID::any, lc::GamepadButton::A)(lc::GamepadID{10}, lc::GamepadButton::B));

	CHECK(test(lc::GamepadID::any, lc::GamepadButton::A)(lc::GamepadID::any, lc::GamepadButton::A));
	CHECK(test(lc::GamepadID::any, lc::GamepadButton::B)(lc::GamepadID::any, lc::GamepadButton::B));
	CHECK(not test(lc::GamepadID::any, lc::GamepadButton::A)(lc::GamepadID::any, lc::GamepadButton::B));

	// Gamepad Analog
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));

	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{2}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{2}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{2}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{2}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{2}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{2}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{2}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{2}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{2}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));

	CHECK(not test(lc::GamepadID{8}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::GamepadID{8}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(not test(lc::GamepadID{8}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID{8}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::GamepadID{8}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(not test(lc::GamepadID{8}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::GamepadID{8}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID{8}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID{8}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));

	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));

	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID{7}, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));

	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_plus)(lc::GamepadID::any, lc::GamepadAnalogID{1}, lc::AnalogDimension::x_minus));

	// Joystick Button
	CHECK(test(lc::JoystickID{10}, lc::JoystickButton::Button3)(lc::JoystickID{10}, lc::JoystickButton::Button3));
	CHECK(test(lc::JoystickID{10}, lc::JoystickButton::Button4)(lc::JoystickID{10}, lc::JoystickButton::Button4));
	CHECK(not test(lc::JoystickID{10}, lc::JoystickButton::Button3)(lc::JoystickID{10}, lc::JoystickButton::Button4));

	CHECK(not test(lc::JoystickID{11}, lc::JoystickButton::Button3)(lc::JoystickID{10}, lc::JoystickButton::Button3));
	CHECK(not test(lc::JoystickID{11}, lc::JoystickButton::Button4)(lc::JoystickID{10}, lc::JoystickButton::Button4));
	CHECK(not test(lc::JoystickID{11}, lc::JoystickButton::Button3)(lc::JoystickID{10}, lc::JoystickButton::Button4));

	CHECK(test(lc::JoystickID{10}, lc::JoystickButton::Button3)(lc::JoystickID::any, lc::JoystickButton::Button3));
	CHECK(test(lc::JoystickID{10}, lc::JoystickButton::Button4)(lc::JoystickID::any, lc::JoystickButton::Button4));
	CHECK(not test(lc::JoystickID{10}, lc::JoystickButton::Button3)(lc::JoystickID::any, lc::JoystickButton::Button4));

	CHECK(test(lc::JoystickID::any, lc::JoystickButton::Button3)(lc::JoystickID{10}, lc::JoystickButton::Button3));
	CHECK(test(lc::JoystickID::any, lc::JoystickButton::Button4)(lc::JoystickID{10}, lc::JoystickButton::Button4));
	CHECK(not test(lc::JoystickID::any, lc::JoystickButton::Button3)(lc::JoystickID{10}, lc::JoystickButton::Button4));

	CHECK(test(lc::JoystickID::any, lc::JoystickButton::Button3)(lc::JoystickID::any, lc::JoystickButton::Button3));
	CHECK(test(lc::JoystickID::any, lc::JoystickButton::Button4)(lc::JoystickID::any, lc::JoystickButton::Button4));
	CHECK(not test(lc::JoystickID::any, lc::JoystickButton::Button3)(lc::JoystickID::any, lc::JoystickButton::Button4));

	// JoystickAnalog
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));

	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{2}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{2}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{2}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{2}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{2}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{2}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{2}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{2}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{2}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));

	CHECK(not test(lc::JoystickID{8}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::JoystickID{8}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(not test(lc::JoystickID{8}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID{8}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::JoystickID{8}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(not test(lc::JoystickID{8}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(not test(lc::JoystickID{8}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID{8}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID{8}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));

	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));

	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID{7}, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));

	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x));
	CHECK(test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus));
	CHECK(not test(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_plus)(lc::JoystickID::any, lc::JoystickAnalogID{1}, lc::AnalogDimension::x_minus));
}
