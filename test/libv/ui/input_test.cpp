// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%


#include <catch.hpp>

#include <GLFW/glfw3.h>
#include <libv/ui/events/inputs.hpp>
#include <libv/utility/utility.hpp>

using namespace libv;
using namespace libv::ui;

// -------------------------------------------------------------------------------------------------

TEST_CASE("Check KeyState consistency with GLFW") {
	CHECK(to_value(KeyState::released) == GLFW_RELEASE);
	CHECK(to_value(KeyState::pressed) == GLFW_PRESS);
}

TEST_CASE("Check Key consistency with GLFW") {
	CHECK(to_value(Key::Unknown) == GLFW_KEY_UNKNOWN);
	CHECK(to_value(Key::Space) == GLFW_KEY_SPACE);
	CHECK(to_value(Key::Apostrophe) == GLFW_KEY_APOSTROPHE);
	CHECK(to_value(Key::Comma) == GLFW_KEY_COMMA);
	CHECK(to_value(Key::Minus) == GLFW_KEY_MINUS);
	CHECK(to_value(Key::Period) == GLFW_KEY_PERIOD);
	CHECK(to_value(Key::Slash) == GLFW_KEY_SLASH);
	CHECK(to_value(Key::Num0) == GLFW_KEY_0);
	CHECK(to_value(Key::Num1) == GLFW_KEY_1);
	CHECK(to_value(Key::Num2) == GLFW_KEY_2);
	CHECK(to_value(Key::Num3) == GLFW_KEY_3);
	CHECK(to_value(Key::Num4) == GLFW_KEY_4);
	CHECK(to_value(Key::Num5) == GLFW_KEY_5);
	CHECK(to_value(Key::Num6) == GLFW_KEY_6);
	CHECK(to_value(Key::Num7) == GLFW_KEY_7);
	CHECK(to_value(Key::Num8) == GLFW_KEY_8);
	CHECK(to_value(Key::Num9) == GLFW_KEY_9);
	CHECK(to_value(Key::Semicolon) == GLFW_KEY_SEMICOLON);
	CHECK(to_value(Key::Equals) == GLFW_KEY_EQUAL);
	CHECK(to_value(Key::A) == GLFW_KEY_A);
	CHECK(to_value(Key::B) == GLFW_KEY_B);
	CHECK(to_value(Key::C) == GLFW_KEY_C);
	CHECK(to_value(Key::D) == GLFW_KEY_D);
	CHECK(to_value(Key::E) == GLFW_KEY_E);
	CHECK(to_value(Key::F) == GLFW_KEY_F);
	CHECK(to_value(Key::G) == GLFW_KEY_G);
	CHECK(to_value(Key::H) == GLFW_KEY_H);
	CHECK(to_value(Key::I) == GLFW_KEY_I);
	CHECK(to_value(Key::J) == GLFW_KEY_J);
	CHECK(to_value(Key::K) == GLFW_KEY_K);
	CHECK(to_value(Key::L) == GLFW_KEY_L);
	CHECK(to_value(Key::M) == GLFW_KEY_M);
	CHECK(to_value(Key::N) == GLFW_KEY_N);
	CHECK(to_value(Key::O) == GLFW_KEY_O);
	CHECK(to_value(Key::P) == GLFW_KEY_P);
	CHECK(to_value(Key::Q) == GLFW_KEY_Q);
	CHECK(to_value(Key::R) == GLFW_KEY_R);
	CHECK(to_value(Key::S) == GLFW_KEY_S);
	CHECK(to_value(Key::T) == GLFW_KEY_T);
	CHECK(to_value(Key::U) == GLFW_KEY_U);
	CHECK(to_value(Key::V) == GLFW_KEY_V);
	CHECK(to_value(Key::W) == GLFW_KEY_W);
	CHECK(to_value(Key::X) == GLFW_KEY_X);
	CHECK(to_value(Key::Y) == GLFW_KEY_Y);
	CHECK(to_value(Key::Z) == GLFW_KEY_Z);
	CHECK(to_value(Key::BracketOpen) == GLFW_KEY_LEFT_BRACKET);
	CHECK(to_value(Key::Backslash) == GLFW_KEY_BACKSLASH);
	CHECK(to_value(Key::BracketClose) == GLFW_KEY_RIGHT_BRACKET);
	CHECK(to_value(Key::Backtick) == GLFW_KEY_GRAVE_ACCENT);
	CHECK(to_value(Key::World1) == GLFW_KEY_WORLD_1);
	CHECK(to_value(Key::World2) == GLFW_KEY_WORLD_2);
	CHECK(to_value(Key::Escape) == GLFW_KEY_ESCAPE);
	CHECK(to_value(Key::Enter) == GLFW_KEY_ENTER);
	CHECK(to_value(Key::Tab) == GLFW_KEY_TAB);
	CHECK(to_value(Key::Backspace) == GLFW_KEY_BACKSPACE);
	CHECK(to_value(Key::Insert) == GLFW_KEY_INSERT);
	CHECK(to_value(Key::Delete) == GLFW_KEY_DELETE);
	CHECK(to_value(Key::Right) == GLFW_KEY_RIGHT);
	CHECK(to_value(Key::Left) == GLFW_KEY_LEFT);
	CHECK(to_value(Key::Down) == GLFW_KEY_DOWN);
	CHECK(to_value(Key::Up) == GLFW_KEY_UP);
	CHECK(to_value(Key::PageUp) == GLFW_KEY_PAGE_UP);
	CHECK(to_value(Key::PageDown) == GLFW_KEY_PAGE_DOWN);
	CHECK(to_value(Key::Home) == GLFW_KEY_HOME);
	CHECK(to_value(Key::End) == GLFW_KEY_END);
	CHECK(to_value(Key::CapsLock) == GLFW_KEY_CAPS_LOCK);
	CHECK(to_value(Key::ScrollLock) == GLFW_KEY_SCROLL_LOCK);
	CHECK(to_value(Key::NumLock) == GLFW_KEY_NUM_LOCK);
	CHECK(to_value(Key::PrintScreen) == GLFW_KEY_PRINT_SCREEN);
	CHECK(to_value(Key::Pause) == GLFW_KEY_PAUSE);
	CHECK(to_value(Key::F1) == GLFW_KEY_F1);
	CHECK(to_value(Key::F2) == GLFW_KEY_F2);
	CHECK(to_value(Key::F3) == GLFW_KEY_F3);
	CHECK(to_value(Key::F4) == GLFW_KEY_F4);
	CHECK(to_value(Key::F5) == GLFW_KEY_F5);
	CHECK(to_value(Key::F6) == GLFW_KEY_F6);
	CHECK(to_value(Key::F7) == GLFW_KEY_F7);
	CHECK(to_value(Key::F8) == GLFW_KEY_F8);
	CHECK(to_value(Key::F9) == GLFW_KEY_F9);
	CHECK(to_value(Key::F10) == GLFW_KEY_F10);
	CHECK(to_value(Key::F11) == GLFW_KEY_F11);
	CHECK(to_value(Key::F12) == GLFW_KEY_F12);
	CHECK(to_value(Key::F13) == GLFW_KEY_F13);
	CHECK(to_value(Key::F14) == GLFW_KEY_F14);
	CHECK(to_value(Key::F15) == GLFW_KEY_F15);
	CHECK(to_value(Key::F16) == GLFW_KEY_F16);
	CHECK(to_value(Key::F17) == GLFW_KEY_F17);
	CHECK(to_value(Key::F18) == GLFW_KEY_F18);
	CHECK(to_value(Key::F19) == GLFW_KEY_F19);
	CHECK(to_value(Key::F20) == GLFW_KEY_F20);
	CHECK(to_value(Key::F21) == GLFW_KEY_F21);
	CHECK(to_value(Key::F22) == GLFW_KEY_F22);
	CHECK(to_value(Key::F23) == GLFW_KEY_F23);
	CHECK(to_value(Key::F24) == GLFW_KEY_F24);
	CHECK(to_value(Key::F25) == GLFW_KEY_F25);
	CHECK(to_value(Key::KPNum0) == GLFW_KEY_KP_0);
	CHECK(to_value(Key::KPNum1) == GLFW_KEY_KP_1);
	CHECK(to_value(Key::KPNum2) == GLFW_KEY_KP_2);
	CHECK(to_value(Key::KPNum3) == GLFW_KEY_KP_3);
	CHECK(to_value(Key::KPNum4) == GLFW_KEY_KP_4);
	CHECK(to_value(Key::KPNum5) == GLFW_KEY_KP_5);
	CHECK(to_value(Key::KPNum6) == GLFW_KEY_KP_6);
	CHECK(to_value(Key::KPNum7) == GLFW_KEY_KP_7);
	CHECK(to_value(Key::KPNum8) == GLFW_KEY_KP_8);
	CHECK(to_value(Key::KPNum9) == GLFW_KEY_KP_9);
	CHECK(to_value(Key::KPDot) == GLFW_KEY_KP_DECIMAL);
	CHECK(to_value(Key::KPSlash) == GLFW_KEY_KP_DIVIDE);
	CHECK(to_value(Key::KPAsterisk) == GLFW_KEY_KP_MULTIPLY);
	CHECK(to_value(Key::KPMinus) == GLFW_KEY_KP_SUBTRACT);
	CHECK(to_value(Key::KPPlus) == GLFW_KEY_KP_ADD);
	CHECK(to_value(Key::KPEnter) == GLFW_KEY_KP_ENTER);
	CHECK(to_value(Key::KPEqual) == GLFW_KEY_KP_EQUAL);
	CHECK(to_value(Key::ShiftLeft) == GLFW_KEY_LEFT_SHIFT);
	CHECK(to_value(Key::ControlLeft) == GLFW_KEY_LEFT_CONTROL);
	CHECK(to_value(Key::AltLeft) == GLFW_KEY_LEFT_ALT);
	CHECK(to_value(Key::SuperLeft) == GLFW_KEY_LEFT_SUPER);
	CHECK(to_value(Key::ShiftRight) == GLFW_KEY_RIGHT_SHIFT);
	CHECK(to_value(Key::ControlRight) == GLFW_KEY_RIGHT_CONTROL);
	CHECK(to_value(Key::AltRight) == GLFW_KEY_RIGHT_ALT);
	CHECK(to_value(Key::SuperRight) == GLFW_KEY_RIGHT_SUPER);
	CHECK(to_value(Key::Menu) == GLFW_KEY_MENU);

	CHECK(to_value(Key::Last) == GLFW_KEY_LAST);
}

