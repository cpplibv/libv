// File: events.hpp, Created on 2019. 02. 04. 23:42, Author: Vader

// hpp
#include <libv/input/inputs.hpp>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

std::string_view to_string(const Action value) {
	switch (value) {
	default: return "unknown";
	case Action::release: return "release";
	case Action::press: return "press";
	case Action::repeat: return "repeat";
	}
}

std::string_view to_string(const KeyState value) {
	switch (value) {
	default: return "unknown";
	case KeyState::pressed: return "pressed";
	case KeyState::released: return "released";
	}
}

std::string_view to_string(const Key value) {
	switch (value) {
	default: return "Invalid";
	case Key::Unknown: return "Unknown";
	case Key::Space: return "Space";
	case Key::Apostrophe: return "Apostrophe";
	case Key::Comma: return "Comma";
	case Key::Minus: return "Minus";
	case Key::Period: return "Period";
	case Key::Slash: return "Slash";
	case Key::Num0: return "Num0";
	case Key::Num1: return "Num1";
	case Key::Num2: return "Num2";
	case Key::Num3: return "Num3";
	case Key::Num4: return "Num4";
	case Key::Num5: return "Num5";
	case Key::Num6: return "Num6";
	case Key::Num7: return "Num7";
	case Key::Num8: return "Num8";
	case Key::Num9: return "Num9";
	case Key::Semicolon: return "Semicolon";
	case Key::Equals: return "Equals";
	case Key::A: return "A";
	case Key::B: return "B";
	case Key::C: return "C";
	case Key::D: return "D";
	case Key::E: return "E";
	case Key::F: return "F";
	case Key::G: return "G";
	case Key::H: return "H";
	case Key::I: return "I";
	case Key::J: return "J";
	case Key::K: return "K";
	case Key::L: return "L";
	case Key::M: return "M";
	case Key::N: return "N";
	case Key::O: return "O";
	case Key::P: return "P";
	case Key::Q: return "Q";
	case Key::R: return "R";
	case Key::S: return "S";
	case Key::T: return "T";
	case Key::U: return "U";
	case Key::V: return "V";
	case Key::W: return "W";
	case Key::X: return "X";
	case Key::Y: return "Y";
	case Key::Z: return "Z";
	case Key::BracketOpen: return "BracketOpen";
	case Key::Backslash: return "Backslash";
	case Key::BracketClose: return "BracketClose";
	case Key::Backtick: return "Backtick";
	case Key::World1: return "World1";
	case Key::World2: return "World2";
	case Key::Escape: return "Escape";
	case Key::Enter: return "Enter";
	case Key::Tab: return "Tab";
	case Key::Backspace: return "Backspace";
	case Key::Insert: return "Insert";
	case Key::Delete: return "Delete";
	case Key::Right: return "Right";
	case Key::Left: return "Left";
	case Key::Down: return "Down";
	case Key::Up: return "Up";
	case Key::PageUp: return "PageUp";
	case Key::PageDown: return "PageDown";
	case Key::Home: return "Home";
	case Key::End: return "End";
	case Key::CapsLock: return "CapsLock";
	case Key::ScrollLock: return "ScrollLock";
	case Key::NumLock: return "NumLock";
	case Key::PrintScreen: return "PrintScreen";
	case Key::Pause: return "Pause";
	case Key::F1: return "F1";
	case Key::F2: return "F2";
	case Key::F3: return "F3";
	case Key::F4: return "F4";
	case Key::F5: return "F5";
	case Key::F6: return "F6";
	case Key::F7: return "F7";
	case Key::F8: return "F8";
	case Key::F9: return "F9";
	case Key::F10: return "F10";
	case Key::F11: return "F11";
	case Key::F12: return "F12";
	case Key::F13: return "F13";
	case Key::F14: return "F14";
	case Key::F15: return "F15";
	case Key::F16: return "F16";
	case Key::F17: return "F17";
	case Key::F18: return "F18";
	case Key::F19: return "F19";
	case Key::F20: return "F20";
	case Key::F21: return "F21";
	case Key::F22: return "F22";
	case Key::F23: return "F23";
	case Key::F24: return "F24";
	case Key::F25: return "F25";
	case Key::KPNum0: return "KPNum0";
	case Key::KPNum1: return "KPNum1";
	case Key::KPNum2: return "KPNum2";
	case Key::KPNum3: return "KPNum3";
	case Key::KPNum4: return "KPNum4";
	case Key::KPNum5: return "KPNum5";
	case Key::KPNum6: return "KPNum6";
	case Key::KPNum7: return "KPNum7";
	case Key::KPNum8: return "KPNum8";
	case Key::KPNum9: return "KPNum9";
	case Key::KPDot: return "KPDot";
	case Key::KPSlash: return "KPSlash";
	case Key::KPAsterisk: return "KPAsterisk";
	case Key::KPMinus: return "KPMinus";
	case Key::KPPlus: return "KPPlus";
	case Key::KPEnter: return "KPEnter";
	case Key::KPEqual: return "KPEqual";
	case Key::ShiftLeft: return "ShiftLeft";
	case Key::ControlLeft: return "ControlLeft";
	case Key::AltLeft: return "AltLeft";
	case Key::SuperLeft: return "SuperLeft";
	case Key::ShiftRight: return "ShiftRight";
	case Key::ControlRight: return "ControlRight";
	case Key::AltRight: return "AltRight";
	case Key::SuperRight: return "SuperRight";
	case Key::Menu: return "Menu";
	}
}

std::string_view to_string(const Mouse value) {
	switch (value) {
	default: return "Unknown";
	case Mouse::Button0: return "Mouse0";
	case Mouse::Button1: return "Mouse1";
	case Mouse::Button2: return "Mouse2";
	case Mouse::Button3: return "Mouse3";
	case Mouse::Button4: return "Mouse4";
	case Mouse::Button5: return "Mouse5";
	case Mouse::Button6: return "Mouse6";
	case Mouse::Button7: return "Mouse7";
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace input
} // namespace libv
