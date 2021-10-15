// Project: libv.input, File: src/libv/input/input.cpp

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

std::string_view to_string(const Keycode value) {
	switch (value) {
	case Keycode::Unknown: return "Unknown";
	case Keycode::Space: return "Space";
	case Keycode::Apostrophe: return "Apostrophe";
	case Keycode::Comma: return "Comma";
	case Keycode::Minus: return "Minus";
	case Keycode::Period: return "Period";
	case Keycode::Slash: return "Slash";
	case Keycode::Num0: return "Num0";
	case Keycode::Num1: return "Num1";
	case Keycode::Num2: return "Num2";
	case Keycode::Num3: return "Num3";
	case Keycode::Num4: return "Num4";
	case Keycode::Num5: return "Num5";
	case Keycode::Num6: return "Num6";
	case Keycode::Num7: return "Num7";
	case Keycode::Num8: return "Num8";
	case Keycode::Num9: return "Num9";
	case Keycode::Semicolon: return "Semicolon";
	case Keycode::Equals: return "Equals";
	case Keycode::A: return "A";
	case Keycode::B: return "B";
	case Keycode::C: return "C";
	case Keycode::D: return "D";
	case Keycode::E: return "E";
	case Keycode::F: return "F";
	case Keycode::G: return "G";
	case Keycode::H: return "H";
	case Keycode::I: return "I";
	case Keycode::J: return "J";
	case Keycode::K: return "K";
	case Keycode::L: return "L";
	case Keycode::M: return "M";
	case Keycode::N: return "N";
	case Keycode::O: return "O";
	case Keycode::P: return "P";
	case Keycode::Q: return "Q";
	case Keycode::R: return "R";
	case Keycode::S: return "S";
	case Keycode::T: return "T";
	case Keycode::U: return "U";
	case Keycode::V: return "V";
	case Keycode::W: return "W";
	case Keycode::X: return "X";
	case Keycode::Y: return "Y";
	case Keycode::Z: return "Z";
	case Keycode::BracketOpen: return "BracketOpen";
	case Keycode::Backslash: return "Backslash";
	case Keycode::BracketClose: return "BracketClose";
	case Keycode::Backtick: return "Backtick";
	case Keycode::World1: return "World1";
	case Keycode::World2: return "World2";
	case Keycode::Escape: return "Escape";
	case Keycode::Enter: return "Enter";
	case Keycode::Tab: return "Tab";
	case Keycode::Backspace: return "Backspace";
	case Keycode::Insert: return "Insert";
	case Keycode::Delete: return "Delete";
	case Keycode::Right: return "Right";
	case Keycode::Left: return "Left";
	case Keycode::Down: return "Down";
	case Keycode::Up: return "Up";
	case Keycode::PageUp: return "PageUp";
	case Keycode::PageDown: return "PageDown";
	case Keycode::Home: return "Home";
	case Keycode::End: return "End";
	case Keycode::CapsLock: return "CapsLock";
	case Keycode::ScrollLock: return "ScrollLock";
	case Keycode::NumLock: return "NumLock";
	case Keycode::PrintScreen: return "PrintScreen";
	case Keycode::Pause: return "Pause";
	case Keycode::F1: return "F1";
	case Keycode::F2: return "F2";
	case Keycode::F3: return "F3";
	case Keycode::F4: return "F4";
	case Keycode::F5: return "F5";
	case Keycode::F6: return "F6";
	case Keycode::F7: return "F7";
	case Keycode::F8: return "F8";
	case Keycode::F9: return "F9";
	case Keycode::F10: return "F10";
	case Keycode::F11: return "F11";
	case Keycode::F12: return "F12";
	case Keycode::F13: return "F13";
	case Keycode::F14: return "F14";
	case Keycode::F15: return "F15";
	case Keycode::F16: return "F16";
	case Keycode::F17: return "F17";
	case Keycode::F18: return "F18";
	case Keycode::F19: return "F19";
	case Keycode::F20: return "F20";
	case Keycode::F21: return "F21";
	case Keycode::F22: return "F22";
	case Keycode::F23: return "F23";
	case Keycode::F24: return "F24";
	case Keycode::F25: return "F25";
	case Keycode::KPNum0: return "KPNum0";
	case Keycode::KPNum1: return "KPNum1";
	case Keycode::KPNum2: return "KPNum2";
	case Keycode::KPNum3: return "KPNum3";
	case Keycode::KPNum4: return "KPNum4";
	case Keycode::KPNum5: return "KPNum5";
	case Keycode::KPNum6: return "KPNum6";
	case Keycode::KPNum7: return "KPNum7";
	case Keycode::KPNum8: return "KPNum8";
	case Keycode::KPNum9: return "KPNum9";
	case Keycode::KPDot: return "KPDot";
	case Keycode::KPSlash: return "KPSlash";
	case Keycode::KPAsterisk: return "KPAsterisk";
	case Keycode::KPMinus: return "KPMinus";
	case Keycode::KPPlus: return "KPPlus";
	case Keycode::KPEnter: return "KPEnter";
	case Keycode::KPEqual: return "KPEqual";
	case Keycode::ShiftLeft: return "ShiftLeft";
	case Keycode::ControlLeft: return "ControlLeft";
	case Keycode::AltLeft: return "AltLeft";
	case Keycode::SuperLeft: return "SuperLeft";
	case Keycode::ShiftRight: return "ShiftRight";
	case Keycode::ControlRight: return "ControlRight";
	case Keycode::AltRight: return "AltRight";
	case Keycode::SuperRight: return "SuperRight";
	case Keycode::Menu: return "Menu";
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
