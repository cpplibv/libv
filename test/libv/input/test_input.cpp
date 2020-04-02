// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/utility/enum.hpp>
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
	CHECK(libv::to_value(libv::input::Joystick::Button0) == GLFW_JOYSTICK_1);
	CHECK(libv::to_value(libv::input::Joystick::Button1) == GLFW_JOYSTICK_2);
	CHECK(libv::to_value(libv::input::Joystick::Button2) == GLFW_JOYSTICK_3);
	CHECK(libv::to_value(libv::input::Joystick::Button3) == GLFW_JOYSTICK_4);
	CHECK(libv::to_value(libv::input::Joystick::Button4) == GLFW_JOYSTICK_5);
	CHECK(libv::to_value(libv::input::Joystick::Button5) == GLFW_JOYSTICK_6);
	CHECK(libv::to_value(libv::input::Joystick::Button6) == GLFW_JOYSTICK_7);
	CHECK(libv::to_value(libv::input::Joystick::Button7) == GLFW_JOYSTICK_8);
	CHECK(libv::to_value(libv::input::Joystick::Button8) == GLFW_JOYSTICK_9);
	CHECK(libv::to_value(libv::input::Joystick::Button9) == GLFW_JOYSTICK_10);
	CHECK(libv::to_value(libv::input::Joystick::Button10) == GLFW_JOYSTICK_11);
	CHECK(libv::to_value(libv::input::Joystick::Button11) == GLFW_JOYSTICK_12);
	CHECK(libv::to_value(libv::input::Joystick::Button12) == GLFW_JOYSTICK_13);
	CHECK(libv::to_value(libv::input::Joystick::Button13) == GLFW_JOYSTICK_14);
	CHECK(libv::to_value(libv::input::Joystick::Button14) == GLFW_JOYSTICK_15);
	CHECK(libv::to_value(libv::input::Joystick::Button15) == GLFW_JOYSTICK_16);

	CHECK(libv::to_value(libv::input::Joystick::Button15) == GLFW_JOYSTICK_LAST);
}