TEST_CASE("Check Mouse consistency with GLFW") {
	CHECK(to_value(Mouse::Button0) == GLFW_MOUSE_BUTTON_1);
	CHECK(to_value(Mouse::Button1) == GLFW_MOUSE_BUTTON_2);
	CHECK(to_value(Mouse::Button2) == GLFW_MOUSE_BUTTON_3);
	CHECK(to_value(Mouse::Button3) == GLFW_MOUSE_BUTTON_4);
	CHECK(to_value(Mouse::Button4) == GLFW_MOUSE_BUTTON_5);
	CHECK(to_value(Mouse::Button5) == GLFW_MOUSE_BUTTON_6);
	CHECK(to_value(Mouse::Button6) == GLFW_MOUSE_BUTTON_7);
	CHECK(to_value(Mouse::Button7) == GLFW_MOUSE_BUTTON_8);

	CHECK(to_value(Mouse::Left) == GLFW_MOUSE_BUTTON_LEFT);
	CHECK(to_value(Mouse::Right) == GLFW_MOUSE_BUTTON_RIGHT);
	CHECK(to_value(Mouse::Middle) == GLFW_MOUSE_BUTTON_MIDDLE);

	CHECK(to_value(Mouse::Last) == GLFW_MOUSE_BUTTON_LAST);
}


