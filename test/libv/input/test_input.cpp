// Project: libv.input, File: test/libv/input/test_input.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/utility/to_underlying.hpp>
// ext
#include <GLFW/glfw3.h>
// pro
#include <libv/input/input.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("Check MonitorEvent consistency with GLFW") {
	CHECK(libv::to_value(libv::input::MonitorEvent::connected) == GLFW_CONNECTED);
	CHECK(libv::to_value(libv::input::MonitorEvent::disconnected) == GLFW_DISCONNECTED);
}

TEST_CASE("Check Action consistency with GLFW") {
	CHECK(libv::to_value(libv::input::Action::press) == GLFW_PRESS);
	CHECK(libv::to_value(libv::input::Action::release) == GLFW_RELEASE);
	CHECK(libv::to_value(libv::input::Action::repeat) == GLFW_REPEAT);
}

TEST_CASE("Check KeyState consistency with GLFW") {
	CHECK(libv::to_value(libv::input::KeyState::released) == GLFW_RELEASE);
	CHECK(libv::to_value(libv::input::KeyState::pressed) == GLFW_PRESS);
}

TEST_CASE("Check KeyModifier consistency with GLFW") {
	CHECK(libv::to_value(libv::input::KeyModifier::alt) == GLFW_MOD_ALT);
	CHECK(libv::to_value(libv::input::KeyModifier::control) == GLFW_MOD_CONTROL);
	CHECK(libv::to_value(libv::input::KeyModifier::shift) == GLFW_MOD_SHIFT);
	CHECK(libv::to_value(libv::input::KeyModifier::super) == GLFW_MOD_SUPER);
	CHECK(libv::to_value(libv::input::KeyModifier::caps_lock) == GLFW_MOD_CAPS_LOCK);
	CHECK(libv::to_value(libv::input::KeyModifier::num_lock) == GLFW_MOD_NUM_LOCK);
}

TEST_CASE("Check Mouse consistency with GLFW") {
	CHECK(libv::to_value(libv::input::MouseButton::Button0) == GLFW_MOUSE_BUTTON_1);
	CHECK(libv::to_value(libv::input::MouseButton::Button1) == GLFW_MOUSE_BUTTON_2);
	CHECK(libv::to_value(libv::input::MouseButton::Button2) == GLFW_MOUSE_BUTTON_3);
	CHECK(libv::to_value(libv::input::MouseButton::Button3) == GLFW_MOUSE_BUTTON_4);
	CHECK(libv::to_value(libv::input::MouseButton::Button4) == GLFW_MOUSE_BUTTON_5);
	CHECK(libv::to_value(libv::input::MouseButton::Button5) == GLFW_MOUSE_BUTTON_6);
	CHECK(libv::to_value(libv::input::MouseButton::Button6) == GLFW_MOUSE_BUTTON_7);
	CHECK(libv::to_value(libv::input::MouseButton::Button7) == GLFW_MOUSE_BUTTON_8);

	CHECK(libv::input::MouseButton::Left == libv::input::MouseButton::Button0);
	CHECK(libv::input::MouseButton::Right == libv::input::MouseButton::Button1);
	CHECK(libv::input::MouseButton::Middle == libv::input::MouseButton::Button2);
	CHECK(libv::to_value(libv::input::MouseButton::Left) == GLFW_MOUSE_BUTTON_LEFT);
	CHECK(libv::to_value(libv::input::MouseButton::Right) == GLFW_MOUSE_BUTTON_RIGHT);
	CHECK(libv::to_value(libv::input::MouseButton::Middle) == GLFW_MOUSE_BUTTON_MIDDLE);

	CHECK(libv::to_value(libv::input::MouseButton::Button7) == GLFW_MOUSE_BUTTON_LAST);
}

