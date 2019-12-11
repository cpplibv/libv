// File: events.hpp, Created on 2019. 02. 04. 23:42, Author: Vader

// hpp
#include <libv/input/info_key.hpp>


namespace libv {
namespace input {

// -------------------------------------------------------------------------------------------------

std::array global_table_key{
	InfoKey{Key::Unknown,      "Unknown"},

	InfoKey{Key::Space,        "Space"},
	InfoKey{Key::Apostrophe,   "'"},
	InfoKey{Key::Comma,        ","},
	InfoKey{Key::Minus,        "-"},
	InfoKey{Key::Period,       "."},
	InfoKey{Key::Slash,        "/"},
	InfoKey{Key::Num0,         "0"},
	InfoKey{Key::Num1,         "1"},
	InfoKey{Key::Num2,         "2"},
	InfoKey{Key::Num3,         "3"},
	InfoKey{Key::Num4,         "4"},
	InfoKey{Key::Num5,         "5"},
	InfoKey{Key::Num6,         "6"},
	InfoKey{Key::Num7,         "7"},
	InfoKey{Key::Num8,         "8"},
	InfoKey{Key::Num9,         "9"},
	InfoKey{Key::Semicolon,    ";"},
	InfoKey{Key::Equals,       "="},
	InfoKey{Key::A,            "A"},
	InfoKey{Key::B,            "B"},
	InfoKey{Key::C,            "C"},
	InfoKey{Key::D,            "D"},
	InfoKey{Key::E,            "E"},
	InfoKey{Key::F,            "F"},
	InfoKey{Key::G,            "G"},
	InfoKey{Key::H,            "H"},
	InfoKey{Key::I,            "I"},
	InfoKey{Key::J,            "J"},
	InfoKey{Key::K,            "K"},
	InfoKey{Key::L,            "L"},
	InfoKey{Key::M,            "M"},
	InfoKey{Key::N,            "N"},
	InfoKey{Key::O,            "O"},
	InfoKey{Key::P,            "P"},
	InfoKey{Key::Q,            "Q"},
	InfoKey{Key::R,            "R"},
	InfoKey{Key::S,            "S"},
	InfoKey{Key::T,            "T"},
	InfoKey{Key::U,            "U"},
	InfoKey{Key::V,            "V"},
	InfoKey{Key::W,            "W"},
	InfoKey{Key::X,            "X"},
	InfoKey{Key::Y,            "Y"},
	InfoKey{Key::Z,            "Z"},
	InfoKey{Key::BracketOpen,  "["},
	InfoKey{Key::Backslash,    "\\"},
	InfoKey{Key::BracketClose, "]"},
	InfoKey{Key::Backtick,     "`"},
	InfoKey{Key::World1,       "World1"},
	InfoKey{Key::World2,       "World2"},
	InfoKey{Key::Escape,       "Esc"},
	InfoKey{Key::Enter,        "Enter"},
	InfoKey{Key::Tab,          "Tab"},
	InfoKey{Key::Backspace,    "Backspace"},
	InfoKey{Key::Insert,       "Insert"},
	InfoKey{Key::Delete,       "Delete"},
	InfoKey{Key::Right,        "Right"},
	InfoKey{Key::Left,         "Left"},
	InfoKey{Key::Down,         "Down"},
	InfoKey{Key::Up,           "Up"},
	InfoKey{Key::PageUp,       "Page Up"},
	InfoKey{Key::PageDown,     "Page Down"},
	InfoKey{Key::Home,         "Home"},
	InfoKey{Key::End,          "End"},
	InfoKey{Key::CapsLock,     "Caps Lock"},
	InfoKey{Key::ScrollLock,   "Scroll Lock"},
	InfoKey{Key::NumLock,      "Num Lock"},
	InfoKey{Key::PrintScreen,  "Print Screen"},
	InfoKey{Key::Pause,        "Pause"},
	InfoKey{Key::F1,           "F1"},
	InfoKey{Key::F2,           "F2"},
	InfoKey{Key::F3,           "F3"},
	InfoKey{Key::F4,           "F4"},
	InfoKey{Key::F5,           "F5"},
	InfoKey{Key::F6,           "F6"},
	InfoKey{Key::F7,           "F7"},
	InfoKey{Key::F8,           "F8"},
	InfoKey{Key::F9,           "F9"},
	InfoKey{Key::F10,          "F10"},
	InfoKey{Key::F11,          "F11"},
	InfoKey{Key::F12,          "F12"},
	InfoKey{Key::F13,          "F13"},
	InfoKey{Key::F14,          "F14"},
	InfoKey{Key::F15,          "F15"},
	InfoKey{Key::F16,          "F16"},
	InfoKey{Key::F17,          "F17"},
	InfoKey{Key::F18,          "F18"},
	InfoKey{Key::F19,          "F19"},
	InfoKey{Key::F20,          "F20"},
	InfoKey{Key::F21,          "F21"},
	InfoKey{Key::F22,          "F22"},
	InfoKey{Key::F23,          "F23"},
	InfoKey{Key::F24,          "F24"},
	InfoKey{Key::F25,          "F25"},
	InfoKey{Key::KPNum0,       "KP 0"},
	InfoKey{Key::KPNum1,       "KP 1"},
	InfoKey{Key::KPNum2,       "KP 2"},
	InfoKey{Key::KPNum3,       "KP 3"},
	InfoKey{Key::KPNum4,       "KP 4"},
	InfoKey{Key::KPNum5,       "KP 5"},
	InfoKey{Key::KPNum6,       "KP 6"},
	InfoKey{Key::KPNum7,       "KP 7"},
	InfoKey{Key::KPNum8,       "KP 8"},
	InfoKey{Key::KPNum9,       "KP 9"},
	InfoKey{Key::KPDot,        "KP ."},
	InfoKey{Key::KPSlash,      "KP /"},
	InfoKey{Key::KPAsterisk,   "KP *"},
	InfoKey{Key::KPMinus,      "KP -"},
	InfoKey{Key::KPPlus,       "KP +"},
	InfoKey{Key::KPEnter,      "KP enter"},
	InfoKey{Key::KPEqual,      "KP ="},
	InfoKey{Key::ShiftLeft,    "L Shift"},
	InfoKey{Key::ControlLeft,  "L Ctrl"},
	InfoKey{Key::AltLeft,      "L Alt"},
	InfoKey{Key::SuperLeft,    "L Super"},
	InfoKey{Key::ShiftRight,   "R Shift"},
	InfoKey{Key::ControlRight, "R Ctrl"},
	InfoKey{Key::AltRight,     "R Alt"},
	InfoKey{Key::SuperRight,   "R Super"},
	InfoKey{Key::Menu,         "Menu"}
};

// -------------------------------------------------------------------------------------------------

} // namespace input
} // namespace libv