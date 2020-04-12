// File: events.hpp, Created on 2019. 02. 04. 23:42, Author: Vader

// hpp
#include <libv/input/input.hpp>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

std::string_view to_string(const Action value) {
	switch (value) {
	case Action::release: return "release";
	case Action::press: return "press";
	case Action::repeat: return "repeat";
	}

	return "<<invalid>>";
}

std::string_view to_string(const GamepadButton value) {
	switch (value) {
	case GamepadButton::A: return "A";
	case GamepadButton::B: return "B";
	case GamepadButton::X: return "X";
	case GamepadButton::Y: return "Y";
	case GamepadButton::LeftBumper: return "LeftBumper";
	case GamepadButton::RightBumper: return "RightBumper";
	case GamepadButton::Back: return "Back";
	case GamepadButton::Start: return "Start";
	case GamepadButton::Guide: return "Guide";
	case GamepadButton::LeftThumb: return "LeftThumb";
	case GamepadButton::RightThumb: return "RightThumb";
	case GamepadButton::DPadUp: return "DPadUp";
	case GamepadButton::DPadRight: return "DPadRight";
	case GamepadButton::DPadDown: return "DPadDown";
	case GamepadButton::DPadLeft: return "DPadLeft";
	}

	return "<<invalid>>";
}

std::string_view to_string(const Key value) {
	switch (value) {
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

	return "<<invalid>>";
}

std::string_view to_string(const KeyState value) {
	switch (value) {
	case KeyState::pressed: return "pressed";
	case KeyState::released: return "released";
	}

	return "<<invalid>>";
}

std::string_view to_string(const MonitorEvent value) {
	switch (value) {
	case MonitorEvent::connected: return "connected";
	case MonitorEvent::disconnected: return "disconnected";
	}

	return "<<invalid>>";
}

std::string_view to_string(const MouseButton value) {
	switch (value) {
	case MouseButton::Button0: static_assert(MouseButton::Button0 == MouseButton::Left, ""); return "left";
	case MouseButton::Button1: static_assert(MouseButton::Button1 == MouseButton::Right, ""); return "right";
	case MouseButton::Button2: static_assert(MouseButton::Button2 == MouseButton::Middle, ""); return "middle";
	case MouseButton::Button3: return "mouse3";
	case MouseButton::Button4: return "mouse4";
	case MouseButton::Button5: return "mouse5";
	case MouseButton::Button6: return "mouse6";
	case MouseButton::Button7: return "mouse7";
	case MouseButton::Button8: return "mouse8";
	case MouseButton::Button9: return "mouse9";
	case MouseButton::Button10: return "mouse10";
	case MouseButton::Button11: return "mouse11";
	case MouseButton::Button12: return "mouse12";
	case MouseButton::Button13: return "mouse13";
	case MouseButton::Button14: return "mouse14";
	case MouseButton::Button15: return "mouse15";
	case MouseButton::Button16: return "mouse16";
	case MouseButton::Button17: return "mouse17";
	case MouseButton::Button18: return "mouse18";
	case MouseButton::Button19: return "mouse19";
	case MouseButton::Button20: return "mouse20";
	case MouseButton::Button21: return "mouse21";
	case MouseButton::Button22: return "mouse22";
	case MouseButton::Button23: return "mouse23";
	case MouseButton::Button24: return "mouse24";
	case MouseButton::Button25: return "mouse25";
	case MouseButton::Button26: return "mouse26";
	case MouseButton::Button27: return "mouse27";
	case MouseButton::Button28: return "mouse28";
	case MouseButton::Button29: return "mouse29";
	case MouseButton::Button30: return "mouse30";
	case MouseButton::Button31: return "mouse31";
	case MouseButton::Button32: return "mouse32";
	case MouseButton::Button33: return "mouse33";
	case MouseButton::Button34: return "mouse34";
	case MouseButton::Button35: return "mouse35";
	case MouseButton::Button36: return "mouse36";
	case MouseButton::Button37: return "mouse37";
	case MouseButton::Button38: return "mouse38";
	case MouseButton::Button39: return "mouse39";
	case MouseButton::Button40: return "mouse40";
	case MouseButton::Button41: return "mouse41";
	case MouseButton::Button42: return "mouse42";
	case MouseButton::Button43: return "mouse43";
	case MouseButton::Button44: return "mouse44";
	case MouseButton::Button45: return "mouse45";
	case MouseButton::Button46: return "mouse46";
	case MouseButton::Button47: return "mouse47";
	case MouseButton::Button48: return "mouse48";
	case MouseButton::Button49: return "mouse49";
	case MouseButton::Button50: return "mouse50";
	case MouseButton::Button51: return "mouse51";
	case MouseButton::Button52: return "mouse52";
	case MouseButton::Button53: return "mouse53";
	case MouseButton::Button54: return "mouse54";
	case MouseButton::Button55: return "mouse55";
	case MouseButton::Button56: return "mouse56";
	case MouseButton::Button57: return "mouse57";
	case MouseButton::Button58: return "mouse58";
	case MouseButton::Button59: return "mouse59";
	case MouseButton::Button60: return "mouse60";
	case MouseButton::Button61: return "mouse61";
	case MouseButton::Button62: return "mouse62";
	case MouseButton::Button63: return "mouse63";
	}

	return "<<invalid>>";
}

// -------------------------------------------------------------------------------------------------

} // namespace input
} // namespace libv
