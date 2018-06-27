// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%


#include <catch/catch.hpp>

#include <GLFW/glfw3.h>
#include <libv/frame/inputs.hpp>
#include <libv/utility/enum.hpp>

// -------------------------------------------------------------------------------------------------

TEST_CASE("Check KeyState consistency with GLFW") {
	CHECK(libv::to_value(libv::frame::KeyState::released) == GLFW_RELEASE);
	CHECK(libv::to_value(libv::frame::KeyState::pressed) == GLFW_PRESS);
}

TEST_CASE("Check Key consistency with GLFW") {
	CHECK(libv::to_value(libv::frame::Key::Unknown) == GLFW_KEY_UNKNOWN);
	CHECK(libv::to_value(libv::frame::Key::Space) == GLFW_KEY_SPACE);
	CHECK(libv::to_value(libv::frame::Key::Apostrophe) == GLFW_KEY_APOSTROPHE);
	CHECK(libv::to_value(libv::frame::Key::Comma) == GLFW_KEY_COMMA);
	CHECK(libv::to_value(libv::frame::Key::Minus) == GLFW_KEY_MINUS);
	CHECK(libv::to_value(libv::frame::Key::Period) == GLFW_KEY_PERIOD);
	CHECK(libv::to_value(libv::frame::Key::Slash) == GLFW_KEY_SLASH);
	CHECK(libv::to_value(libv::frame::Key::Num0) == GLFW_KEY_0);
	CHECK(libv::to_value(libv::frame::Key::Num1) == GLFW_KEY_1);
	CHECK(libv::to_value(libv::frame::Key::Num2) == GLFW_KEY_2);
	CHECK(libv::to_value(libv::frame::Key::Num3) == GLFW_KEY_3);
	CHECK(libv::to_value(libv::frame::Key::Num4) == GLFW_KEY_4);
	CHECK(libv::to_value(libv::frame::Key::Num5) == GLFW_KEY_5);
	CHECK(libv::to_value(libv::frame::Key::Num6) == GLFW_KEY_6);
	CHECK(libv::to_value(libv::frame::Key::Num7) == GLFW_KEY_7);
	CHECK(libv::to_value(libv::frame::Key::Num8) == GLFW_KEY_8);
	CHECK(libv::to_value(libv::frame::Key::Num9) == GLFW_KEY_9);
	CHECK(libv::to_value(libv::frame::Key::Semicolon) == GLFW_KEY_SEMICOLON);
	CHECK(libv::to_value(libv::frame::Key::Equals) == GLFW_KEY_EQUAL);
	CHECK(libv::to_value(libv::frame::Key::A) == GLFW_KEY_A);
	CHECK(libv::to_value(libv::frame::Key::B) == GLFW_KEY_B);
	CHECK(libv::to_value(libv::frame::Key::C) == GLFW_KEY_C);
	CHECK(libv::to_value(libv::frame::Key::D) == GLFW_KEY_D);
	CHECK(libv::to_value(libv::frame::Key::E) == GLFW_KEY_E);
	CHECK(libv::to_value(libv::frame::Key::F) == GLFW_KEY_F);
	CHECK(libv::to_value(libv::frame::Key::G) == GLFW_KEY_G);
	CHECK(libv::to_value(libv::frame::Key::H) == GLFW_KEY_H);
	CHECK(libv::to_value(libv::frame::Key::I) == GLFW_KEY_I);
	CHECK(libv::to_value(libv::frame::Key::J) == GLFW_KEY_J);
	CHECK(libv::to_value(libv::frame::Key::K) == GLFW_KEY_K);
	CHECK(libv::to_value(libv::frame::Key::L) == GLFW_KEY_L);
	CHECK(libv::to_value(libv::frame::Key::M) == GLFW_KEY_M);
	CHECK(libv::to_value(libv::frame::Key::N) == GLFW_KEY_N);
	CHECK(libv::to_value(libv::frame::Key::O) == GLFW_KEY_O);
	CHECK(libv::to_value(libv::frame::Key::P) == GLFW_KEY_P);
	CHECK(libv::to_value(libv::frame::Key::Q) == GLFW_KEY_Q);
	CHECK(libv::to_value(libv::frame::Key::R) == GLFW_KEY_R);
	CHECK(libv::to_value(libv::frame::Key::S) == GLFW_KEY_S);
	CHECK(libv::to_value(libv::frame::Key::T) == GLFW_KEY_T);
	CHECK(libv::to_value(libv::frame::Key::U) == GLFW_KEY_U);
	CHECK(libv::to_value(libv::frame::Key::V) == GLFW_KEY_V);
	CHECK(libv::to_value(libv::frame::Key::W) == GLFW_KEY_W);
	CHECK(libv::to_value(libv::frame::Key::X) == GLFW_KEY_X);
	CHECK(libv::to_value(libv::frame::Key::Y) == GLFW_KEY_Y);
	CHECK(libv::to_value(libv::frame::Key::Z) == GLFW_KEY_Z);
	CHECK(libv::to_value(libv::frame::Key::BracketOpen) == GLFW_KEY_LEFT_BRACKET);
	CHECK(libv::to_value(libv::frame::Key::Backslash) == GLFW_KEY_BACKSLASH);
	CHECK(libv::to_value(libv::frame::Key::BracketClose) == GLFW_KEY_RIGHT_BRACKET);
	CHECK(libv::to_value(libv::frame::Key::Backtick) == GLFW_KEY_GRAVE_ACCENT);
	CHECK(libv::to_value(libv::frame::Key::World1) == GLFW_KEY_WORLD_1);
	CHECK(libv::to_value(libv::frame::Key::World2) == GLFW_KEY_WORLD_2);
	CHECK(libv::to_value(libv::frame::Key::Escape) == GLFW_KEY_ESCAPE);
	CHECK(libv::to_value(libv::frame::Key::Enter) == GLFW_KEY_ENTER);
	CHECK(libv::to_value(libv::frame::Key::Tab) == GLFW_KEY_TAB);
	CHECK(libv::to_value(libv::frame::Key::Backspace) == GLFW_KEY_BACKSPACE);
	CHECK(libv::to_value(libv::frame::Key::Insert) == GLFW_KEY_INSERT);
	CHECK(libv::to_value(libv::frame::Key::Delete) == GLFW_KEY_DELETE);
	CHECK(libv::to_value(libv::frame::Key::Right) == GLFW_KEY_RIGHT);
	CHECK(libv::to_value(libv::frame::Key::Left) == GLFW_KEY_LEFT);
	CHECK(libv::to_value(libv::frame::Key::Down) == GLFW_KEY_DOWN);
	CHECK(libv::to_value(libv::frame::Key::Up) == GLFW_KEY_UP);
	CHECK(libv::to_value(libv::frame::Key::PageUp) == GLFW_KEY_PAGE_UP);
	CHECK(libv::to_value(libv::frame::Key::PageDown) == GLFW_KEY_PAGE_DOWN);
	CHECK(libv::to_value(libv::frame::Key::Home) == GLFW_KEY_HOME);
	CHECK(libv::to_value(libv::frame::Key::End) == GLFW_KEY_END);
	CHECK(libv::to_value(libv::frame::Key::CapsLock) == GLFW_KEY_CAPS_LOCK);
	CHECK(libv::to_value(libv::frame::Key::ScrollLock) == GLFW_KEY_SCROLL_LOCK);
	CHECK(libv::to_value(libv::frame::Key::NumLock) == GLFW_KEY_NUM_LOCK);
	CHECK(libv::to_value(libv::frame::Key::PrintScreen) == GLFW_KEY_PRINT_SCREEN);
	CHECK(libv::to_value(libv::frame::Key::Pause) == GLFW_KEY_PAUSE);
	CHECK(libv::to_value(libv::frame::Key::F1) == GLFW_KEY_F1);
	CHECK(libv::to_value(libv::frame::Key::F2) == GLFW_KEY_F2);
	CHECK(libv::to_value(libv::frame::Key::F3) == GLFW_KEY_F3);
	CHECK(libv::to_value(libv::frame::Key::F4) == GLFW_KEY_F4);
	CHECK(libv::to_value(libv::frame::Key::F5) == GLFW_KEY_F5);
	CHECK(libv::to_value(libv::frame::Key::F6) == GLFW_KEY_F6);
	CHECK(libv::to_value(libv::frame::Key::F7) == GLFW_KEY_F7);
	CHECK(libv::to_value(libv::frame::Key::F8) == GLFW_KEY_F8);
	CHECK(libv::to_value(libv::frame::Key::F9) == GLFW_KEY_F9);
	CHECK(libv::to_value(libv::frame::Key::F10) == GLFW_KEY_F10);
	CHECK(libv::to_value(libv::frame::Key::F11) == GLFW_KEY_F11);
	CHECK(libv::to_value(libv::frame::Key::F12) == GLFW_KEY_F12);
	CHECK(libv::to_value(libv::frame::Key::F13) == GLFW_KEY_F13);
	CHECK(libv::to_value(libv::frame::Key::F14) == GLFW_KEY_F14);
	CHECK(libv::to_value(libv::frame::Key::F15) == GLFW_KEY_F15);
	CHECK(libv::to_value(libv::frame::Key::F16) == GLFW_KEY_F16);
	CHECK(libv::to_value(libv::frame::Key::F17) == GLFW_KEY_F17);
	CHECK(libv::to_value(libv::frame::Key::F18) == GLFW_KEY_F18);
	CHECK(libv::to_value(libv::frame::Key::F19) == GLFW_KEY_F19);
	CHECK(libv::to_value(libv::frame::Key::F20) == GLFW_KEY_F20);
	CHECK(libv::to_value(libv::frame::Key::F21) == GLFW_KEY_F21);
	CHECK(libv::to_value(libv::frame::Key::F22) == GLFW_KEY_F22);
	CHECK(libv::to_value(libv::frame::Key::F23) == GLFW_KEY_F23);
	CHECK(libv::to_value(libv::frame::Key::F24) == GLFW_KEY_F24);
	CHECK(libv::to_value(libv::frame::Key::F25) == GLFW_KEY_F25);
	CHECK(libv::to_value(libv::frame::Key::KPNum0) == GLFW_KEY_KP_0);
	CHECK(libv::to_value(libv::frame::Key::KPNum1) == GLFW_KEY_KP_1);
	CHECK(libv::to_value(libv::frame::Key::KPNum2) == GLFW_KEY_KP_2);
	CHECK(libv::to_value(libv::frame::Key::KPNum3) == GLFW_KEY_KP_3);
	CHECK(libv::to_value(libv::frame::Key::KPNum4) == GLFW_KEY_KP_4);
	CHECK(libv::to_value(libv::frame::Key::KPNum5) == GLFW_KEY_KP_5);
	CHECK(libv::to_value(libv::frame::Key::KPNum6) == GLFW_KEY_KP_6);
	CHECK(libv::to_value(libv::frame::Key::KPNum7) == GLFW_KEY_KP_7);
	CHECK(libv::to_value(libv::frame::Key::KPNum8) == GLFW_KEY_KP_8);
	CHECK(libv::to_value(libv::frame::Key::KPNum9) == GLFW_KEY_KP_9);
	CHECK(libv::to_value(libv::frame::Key::KPDot) == GLFW_KEY_KP_DECIMAL);
	CHECK(libv::to_value(libv::frame::Key::KPSlash) == GLFW_KEY_KP_DIVIDE);
	CHECK(libv::to_value(libv::frame::Key::KPAsterisk) == GLFW_KEY_KP_MULTIPLY);
	CHECK(libv::to_value(libv::frame::Key::KPMinus) == GLFW_KEY_KP_SUBTRACT);
	CHECK(libv::to_value(libv::frame::Key::KPPlus) == GLFW_KEY_KP_ADD);
	CHECK(libv::to_value(libv::frame::Key::KPEnter) == GLFW_KEY_KP_ENTER);
	CHECK(libv::to_value(libv::frame::Key::KPEqual) == GLFW_KEY_KP_EQUAL);
	CHECK(libv::to_value(libv::frame::Key::ShiftLeft) == GLFW_KEY_LEFT_SHIFT);
	CHECK(libv::to_value(libv::frame::Key::ControlLeft) == GLFW_KEY_LEFT_CONTROL);
	CHECK(libv::to_value(libv::frame::Key::AltLeft) == GLFW_KEY_LEFT_ALT);
	CHECK(libv::to_value(libv::frame::Key::SuperLeft) == GLFW_KEY_LEFT_SUPER);
	CHECK(libv::to_value(libv::frame::Key::ShiftRight) == GLFW_KEY_RIGHT_SHIFT);
	CHECK(libv::to_value(libv::frame::Key::ControlRight) == GLFW_KEY_RIGHT_CONTROL);
	CHECK(libv::to_value(libv::frame::Key::AltRight) == GLFW_KEY_RIGHT_ALT);
	CHECK(libv::to_value(libv::frame::Key::SuperRight) == GLFW_KEY_RIGHT_SUPER);
	CHECK(libv::to_value(libv::frame::Key::Menu) == GLFW_KEY_MENU);

	CHECK(libv::to_value(libv::frame::Key::Last) == GLFW_KEY_LAST);
}