TEST_CASE("Check Joystick consistency with GLFW") {
	CHECK(libv::to_value(libv::input::JoystickButton::Button0) == GLFW_JOYSTICK_1);
	CHECK(libv::to_value(libv::input::JoystickButton::Button1) == GLFW_JOYSTICK_2);
	CHECK(libv::to_value(libv::input::JoystickButton::Button2) == GLFW_JOYSTICK_3);
	CHECK(libv::to_value(libv::input::JoystickButton::Button3) == GLFW_JOYSTICK_4);
	CHECK(libv::to_value(libv::input::JoystickButton::Button4) == GLFW_JOYSTICK_5);
	CHECK(libv::to_value(libv::input::JoystickButton::Button5) == GLFW_JOYSTICK_6);
	CHECK(libv::to_value(libv::input::JoystickButton::Button6) == GLFW_JOYSTICK_7);
	CHECK(libv::to_value(libv::input::JoystickButton::Button7) == GLFW_JOYSTICK_8);
	CHECK(libv::to_value(libv::input::JoystickButton::Button8) == GLFW_JOYSTICK_9);
	CHECK(libv::to_value(libv::input::JoystickButton::Button9) == GLFW_JOYSTICK_10);
	CHECK(libv::to_value(libv::input::JoystickButton::Button10) == GLFW_JOYSTICK_11);
	CHECK(libv::to_value(libv::input::JoystickButton::Button11) == GLFW_JOYSTICK_12);
	CHECK(libv::to_value(libv::input::JoystickButton::Button12) == GLFW_JOYSTICK_13);
	CHECK(libv::to_value(libv::input::JoystickButton::Button13) == GLFW_JOYSTICK_14);
	CHECK(libv::to_value(libv::input::JoystickButton::Button14) == GLFW_JOYSTICK_15);
	CHECK(libv::to_value(libv::input::JoystickButton::Button15) == GLFW_JOYSTICK_16);

	CHECK(libv::to_value(libv::input::JoystickButton::Button15) == GLFW_JOYSTICK_LAST);
}