TEST_CASE("Check Key consistency with GLFW") {
	CHECK(libv::to_value(libv::input::Key::Unknown) == GLFW_KEY_UNKNOWN);
	CHECK(libv::to_value(libv::input::Key::Space) == GLFW_KEY_SPACE);
	CHECK(libv::to_value(libv::input::Key::Apostrophe) == GLFW_KEY_APOSTROPHE);
	CHECK(libv::to_value(libv::input::Key::Comma) == GLFW_KEY_COMMA);
	CHECK(libv::to_value(libv::input::Key::Minus) == GLFW_KEY_MINUS);
	CHECK(libv::to_value(libv::input::Key::Period) == GLFW_KEY_PERIOD);
	CHECK(libv::to_value(libv::input::Key::Slash) == GLFW_KEY_SLASH);
	CHECK(libv::to_value(libv::input::Key::Num0) == GLFW_KEY_0);
	CHECK(libv::to_value(libv::input::Key::Num1) == GLFW_KEY_1);
	CHECK(libv::to_value(libv::input::Key::Num2) == GLFW_KEY_2);
	CHECK(libv::to_value(libv::input::Key::Num3) == GLFW_KEY_3);
	CHECK(libv::to_value(libv::input::Key::Num4) == GLFW_KEY_4);
	CHECK(libv::to_value(libv::input::Key::Num5) == GLFW_KEY_5);
	CHECK(libv::to_value(libv::input::Key::Num6) == GLFW_KEY_6);
	CHECK(libv::to_value(libv::input::Key::Num7) == GLFW_KEY_7);
	CHECK(libv::to_value(libv::input::Key::Num8) == GLFW_KEY_8);
	CHECK(libv::to_value(libv::input::Key::Num9) == GLFW_KEY_9);
	CHECK(libv::to_value(libv::input::Key::Semicolon) == GLFW_KEY_SEMICOLON);
	CHECK(libv::to_value(libv::input::Key::Equals) == GLFW_KEY_EQUAL);
	CHECK(libv::to_value(libv::input::Key::A) == GLFW_KEY_A);
	CHECK(libv::to_value(libv::input::Key::B) == GLFW_KEY_B);
	CHECK(libv::to_value(libv::input::Key::C) == GLFW_KEY_C);
	CHECK(libv::to_value(libv::input::Key::D) == GLFW_KEY_D);
	CHECK(libv::to_value(libv::input::Key::E) == GLFW_KEY_E);
	CHECK(libv::to_value(libv::input::Key::F) == GLFW_KEY_F);
	CHECK(libv::to_value(libv::input::Key::G) == GLFW_KEY_G);
	CHECK(libv::to_value(libv::input::Key::H) == GLFW_KEY_H);
	CHECK(libv::to_value(libv::input::Key::I) == GLFW_KEY_I);
	CHECK(libv::to_value(libv::input::Key::J) == GLFW_KEY_J);
	CHECK(libv::to_value(libv::input::Key::K) == GLFW_KEY_K);
	CHECK(libv::to_value(libv::input::Key::L) == GLFW_KEY_L);
	CHECK(libv::to_value(libv::input::Key::M) == GLFW_KEY_M);
	CHECK(libv::to_value(libv::input::Key::N) == GLFW_KEY_N);
	CHECK(libv::to_value(libv::input::Key::O) == GLFW_KEY_O);
	CHECK(libv::to_value(libv::input::Key::P) == GLFW_KEY_P);
	CHECK(libv::to_value(libv::input::Key::Q) == GLFW_KEY_Q);
	CHECK(libv::to_value(libv::input::Key::R) == GLFW_KEY_R);
	CHECK(libv::to_value(libv::input::Key::S) == GLFW_KEY_S);
	CHECK(libv::to_value(libv::input::Key::T) == GLFW_KEY_T);
	CHECK(libv::to_value(libv::input::Key::U) == GLFW_KEY_U);
	CHECK(libv::to_value(libv::input::Key::V) == GLFW_KEY_V);
	CHECK(libv::to_value(libv::input::Key::W) == GLFW_KEY_W);
	CHECK(libv::to_value(libv::input::Key::X) == GLFW_KEY_X);
	CHECK(libv::to_value(libv::input::Key::Y) == GLFW_KEY_Y);
	CHECK(libv::to_value(libv::input::Key::Z) == GLFW_KEY_Z);
	CHECK(libv::to_value(libv::input::Key::BracketOpen) == GLFW_KEY_LEFT_BRACKET);
	CHECK(libv::to_value(libv::input::Key::Backslash) == GLFW_KEY_BACKSLASH);
	CHECK(libv::to_value(libv::input::Key::BracketClose) == GLFW_KEY_RIGHT_BRACKET);
	CHECK(libv::to_value(libv::input::Key::Backtick) == GLFW_KEY_GRAVE_ACCENT);
	CHECK(libv::to_value(libv::input::Key::World1) == GLFW_KEY_WORLD_1);
	CHECK(libv::to_value(libv::input::Key::World2) == GLFW_KEY_WORLD_2);
	CHECK(libv::to_value(libv::input::Key::Escape) == GLFW_KEY_ESCAPE);
	CHECK(libv::to_value(libv::input::Key::Enter) == GLFW_KEY_ENTER);
	CHECK(libv::to_value(libv::input::Key::Tab) == GLFW_KEY_TAB);
	CHECK(libv::to_value(libv::input::Key::Backspace) == GLFW_KEY_BACKSPACE);
	CHECK(libv::to_value(libv::input::Key::Insert) == GLFW_KEY_INSERT);
	CHECK(libv::to_value(libv::input::Key::Delete) == GLFW_KEY_DELETE);
	CHECK(libv::to_value(libv::input::Key::Right) == GLFW_KEY_RIGHT);
	CHECK(libv::to_value(libv::input::Key::Left) == GLFW_KEY_LEFT);
	CHECK(libv::to_value(libv::input::Key::Down) == GLFW_KEY_DOWN);
	CHECK(libv::to_value(libv::input::Key::Up) == GLFW_KEY_UP);
	CHECK(libv::to_value(libv::input::Key::PageUp) == GLFW_KEY_PAGE_UP);
	CHECK(libv::to_value(libv::input::Key::PageDown) == GLFW_KEY_PAGE_DOWN);
	CHECK(libv::to_value(libv::input::Key::Home) == GLFW_KEY_HOME);
	CHECK(libv::to_value(libv::input::Key::End) == GLFW_KEY_END);
	CHECK(libv::to_value(libv::input::Key::CapsLock) == GLFW_KEY_CAPS_LOCK);
	CHECK(libv::to_value(libv::input::Key::ScrollLock) == GLFW_KEY_SCROLL_LOCK);
	CHECK(libv::to_value(libv::input::Key::NumLock) == GLFW_KEY_NUM_LOCK);
	CHECK(libv::to_value(libv::input::Key::PrintScreen) == GLFW_KEY_PRINT_SCREEN);
	CHECK(libv::to_value(libv::input::Key::Pause) == GLFW_KEY_PAUSE);
	CHECK(libv::to_value(libv::input::Key::F1) == GLFW_KEY_F1);
	CHECK(libv::to_value(libv::input::Key::F2) == GLFW_KEY_F2);
	CHECK(libv::to_value(libv::input::Key::F3) == GLFW_KEY_F3);
	CHECK(libv::to_value(libv::input::Key::F4) == GLFW_KEY_F4);
	CHECK(libv::to_value(libv::input::Key::F5) == GLFW_KEY_F5);
	CHECK(libv::to_value(libv::input::Key::F6) == GLFW_KEY_F6);
	CHECK(libv::to_value(libv::input::Key::F7) == GLFW_KEY_F7);
	CHECK(libv::to_value(libv::input::Key::F8) == GLFW_KEY_F8);
	CHECK(libv::to_value(libv::input::Key::F9) == GLFW_KEY_F9);
	CHECK(libv::to_value(libv::input::Key::F10) == GLFW_KEY_F10);
	CHECK(libv::to_value(libv::input::Key::F11) == GLFW_KEY_F11);
	CHECK(libv::to_value(libv::input::Key::F12) == GLFW_KEY_F12);
	CHECK(libv::to_value(libv::input::Key::F13) == GLFW_KEY_F13);
	CHECK(libv::to_value(libv::input::Key::F14) == GLFW_KEY_F14);
	CHECK(libv::to_value(libv::input::Key::F15) == GLFW_KEY_F15);
	CHECK(libv::to_value(libv::input::Key::F16) == GLFW_KEY_F16);
	CHECK(libv::to_value(libv::input::Key::F17) == GLFW_KEY_F17);
	CHECK(libv::to_value(libv::input::Key::F18) == GLFW_KEY_F18);
	CHECK(libv::to_value(libv::input::Key::F19) == GLFW_KEY_F19);
	CHECK(libv::to_value(libv::input::Key::F20) == GLFW_KEY_F20);
	CHECK(libv::to_value(libv::input::Key::F21) == GLFW_KEY_F21);
	CHECK(libv::to_value(libv::input::Key::F22) == GLFW_KEY_F22);
	CHECK(libv::to_value(libv::input::Key::F23) == GLFW_KEY_F23);
	CHECK(libv::to_value(libv::input::Key::F24) == GLFW_KEY_F24);
	CHECK(libv::to_value(libv::input::Key::F25) == GLFW_KEY_F25);
	CHECK(libv::to_value(libv::input::Key::KPNum0) == GLFW_KEY_KP_0);
	CHECK(libv::to_value(libv::input::Key::KPNum1) == GLFW_KEY_KP_1);
	CHECK(libv::to_value(libv::input::Key::KPNum2) == GLFW_KEY_KP_2);
	CHECK(libv::to_value(libv::input::Key::KPNum3) == GLFW_KEY_KP_3);
	CHECK(libv::to_value(libv::input::Key::KPNum4) == GLFW_KEY_KP_4);
	CHECK(libv::to_value(libv::input::Key::KPNum5) == GLFW_KEY_KP_5);
	CHECK(libv::to_value(libv::input::Key::KPNum6) == GLFW_KEY_KP_6);
	CHECK(libv::to_value(libv::input::Key::KPNum7) == GLFW_KEY_KP_7);
	CHECK(libv::to_value(libv::input::Key::KPNum8) == GLFW_KEY_KP_8);
	CHECK(libv::to_value(libv::input::Key::KPNum9) == GLFW_KEY_KP_9);
	CHECK(libv::to_value(libv::input::Key::KPDot) == GLFW_KEY_KP_DECIMAL);
	CHECK(libv::to_value(libv::input::Key::KPSlash) == GLFW_KEY_KP_DIVIDE);
	CHECK(libv::to_value(libv::input::Key::KPAsterisk) == GLFW_KEY_KP_MULTIPLY);
	CHECK(libv::to_value(libv::input::Key::KPMinus) == GLFW_KEY_KP_SUBTRACT);
	CHECK(libv::to_value(libv::input::Key::KPPlus) == GLFW_KEY_KP_ADD);
	CHECK(libv::to_value(libv::input::Key::KPEnter) == GLFW_KEY_KP_ENTER);
	CHECK(libv::to_value(libv::input::Key::KPEqual) == GLFW_KEY_KP_EQUAL);
	CHECK(libv::to_value(libv::input::Key::ShiftLeft) == GLFW_KEY_LEFT_SHIFT);
	CHECK(libv::to_value(libv::input::Key::ControlLeft) == GLFW_KEY_LEFT_CONTROL);
	CHECK(libv::to_value(libv::input::Key::AltLeft) == GLFW_KEY_LEFT_ALT);
	CHECK(libv::to_value(libv::input::Key::SuperLeft) == GLFW_KEY_LEFT_SUPER);
	CHECK(libv::to_value(libv::input::Key::ShiftRight) == GLFW_KEY_RIGHT_SHIFT);
	CHECK(libv::to_value(libv::input::Key::ControlRight) == GLFW_KEY_RIGHT_CONTROL);
	CHECK(libv::to_value(libv::input::Key::AltRight) == GLFW_KEY_RIGHT_ALT);
	CHECK(libv::to_value(libv::input::Key::SuperRight) == GLFW_KEY_RIGHT_SUPER);
	CHECK(libv::to_value(libv::input::Key::Menu) == GLFW_KEY_MENU);

	CHECK(libv::to_value(libv::input::Key::Menu) == GLFW_KEY_LAST);
}