TEST_CASE("Check Mouse consistency with GLFW") {
	CHECK(libv::to_value(libv::frame::Mouse::Button0) == GLFW_MOUSE_BUTTON_1);
	CHECK(libv::to_value(libv::frame::Mouse::Button1) == GLFW_MOUSE_BUTTON_2);
	CHECK(libv::to_value(libv::frame::Mouse::Button2) == GLFW_MOUSE_BUTTON_3);
	CHECK(libv::to_value(libv::frame::Mouse::Button3) == GLFW_MOUSE_BUTTON_4);
	CHECK(libv::to_value(libv::frame::Mouse::Button4) == GLFW_MOUSE_BUTTON_5);
	CHECK(libv::to_value(libv::frame::Mouse::Button5) == GLFW_MOUSE_BUTTON_6);
	CHECK(libv::to_value(libv::frame::Mouse::Button6) == GLFW_MOUSE_BUTTON_7);
	CHECK(libv::to_value(libv::frame::Mouse::Button7) == GLFW_MOUSE_BUTTON_8);

	CHECK(libv::to_value(libv::frame::Mouse::Left) == GLFW_MOUSE_BUTTON_LEFT);
	CHECK(libv::to_value(libv::frame::Mouse::Right) == GLFW_MOUSE_BUTTON_RIGHT);
	CHECK(libv::to_value(libv::frame::Mouse::Middle) == GLFW_MOUSE_BUTTON_MIDDLE);

	CHECK(libv::to_value(libv::frame::Mouse::Last) == GLFW_MOUSE_BUTTON_LAST);
}