TEST_CASE("Check Gamepad consistency with GLFW") {
	CHECK(libv::to_value(libv::input::GamepadButton::A) == GLFW_GAMEPAD_BUTTON_A);
	CHECK(libv::to_value(libv::input::GamepadButton::B) == GLFW_GAMEPAD_BUTTON_B);
	CHECK(libv::to_value(libv::input::GamepadButton::X) == GLFW_GAMEPAD_BUTTON_X);
	CHECK(libv::to_value(libv::input::GamepadButton::Y) == GLFW_GAMEPAD_BUTTON_Y);
	CHECK(libv::to_value(libv::input::GamepadButton::LeftBumper) == GLFW_GAMEPAD_BUTTON_LEFT_BUMPER);
	CHECK(libv::to_value(libv::input::GamepadButton::RightBumper) == GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);
	CHECK(libv::to_value(libv::input::GamepadButton::Back) == GLFW_GAMEPAD_BUTTON_BACK);
	CHECK(libv::to_value(libv::input::GamepadButton::Start) == GLFW_GAMEPAD_BUTTON_START);
	CHECK(libv::to_value(libv::input::GamepadButton::Guide) == GLFW_GAMEPAD_BUTTON_GUIDE);
	CHECK(libv::to_value(libv::input::GamepadButton::LeftThumb) == GLFW_GAMEPAD_BUTTON_LEFT_THUMB);
	CHECK(libv::to_value(libv::input::GamepadButton::RightThumb) == GLFW_GAMEPAD_BUTTON_RIGHT_THUMB);
	CHECK(libv::to_value(libv::input::GamepadButton::DPadUp) == GLFW_GAMEPAD_BUTTON_DPAD_UP);
	CHECK(libv::to_value(libv::input::GamepadButton::DPadRight) == GLFW_GAMEPAD_BUTTON_DPAD_RIGHT);
	CHECK(libv::to_value(libv::input::GamepadButton::DPadDown) == GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
	CHECK(libv::to_value(libv::input::GamepadButton::DPadLeft) == GLFW_GAMEPAD_BUTTON_DPAD_LEFT);

	CHECK(libv::to_value(libv::input::GamepadButton::Cross) == GLFW_GAMEPAD_BUTTON_CROSS);
	CHECK(libv::to_value(libv::input::GamepadButton::Circle) == GLFW_GAMEPAD_BUTTON_CIRCLE);
	CHECK(libv::to_value(libv::input::GamepadButton::Square) == GLFW_GAMEPAD_BUTTON_SQUARE);
	CHECK(libv::to_value(libv::input::GamepadButton::Triangle) == GLFW_GAMEPAD_BUTTON_TRIANGLE);

	CHECK(libv::to_value(libv::input::GamepadButton::DPadLeft) == GLFW_GAMEPAD_BUTTON_LAST);
}

TEST_CASE("Check Key consistency with GLFW") {
	CHECK(libv::to_value(libv::input::Keycode::Unknown) == GLFW_KEY_UNKNOWN);
	CHECK(libv::to_value(libv::input::Keycode::Space) == GLFW_KEY_SPACE);
	CHECK(libv::to_value(libv::input::Keycode::Apostrophe) == GLFW_KEY_APOSTROPHE);
	CHECK(libv::to_value(libv::input::Keycode::Comma) == GLFW_KEY_COMMA);
	CHECK(libv::to_value(libv::input::Keycode::Minus) == GLFW_KEY_MINUS);
	CHECK(libv::to_value(libv::input::Keycode::Period) == GLFW_KEY_PERIOD);
	CHECK(libv::to_value(libv::input::Keycode::Slash) == GLFW_KEY_SLASH);
	CHECK(libv::to_value(libv::input::Keycode::Num0) == GLFW_KEY_0);
	CHECK(libv::to_value(libv::input::Keycode::Num1) == GLFW_KEY_1);
	CHECK(libv::to_value(libv::input::Keycode::Num2) == GLFW_KEY_2);
	CHECK(libv::to_value(libv::input::Keycode::Num3) == GLFW_KEY_3);
	CHECK(libv::to_value(libv::input::Keycode::Num4) == GLFW_KEY_4);
	CHECK(libv::to_value(libv::input::Keycode::Num5) == GLFW_KEY_5);
	CHECK(libv::to_value(libv::input::Keycode::Num6) == GLFW_KEY_6);
	CHECK(libv::to_value(libv::input::Keycode::Num7) == GLFW_KEY_7);
	CHECK(libv::to_value(libv::input::Keycode::Num8) == GLFW_KEY_8);
	CHECK(libv::to_value(libv::input::Keycode::Num9) == GLFW_KEY_9);
	CHECK(libv::to_value(libv::input::Keycode::Semicolon) == GLFW_KEY_SEMICOLON);
	CHECK(libv::to_value(libv::input::Keycode::Equals) == GLFW_KEY_EQUAL);
	CHECK(libv::to_value(libv::input::Keycode::A) == GLFW_KEY_A);
	CHECK(libv::to_value(libv::input::Keycode::B) == GLFW_KEY_B);
	CHECK(libv::to_value(libv::input::Keycode::C) == GLFW_KEY_C);
	CHECK(libv::to_value(libv::input::Keycode::D) == GLFW_KEY_D);
	CHECK(libv::to_value(libv::input::Keycode::E) == GLFW_KEY_E);
	CHECK(libv::to_value(libv::input::Keycode::F) == GLFW_KEY_F);
	CHECK(libv::to_value(libv::input::Keycode::G) == GLFW_KEY_G);
	CHECK(libv::to_value(libv::input::Keycode::H) == GLFW_KEY_H);
	CHECK(libv::to_value(libv::input::Keycode::I) == GLFW_KEY_I);
	CHECK(libv::to_value(libv::input::Keycode::J) == GLFW_KEY_J);
	CHECK(libv::to_value(libv::input::Keycode::K) == GLFW_KEY_K);
	CHECK(libv::to_value(libv::input::Keycode::L) == GLFW_KEY_L);
	CHECK(libv::to_value(libv::input::Keycode::M) == GLFW_KEY_M);
	CHECK(libv::to_value(libv::input::Keycode::N) == GLFW_KEY_N);
	CHECK(libv::to_value(libv::input::Keycode::O) == GLFW_KEY_O);
	CHECK(libv::to_value(libv::input::Keycode::P) == GLFW_KEY_P);
	CHECK(libv::to_value(libv::input::Keycode::Q) == GLFW_KEY_Q);
	CHECK(libv::to_value(libv::input::Keycode::R) == GLFW_KEY_R);
	CHECK(libv::to_value(libv::input::Keycode::S) == GLFW_KEY_S);
	CHECK(libv::to_value(libv::input::Keycode::T) == GLFW_KEY_T);
	CHECK(libv::to_value(libv::input::Keycode::U) == GLFW_KEY_U);
	CHECK(libv::to_value(libv::input::Keycode::V) == GLFW_KEY_V);
	CHECK(libv::to_value(libv::input::Keycode::W) == GLFW_KEY_W);
	CHECK(libv::to_value(libv::input::Keycode::X) == GLFW_KEY_X);
	CHECK(libv::to_value(libv::input::Keycode::Y) == GLFW_KEY_Y);
	CHECK(libv::to_value(libv::input::Keycode::Z) == GLFW_KEY_Z);
	CHECK(libv::to_value(libv::input::Keycode::BracketOpen) == GLFW_KEY_LEFT_BRACKET);
	CHECK(libv::to_value(libv::input::Keycode::Backslash) == GLFW_KEY_BACKSLASH);
	CHECK(libv::to_value(libv::input::Keycode::BracketClose) == GLFW_KEY_RIGHT_BRACKET);
	CHECK(libv::to_value(libv::input::Keycode::Backtick) == GLFW_KEY_GRAVE_ACCENT);
	CHECK(libv::to_value(libv::input::Keycode::World1) == GLFW_KEY_WORLD_1);
	CHECK(libv::to_value(libv::input::Keycode::World2) == GLFW_KEY_WORLD_2);
	CHECK(libv::to_value(libv::input::Keycode::Escape) == GLFW_KEY_ESCAPE);
	CHECK(libv::to_value(libv::input::Keycode::Enter) == GLFW_KEY_ENTER);
	CHECK(libv::to_value(libv::input::Keycode::Tab) == GLFW_KEY_TAB);
	CHECK(libv::to_value(libv::input::Keycode::Backspace) == GLFW_KEY_BACKSPACE);
	CHECK(libv::to_value(libv::input::Keycode::Insert) == GLFW_KEY_INSERT);
	CHECK(libv::to_value(libv::input::Keycode::Delete) == GLFW_KEY_DELETE);
	CHECK(libv::to_value(libv::input::Keycode::Right) == GLFW_KEY_RIGHT);
	CHECK(libv::to_value(libv::input::Keycode::Left) == GLFW_KEY_LEFT);
	CHECK(libv::to_value(libv::input::Keycode::Down) == GLFW_KEY_DOWN);
	CHECK(libv::to_value(libv::input::Keycode::Up) == GLFW_KEY_UP);
	CHECK(libv::to_value(libv::input::Keycode::PageUp) == GLFW_KEY_PAGE_UP);
	CHECK(libv::to_value(libv::input::Keycode::PageDown) == GLFW_KEY_PAGE_DOWN);
	CHECK(libv::to_value(libv::input::Keycode::Home) == GLFW_KEY_HOME);
	CHECK(libv::to_value(libv::input::Keycode::End) == GLFW_KEY_END);
	CHECK(libv::to_value(libv::input::Keycode::CapsLock) == GLFW_KEY_CAPS_LOCK);
	CHECK(libv::to_value(libv::input::Keycode::ScrollLock) == GLFW_KEY_SCROLL_LOCK);
	CHECK(libv::to_value(libv::input::Keycode::NumLock) == GLFW_KEY_NUM_LOCK);
	CHECK(libv::to_value(libv::input::Keycode::PrintScreen) == GLFW_KEY_PRINT_SCREEN);
	CHECK(libv::to_value(libv::input::Keycode::Pause) == GLFW_KEY_PAUSE);
	CHECK(libv::to_value(libv::input::Keycode::F1) == GLFW_KEY_F1);
	CHECK(libv::to_value(libv::input::Keycode::F2) == GLFW_KEY_F2);
	CHECK(libv::to_value(libv::input::Keycode::F3) == GLFW_KEY_F3);
	CHECK(libv::to_value(libv::input::Keycode::F4) == GLFW_KEY_F4);
	CHECK(libv::to_value(libv::input::Keycode::F5) == GLFW_KEY_F5);
	CHECK(libv::to_value(libv::input::Keycode::F6) == GLFW_KEY_F6);
	CHECK(libv::to_value(libv::input::Keycode::F7) == GLFW_KEY_F7);
	CHECK(libv::to_value(libv::input::Keycode::F8) == GLFW_KEY_F8);
	CHECK(libv::to_value(libv::input::Keycode::F9) == GLFW_KEY_F9);
	CHECK(libv::to_value(libv::input::Keycode::F10) == GLFW_KEY_F10);
	CHECK(libv::to_value(libv::input::Keycode::F11) == GLFW_KEY_F11);
	CHECK(libv::to_value(libv::input::Keycode::F12) == GLFW_KEY_F12);
	CHECK(libv::to_value(libv::input::Keycode::F13) == GLFW_KEY_F13);
	CHECK(libv::to_value(libv::input::Keycode::F14) == GLFW_KEY_F14);
	CHECK(libv::to_value(libv::input::Keycode::F15) == GLFW_KEY_F15);
	CHECK(libv::to_value(libv::input::Keycode::F16) == GLFW_KEY_F16);
	CHECK(libv::to_value(libv::input::Keycode::F17) == GLFW_KEY_F17);
	CHECK(libv::to_value(libv::input::Keycode::F18) == GLFW_KEY_F18);
	CHECK(libv::to_value(libv::input::Keycode::F19) == GLFW_KEY_F19);
	CHECK(libv::to_value(libv::input::Keycode::F20) == GLFW_KEY_F20);
	CHECK(libv::to_value(libv::input::Keycode::F21) == GLFW_KEY_F21);
	CHECK(libv::to_value(libv::input::Keycode::F22) == GLFW_KEY_F22);
	CHECK(libv::to_value(libv::input::Keycode::F23) == GLFW_KEY_F23);
	CHECK(libv::to_value(libv::input::Keycode::F24) == GLFW_KEY_F24);
	CHECK(libv::to_value(libv::input::Keycode::F25) == GLFW_KEY_F25);
	CHECK(libv::to_value(libv::input::Keycode::KPNum0) == GLFW_KEY_KP_0);
	CHECK(libv::to_value(libv::input::Keycode::KPNum1) == GLFW_KEY_KP_1);
	CHECK(libv::to_value(libv::input::Keycode::KPNum2) == GLFW_KEY_KP_2);
	CHECK(libv::to_value(libv::input::Keycode::KPNum3) == GLFW_KEY_KP_3);
	CHECK(libv::to_value(libv::input::Keycode::KPNum4) == GLFW_KEY_KP_4);
	CHECK(libv::to_value(libv::input::Keycode::KPNum5) == GLFW_KEY_KP_5);
	CHECK(libv::to_value(libv::input::Keycode::KPNum6) == GLFW_KEY_KP_6);
	CHECK(libv::to_value(libv::input::Keycode::KPNum7) == GLFW_KEY_KP_7);
	CHECK(libv::to_value(libv::input::Keycode::KPNum8) == GLFW_KEY_KP_8);
	CHECK(libv::to_value(libv::input::Keycode::KPNum9) == GLFW_KEY_KP_9);
	CHECK(libv::to_value(libv::input::Keycode::KPDot) == GLFW_KEY_KP_DECIMAL);
	CHECK(libv::to_value(libv::input::Keycode::KPSlash) == GLFW_KEY_KP_DIVIDE);
	CHECK(libv::to_value(libv::input::Keycode::KPAsterisk) == GLFW_KEY_KP_MULTIPLY);
	CHECK(libv::to_value(libv::input::Keycode::KPMinus) == GLFW_KEY_KP_SUBTRACT);
	CHECK(libv::to_value(libv::input::Keycode::KPPlus) == GLFW_KEY_KP_ADD);
	CHECK(libv::to_value(libv::input::Keycode::KPEnter) == GLFW_KEY_KP_ENTER);
	CHECK(libv::to_value(libv::input::Keycode::KPEqual) == GLFW_KEY_KP_EQUAL);
	CHECK(libv::to_value(libv::input::Keycode::ShiftLeft) == GLFW_KEY_LEFT_SHIFT);
	CHECK(libv::to_value(libv::input::Keycode::ControlLeft) == GLFW_KEY_LEFT_CONTROL);
	CHECK(libv::to_value(libv::input::Keycode::AltLeft) == GLFW_KEY_LEFT_ALT);
	CHECK(libv::to_value(libv::input::Keycode::SuperLeft) == GLFW_KEY_LEFT_SUPER);
	CHECK(libv::to_value(libv::input::Keycode::ShiftRight) == GLFW_KEY_RIGHT_SHIFT);
	CHECK(libv::to_value(libv::input::Keycode::ControlRight) == GLFW_KEY_RIGHT_CONTROL);
	CHECK(libv::to_value(libv::input::Keycode::AltRight) == GLFW_KEY_RIGHT_ALT);
	CHECK(libv::to_value(libv::input::Keycode::SuperRight) == GLFW_KEY_RIGHT_SUPER);
	CHECK(libv::to_value(libv::input::Keycode::Menu) == GLFW_KEY_MENU);

	CHECK(libv::to_value(libv::input::Keycode::Menu) == GLFW_KEY_LAST);
}
