// Project: libv.ctrl, File: src/libv/ctrl/info.cpp

// hpp
#include <libv/ctrl/info.hpp>
// ext
#include <fmt/format.h>
#include <fmt/ostream.h>
// libv
#include <libv/utility/utf8.hpp>
// std
#include <array>
#include <ostream>
#include <sstream>
#include <string>
// pro
#include <libv/ctrl/input_id.hpp>
#include <libv/ctrl/input.hpp>
#include <libv/ctrl/combination.hpp>
#include <libv/ctrl/sequence.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

/// Input format string
struct IF {
	std::string_view symbol;
	std::string_view identifier;
	std::string_view name;
	std::string_view description;
};

/// Input information
template <typename Enum>
struct II {
	Enum id;

	std::string_view symbol;
	std::string_view identifier;
	std::string_view name;
	std::string_view description;
};

IDE_IGNORE(template <typename Enum> II(Enum, const char*, const char*, const char*, const char*) -> II<Enum>;)

// -------------------------------------------------------------------------------------------------

static constexpr std::array info_table_keycode{
	II{Keycode::Unknown        , "Unknown"     , "unknown"       , "Unknown"       , "Unknown"       },

	II{Keycode::Space          , "Space"       , "space"         , "Space"         , "Space"         },
	II{Keycode::Apostrophe     , "' "          , "apostrophe"    , "Apostrophe"    , "Apostrophe"    },
	II{Keycode::Comma          , ","           , "comma"         , "Comma"         , "Comma"         },
	II{Keycode::Minus          , "-"           , "minus"         , "Minus"         , "Minus"         },
	II{Keycode::Period         , "."           , "period"        , "Period"        , "Period"        },
	II{Keycode::Slash          , "/"           , "slash"         , "Slash"         , "Slash"         },
	II{Keycode::Num0           , "0"           , "num0"          , "0"             , "0"             },
	II{Keycode::Num1           , "1"           , "num1"          , "1"             , "1"             },
	II{Keycode::Num2           , "2"           , "num2"          , "2"             , "2"             },
	II{Keycode::Num3           , "3"           , "num3"          , "3"             , "3"             },
	II{Keycode::Num4           , "4"           , "num4"          , "4"             , "4"             },
	II{Keycode::Num5           , "5"           , "num5"          , "5"             , "5"             },
	II{Keycode::Num6           , "6"           , "num6"          , "6"             , "6"             },
	II{Keycode::Num7           , "7"           , "num7"          , "7"             , "7"             },
	II{Keycode::Num8           , "8"           , "num8"          , "8"             , "8"             },
	II{Keycode::Num9           , "9"           , "num9"          , "9"             , "9"             },
	II{Keycode::Semicolon      , ";"           , "semicolon"     , "Semicolon"     , "Semicolon"     },
	II{Keycode::Equals         , "="           , "equals"        , "Equals"        , "Equals"        },
	II{Keycode::A              , "A"           , "a"             , "A"             , "A"             },
	II{Keycode::B              , "B"           , "b"             , "B"             , "B"             },
	II{Keycode::C              , "C"           , "c"             , "C"             , "C"             },
	II{Keycode::D              , "D"           , "d"             , "D"             , "D"             },
	II{Keycode::E              , "E"           , "e"             , "E"             , "E"             },
	II{Keycode::F              , "F"           , "f"             , "F"             , "F"             },
	II{Keycode::G              , "G"           , "g"             , "G"             , "G"             },
	II{Keycode::H              , "H"           , "h"             , "H"             , "H"             },
	II{Keycode::I              , "I"           , "i"             , "I"             , "I"             },
	II{Keycode::J              , "J"           , "j"             , "J"             , "J"             },
	II{Keycode::K              , "K"           , "k"             , "K"             , "K"             },
	II{Keycode::L              , "L"           , "l"             , "L"             , "L"             },
	II{Keycode::M              , "M"           , "m"             , "M"             , "M"             },
	II{Keycode::N              , "N"           , "n"             , "N"             , "N"             },
	II{Keycode::O              , "O"           , "o"             , "O"             , "O"             },
	II{Keycode::P              , "P"           , "p"             , "P"             , "P"             },
	II{Keycode::Q              , "Q"           , "q"             , "Q"             , "Q"             },
	II{Keycode::R              , "R"           , "r"             , "R"             , "R"             },
	II{Keycode::S              , "S"           , "s"             , "S"             , "S"             },
	II{Keycode::T              , "T"           , "t"             , "T"             , "T"             },
	II{Keycode::U              , "U"           , "u"             , "U"             , "U"             },
	II{Keycode::V              , "V"           , "v"             , "V"             , "V"             },
	II{Keycode::W              , "W"           , "w"             , "W"             , "W"             },
	II{Keycode::X              , "X"           , "x"             , "X"             , "X"             },
	II{Keycode::Y              , "Y"           , "y"             , "Y"             , "Y"             },
	II{Keycode::Z              , "Z"           , "z"             , "Z"             , "Z"             },
	II{Keycode::BracketOpen    , "["           , "bracket_open"  , "Bracket Open"  , "Bracket Open"  },
	II{Keycode::Backslash      , "\\"          , "backslash"     , "Backslash"     , "Backslash"     },
	II{Keycode::BracketClose   , "]"           , "bracket_close" , "Bracket Close" , "Bracket Close" },
	II{Keycode::Backtick       , "`"           , "backtick"      , "Backtick"      , "Backtick"      },
	II{Keycode::World1         , "W1"          , "world1"        , "World1"        , "World 1"       },
	II{Keycode::World2         , "W2"          , "world2"        , "World2"        , "World 2"       },
	II{Keycode::Escape         , "Esc"         , "escape"        , "Escape"        , "Escape"        },
	II{Keycode::Enter          , "Enter"       , "enter"         , "Enter"         , "Enter"         },
	II{Keycode::Tab            , "Tab"         , "tab"           , "Tab"           , "Tab"           },
	II{Keycode::Backspace      , "Backspace"   , "backspace"     , "Backspace"     , "Backspace"     },
	II{Keycode::Insert         , "Insert"      , "insert"        , "Insert"        , "Insert"        },
	II{Keycode::Delete         , "Delete"      , "delete"        , "Delete"        , "Delete"        },
	II{Keycode::Right          , "Right"       , "right"         , "Right"         , "Right"         },
	II{Keycode::Left           , "Left"        , "left"          , "Left"          , "Left"          },
	II{Keycode::Down           , "Down"        , "down"          , "Down"          , "Down"          },
	II{Keycode::Up             , "Up"          , "up"            , "Up"            , "Up"            },
	II{Keycode::PageUp         , "PgUp"        , "page_up"       , "Page Up"       , "Page Up"       },
	II{Keycode::PageDown       , "PgDown"      , "page_down"     , "Page Down"     , "Page Down"     },
	II{Keycode::Home           , "Home"        , "home"          , "Home"          , "Home"          },
	II{Keycode::End            , "End"         , "end"           , "End"           , "End"           },
	II{Keycode::CapsLock       , "CapsLck"     , "caps_lock"     , "Caps Lock"     , "Caps Lock"     },
	II{Keycode::ScrollLock     , "ScrollLck"   , "scroll_lock"   , "Scroll Lock"   , "Scroll Lock"   },
	II{Keycode::NumLock        , "NumLck"      , "num_lock"      , "Num Lock"      , "Num Lock"      },
	II{Keycode::PrintScreen    , "PrtSc"       , "print_screen"  , "Print Screen"  , "Print Screen"  },
	II{Keycode::Pause          , "Pause"       , "pause"         , "Pause"         , "Pause"         },
	II{Keycode::F1             , "F1"          , "f1"            , "F1"            , "F1"            },
	II{Keycode::F2             , "F2"          , "f2"            , "F2"            , "F2"            },
	II{Keycode::F3             , "F3"          , "f3"            , "F3"            , "F3"            },
	II{Keycode::F4             , "F4"          , "f4"            , "F4"            , "F4"            },
	II{Keycode::F5             , "F5"          , "f5"            , "F5"            , "F5"            },
	II{Keycode::F6             , "F6"          , "f6"            , "F6"            , "F6"            },
	II{Keycode::F7             , "F7"          , "f7"            , "F7"            , "F7"            },
	II{Keycode::F8             , "F8"          , "f8"            , "F8"            , "F8"            },
	II{Keycode::F9             , "F9"          , "f9"            , "F9"            , "F9"            },
	II{Keycode::F10            , "F10"         , "f10"           , "F10"           , "F10"           },
	II{Keycode::F11            , "F11"         , "f11"           , "F11"           , "F11"           },
	II{Keycode::F12            , "F12"         , "f12"           , "F12"           , "F12"           },
	II{Keycode::F13            , "F13"         , "f13"           , "F13"           , "F13"           },
	II{Keycode::F14            , "F14"         , "f14"           , "F14"           , "F14"           },
	II{Keycode::F15            , "F15"         , "f15"           , "F15"           , "F15"           },
	II{Keycode::F16            , "F16"         , "f16"           , "F16"           , "F16"           },
	II{Keycode::F17            , "F17"         , "f17"           , "F17"           , "F17"           },
	II{Keycode::F18            , "F18"         , "f18"           , "F18"           , "F18"           },
	II{Keycode::F19            , "F19"         , "f19"           , "F19"           , "F19"           },
	II{Keycode::F20            , "F20"         , "f20"           , "F20"           , "F20"           },
	II{Keycode::F21            , "F21"         , "f21"           , "F21"           , "F21"           },
	II{Keycode::F22            , "F22"         , "f22"           , "F22"           , "F22"           },
	II{Keycode::F23            , "F23"         , "f23"           , "F23"           , "F23"           },
	II{Keycode::F24            , "F24"         , "f24"           , "F24"           , "F24"           },
	II{Keycode::F25            , "F25"         , "f25"           , "F25"           , "F25"           },
	II{Keycode::KPNum0         , "KP0"         , "kp_num0"       , "KP 0"          , "KP 0"          },
	II{Keycode::KPNum1         , "KP1"         , "kp_num1"       , "KP 1"          , "KP 1"          },
	II{Keycode::KPNum2         , "KP2"         , "kp_num2"       , "KP 2"          , "KP 2"          },
	II{Keycode::KPNum3         , "KP3"         , "kp_num3"       , "KP 3"          , "KP 3"          },
	II{Keycode::KPNum4         , "KP4"         , "kp_num4"       , "KP 4"          , "KP 4"          },
	II{Keycode::KPNum5         , "KP5"         , "kp_num5"       , "KP 5"          , "KP 5"          },
	II{Keycode::KPNum6         , "KP6"         , "kp_num6"       , "KP 6"          , "KP 6"          },
	II{Keycode::KPNum7         , "KP7"         , "kp_num7"       , "KP 7"          , "KP 7"          },
	II{Keycode::KPNum8         , "KP8"         , "kp_num8"       , "KP 8"          , "KP 8"          },
	II{Keycode::KPNum9         , "KP9"         , "kp_num9"       , "KP 9"          , "KP 9"          },
	II{Keycode::KPDot          , "KP."         , "kp_dot"        , "KP Dot"        , "KP Dot"        },
	II{Keycode::KPSlash        , "KP/"         , "kp_slash"      , "KP Slash"      , "KP Slash"      },
	II{Keycode::KPAsterisk     , "KP*"         , "kp_asterisk"   , "KP Asterisk"   , "KP Asterisk"   },
	II{Keycode::KPMinus        , "KP-"         , "kp_minus"      , "KP Minus"      , "KP Minus"      },
	II{Keycode::KPPlus         , "KP+"         , "kp_plus"       , "KP Plus"       , "KP Plus"       },
	II{Keycode::KPEnter        , "KPEnter"     , "kp_enter"      , "KP Enter"      , "KP Enter"      },
	II{Keycode::KPEqual        , "KP="         , "kp_equal"      , "KP Equal"      , "KP Equal"      },
	II{Keycode::ShiftLeft      , "LShift"      , "left_shift"    , "LShift"        , "Left Shift"    },
	II{Keycode::ControlLeft    , "LCtrl"       , "left_control"  , "LCtrl"         , "Left Control"  },
	II{Keycode::AltLeft        , "LAlt"        , "left_alt"      , "LAlt"          , "Left Alt"      },
	II{Keycode::SuperLeft      , "LSuper"      , "left_super"    , "LSuper"        , "Left Super"    },
	II{Keycode::ShiftRight     , "RShift"      , "right_shift"   , "RShift"        , "Right Shift"   },
	II{Keycode::ControlRight   , "RCtrl"       , "right_control" , "RCtrl"         , "Right Control" },
	II{Keycode::AltRight       , "RAlt"        , "right_alt"     , "RAlt"          , "Right Alt"     },
	II{Keycode::SuperRight     , "RSuper"      , "right_super"   , "RSuper"        , "Right Super"   },
	II{Keycode::Menu           , "Menu"        , "menu"          , "Menu"          , "Menu"          },
};

static constexpr std::array info_table_modifier{
	II{Modifier::shift         , "Shift"       , "Shift"         , "Shift"         , "Shift"         },
	II{Modifier::control       , "Ctrl"        , "Control"       , "Control"       , "Control"       },
	II{Modifier::alt           , "Alt"         , "Alt"           , "Alt"           , "Alt"           },
	II{Modifier::super         , "Super"       , "Super"         , "Super"         , "Super"         },
};

static constexpr std::array info_table_mouse_button{
	II{MouseButton::Left       , "LMB"         , "mouse_left"    , "LMouse"        , "Left Mouse"    },
	II{MouseButton::Right      , "RMB"         , "mouse_right"   , "RMouse"        , "Right Mouse"   },
	II{MouseButton::Middle     , "MMB"         , "mouse_middle"  , "MMouse"        , "Middle Mouse"  },
};

static constexpr std::array info_table_mouse_move{
	II{MouseMovement::x        , "MX"          , "mouse_x"       , "MouseX"        , "Mouse X"       },
	II{MouseMovement::x_plus   , "MX+"         , "mouse_x_plus"  , "MouseX+"       , "Mouse X+"      },
	II{MouseMovement::x_minus  , "MX-"         , "mouse_x_minus" , "MouseX-"       , "Mouse X-"      },
	II{MouseMovement::y        , "MY"          , "mouse_y"       , "MouseY"        , "Mouse Y"       },
	II{MouseMovement::y_plus   , "MY+"         , "mouse_y_plus"  , "MouseY+"       , "Mouse Y+"      },
	II{MouseMovement::y_minus  , "MY-"         , "mouse_y_minus" , "MouseY-"       , "Mouse Y-"      },
};

static constexpr std::array info_table_mouse_scroll{
	II{MouseScroll::x          , "MSX"         , "scroll_x"      , "Scroll X"      , "Scroll X"      },
	II{MouseScroll::x_plus     , "MSX+"        , "scroll_x_plus" , "Scroll X+"     , "Scroll X+"     },
	II{MouseScroll::x_minus    , "MSX-"        , "scroll_x_minus", "Scroll X-"     , "Scroll X-"     },
	II{MouseScroll::y          , "MS"          , "scroll_y"      , "Scroll"        , "Scroll Y"      },
	II{MouseScroll::y_plus     , "MS+"         , "scroll_y_plus" , "Scroll+"       , "Scroll Y+"     },
	II{MouseScroll::y_minus    , "MS-"         , "scroll_y_minus", "Scroll-"       , "Scroll Y-"     },
};

static constexpr std::array info_table_gamepad_button{
	II{GamepadButton::A          , "A"         , "a"             , "A"             , "A"             },
	II{GamepadButton::B          , "B"         , "b"             , "B"             , "B"             },
	II{GamepadButton::X          , "X"         , "x"             , "X"             , "X"             },
	II{GamepadButton::Y          , "Y"         , "y"             , "Y"             , "Y"             },
	II{GamepadButton::LeftBumper , "LB"        , "left_bumper"   , "LeftBumper"    , "Left Bumper"   },
	II{GamepadButton::RightBumper, "RB"        , "right_bumper"  , "RightBumper"   , "Right Bumper"  },
	II{GamepadButton::Back       , "Back"      , "back"          , "Back"          , "Back"          },
	II{GamepadButton::Start      , "Start"     , "start"         , "Start"         , "Start"         },
	II{GamepadButton::Guide      , "Guide"     , "guide"         , "Guide"         , "Guide"         },
	II{GamepadButton::LeftThumb  , "LT"        , "left_thumb"    , "LeftThumb"     , "Left Thumb"    },
	II{GamepadButton::RightThumb , "RT"        , "right_thumb"   , "RightThumb"    , "Right Thumb"   },
	II{GamepadButton::DPadUp     , "DUp"       , "dpad_up"       , "DPadUp"        , "DPad Up"       },
	II{GamepadButton::DPadRight  , "DRight"    , "dpad_right"    , "DPadRight"     , "DPad Right"    },
	II{GamepadButton::DPadDown   , "DDown"     , "dpad_down"     , "DPadDown"      , "DPad Down"     },
	II{GamepadButton::DPadLeft   , "DLeft"     , "dpad_left"     , "DPadLeft"      , "DPad Left"     },
};

static constexpr std::array info_table_analog_dimension{
	II{AnalogDimension::x      , "X"           , "x"             , "X"             , "X"             },
	II{AnalogDimension::x_plus , "X+"          , "x_plus"        , "X+"            , "X+"            },
	II{AnalogDimension::x_minus, "X-"          , "x_minus"       , "X-"            , "X-"            },
	II{AnalogDimension::y      , "Y"           , "y"             , "Y"             , "Y"             },
	II{AnalogDimension::y_plus , "Y+"          , "y_plus"        , "Y+"            , "Y+"            },
	II{AnalogDimension::y_minus, "Y-"          , "y_minus"       , "Y-"            , "Y-"            },
};

static constexpr auto fmt_input_dia_open      = IF{"["        , "_"                     , " ["                    , " ["                      };
static constexpr auto fmt_input_dia_close     = IF{"]"        , "_"                     , "]"                     , "]"                       };

static constexpr auto fmt_combination_sep     = IF{"+"        , "_and_"                 , " + "                   , " + "                     };

static constexpr auto fmt_sequence_sep        = IF{","        , "_then_"                , ", "                    , ", "                     };

static constexpr auto fmt_invalid             = IF{"<?>"      , "__invalid__"           , "<<invalid>>"           , "Invalid"                 };

static constexpr auto fmt_codepoint           = IF{"\"{}\""   , "\"{}\""                , "\"{}\""                , "Codepoint \"{}\""        };
static constexpr auto fmt_codepoint_s         = IF{"'{}'"     , "'{}'"                  , "'{}'"                  , "Codepoint '{}'"          };
static constexpr auto fmt_scancode            = IF{"sc{}"     , "sc{}"                  , "scan{}"                , "Scancode {}"             };
static constexpr auto fmt_mouse_button        = IF{"MB{}"     , "mouse{}"               , "Mouse{}"               , "Mouse Button {}"         };
static constexpr auto fmt_gamepad_button      = IF{"G{}B{}"   , "gamepad{}_{}"          , "Gamepad{}Button{}"     , "Gamepad {} Button {}"    };
static constexpr auto fmt_gamepad_any_button  = IF{"GB{}"     , "gamepad_{}"            , "GamepadButton{}"       , "Gamepad Button {}"       };
static constexpr auto fmt_gamepad_analog      = IF{"G{}A{}:{}", "gamepad{}_analog{}_{}" , "Gamepad{}Analog{}:{}"  , "Gamepad {} Analog {} {}" };
static constexpr auto fmt_gamepad_any_analog  = IF{"GA{}:{}"  , "gamepad_analog{}_{}"   , "GamepadAnalog{}:{}"    , "Gamepad Analog {} {}"    };
static constexpr auto fmt_joystick_button     = IF{"J{}B{}"   , "joystick{}_button{}"   , "Joystick{}Button{}"    , "Joystick {} Button {}"   };
static constexpr auto fmt_joystick_any_button = IF{"JB{}"     , "joystick_button{}"     , "JoystickButton{}"      , "Joystick Button {}"      };
static constexpr auto fmt_joystick_analog     = IF{"J{}A{}:{}", "joystick{}_analog{}_{}", "Joystick{}Analog{}:{}" , "Joystick {} Analog {} {}"};
static constexpr auto fmt_joystick_any_analog = IF{"JA{}:{}"  , "joystick_analog{}_{}"  , "JoystickAnalog{}:{}"   , "Joystick Analog {} {}"   };


// -------------------------------------------------------------------------------------------------

template <auto Select, typename Table, typename T>
inline std::ostream& table_or_invalid(std::ostream& os, const Table& table, const T key) {
	for (const auto& info : table)
		if (info.id == key)
			return os << Select(info);

	return os << Select(fmt_invalid);
}

template <auto Select, typename Table, typename T>
inline std::string table_or_invalid(const Table& table, const T key) {
	for (const auto& info : table)
		if (info.id == key)
			return std::string{Select(info)};

	return std::string(Select(fmt_invalid));
}

// -------------------------------------------------------------------------------------------------

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, Keycode keycode) {
	return table_or_invalid<Select>(os, info_table_keycode, keycode);
}

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, Codepoint codepoint) {
	if (codepoint == Codepoint{'"'})
		return fmt::print(os, Select(fmt_codepoint_s), libv::unicode_to_utf8(libv::to_value(codepoint))), os;
	else
		return fmt::print(os, Select(fmt_codepoint), libv::unicode_to_utf8(libv::to_value(codepoint))), os;
}

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, Scancode scancode) {
	return fmt::print(os, Select(fmt_scancode), libv::to_value(scancode)), os;
}

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, Modifier modifier) {
	return table_or_invalid<Select>(os, info_table_modifier, modifier);
}

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, MouseButton button) {
	for (const auto& info : info_table_mouse_button)
		if (info.id == button)
			return os << Select(info);

	return fmt::print(os, Select(fmt_mouse_button), libv::to_value(button)), os;
}

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, MouseMovement movement) {
	return table_or_invalid<Select>(os, info_table_mouse_move, movement);
}

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, MouseScroll scroll) {
	return table_or_invalid<Select>(os, info_table_mouse_scroll, scroll);
}

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, GamepadID gamepad, GamepadButton button) {
	for (const auto& info : info_table_gamepad_button)
		if (info.id == button) {
			if (gamepad == GamepadID::any)
				return fmt::print(os, Select(fmt_gamepad_any_button), Select(info)), os;
			else
				return fmt::print(os, Select(fmt_gamepad_button), libv::to_value(gamepad), Select(info)), os;
		}

	return os << Select(fmt_invalid);
}

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, GamepadID gamepad, GamepadAnalogID analogID, AnalogDimension dim) {
	for (const auto& info : info_table_analog_dimension)
		if (info.id == dim) {
			if (gamepad == GamepadID::any)
				return fmt::print(os, Select(fmt_gamepad_any_analog), libv::to_value(analogID), Select(info)), os;
			else
				return fmt::print(os, Select(fmt_gamepad_analog), libv::to_value(gamepad), libv::to_value(analogID), Select(info)), os;
		}

	return os << Select(fmt_invalid);
}

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, JoystickID joystick, JoystickButton button) {
	if (joystick == JoystickID::any)
		return fmt::print(os, Select(fmt_joystick_any_button), libv::to_value(button)), os;
	else
		return fmt::print(os, Select(fmt_joystick_button), libv::to_value(joystick), libv::to_value(button)), os;
}

template <auto Select>
inline std::ostream& aux_to_stream(std::ostream& os, JoystickID joystick, JoystickAnalogID analogID, AnalogDimension dim) {
	for (const auto& info : info_table_analog_dimension)
		if (info.id == dim) {
			if (joystick == JoystickID::any)
				return fmt::print(os, Select(fmt_joystick_any_analog), libv::to_value(analogID), Select(info)), os;
			else
				return fmt::print(os, Select(fmt_joystick_analog), libv::to_value(joystick), libv::to_value(analogID), Select(info)), os;
		}

	return os << Select(fmt_invalid);
}

// -------------------------------------------------------------------------------------------------

template <auto Select>
inline std::string aux_to_string(Keycode keycode) {
	return table_or_invalid<Select>(info_table_keycode, keycode);
}

template <auto Select>
inline std::string aux_to_string(Codepoint codepoint) {
	if (codepoint != Codepoint{'"'})
		return fmt::format(Select(fmt_codepoint), libv::unicode_to_utf8(libv::to_value(codepoint)));
	else
		return fmt::format(Select(fmt_codepoint_s), libv::unicode_to_utf8(libv::to_value(codepoint)));
}

template <auto Select>
inline std::string aux_to_string(Scancode scancode) {
	return fmt::format(Select(fmt_scancode), libv::to_value(scancode));
}

template <auto Select>
inline std::string aux_to_string(Modifier modifier) {
	return table_or_invalid<Select>(info_table_modifier, modifier);
}

template <auto Select>
inline std::string aux_to_string(MouseButton button) {
	for (const auto& info : info_table_mouse_button)
		if (info.id == button)
			return std::string(Select(info));

	return fmt::format(Select(fmt_mouse_button), libv::to_value(button));
}

template <auto Select>
inline std::string aux_to_string(MouseMovement movement) {
	return table_or_invalid<Select>(info_table_mouse_move, movement);
}

template <auto Select>
inline std::string aux_to_string(MouseScroll scroll) {
	return table_or_invalid<Select>(info_table_mouse_scroll, scroll);
}

template <auto Select>
inline std::string aux_to_string(GamepadID gamepad, GamepadButton button) {
	for (const auto& info : info_table_gamepad_button)
		if (info.id == button) {
			if (gamepad == GamepadID::any)
				return fmt::format(Select(fmt_gamepad_any_button), Select(info));
			else
				return fmt::format(Select(fmt_gamepad_button), libv::to_value(gamepad), Select(info));
		}

	return std::string(Select(fmt_invalid));
}

template <auto Select>
inline std::string aux_to_string(GamepadID gamepad, GamepadAnalogID analogID, AnalogDimension dim) {
	for (const auto& info : info_table_analog_dimension)
		if (info.id == dim) {
			if (gamepad == GamepadID::any)
				return fmt::format(Select(fmt_gamepad_any_analog), libv::to_value(analogID), Select(info));
			else
				return fmt::format(Select(fmt_gamepad_analog), libv::to_value(gamepad), libv::to_value(analogID), Select(info));
		}

	return std::string(Select(fmt_invalid));
}

template <auto Select>
inline std::string aux_to_string(JoystickID joystick, JoystickButton button) {
	if (joystick == JoystickID::any)
		return fmt::format(Select(fmt_joystick_any_button), libv::to_value(button));
	else
		return fmt::format(Select(fmt_joystick_button), libv::to_value(joystick), libv::to_value(button));
}

template <auto Select>
inline std::string aux_to_string(JoystickID joystick, JoystickAnalogID analogID, AnalogDimension dim) {
	for (const auto& info : info_table_analog_dimension)
		if (info.id == dim) {
			if (joystick == JoystickID::any)
				return fmt::format(Select(fmt_joystick_any_analog), libv::to_value(analogID), Select(info));
			else
				return fmt::format(Select(fmt_joystick_analog), libv::to_value(joystick), libv::to_value(analogID), Select(info));
		}

	return std::string(Select(fmt_invalid));
}

// -------------------------------------------------------------------------------------------------

static constexpr auto select_symbol = [](const auto& var) -> const auto& {
	return var.symbol;
};
static constexpr auto select_identifier = [](const auto& var) -> const auto& {
	return var.identifier;
};
static constexpr auto select_name = [](const auto& var) -> const auto& {
	return var.name;
};
static constexpr auto select_description = [](const auto& var) -> const auto& {
	return var.description;
};

template <auto Select, typename F>
inline std::string dispatch_inputID_type(InputID inputID, F&& func) {
	switch (inputID.type()) {
	case SourceType::invalid: return std::string(Select(fmt_invalid));

	case SourceType::codepoint: return func(inputID.codepoint());
	case SourceType::keycode: return func(inputID.keycode());
	case SourceType::scancode: return func(inputID.scancode());
	case SourceType::modifier: return func(inputID.modifier());

	case SourceType::mouseButton: return func(inputID.mouseButton());
	case SourceType::mouseMovement: return func(inputID.mouseMovement());
	case SourceType::mouseScroll: return func(inputID.mouseScroll());

	case SourceType::gamepadButton: return func(inputID.gamepadID(), inputID.gamepadButton());
	case SourceType::gamepadAnalog: return func(inputID.gamepadID(), inputID.gamepadAnalogID(), inputID.analogDimension());

	case SourceType::joystickButton: return func(inputID.joystickID(), inputID.joystickButton());
	case SourceType::joystickAnalog: return func(inputID.joystickID(), inputID.joystickAnalogID(), inputID.analogDimension());
	}

	assert(false && "Invalid source type");
	return std::string(Select(fmt_invalid));
}

template <auto Select, typename F>
inline std::ostream& dispatch_inputID_type(std::ostream& os, InputID inputID, F&& func) {
	switch (inputID.type()) {
	case SourceType::invalid: return os << Select(fmt_invalid);

	case SourceType::codepoint: return func(os, inputID.codepoint());
	case SourceType::keycode: return func(os, inputID.keycode());
	case SourceType::scancode: return func(os, inputID.scancode());
	case SourceType::modifier: return func(os, inputID.modifier());

	case SourceType::mouseButton: return func(os, inputID.mouseButton());
	case SourceType::mouseMovement: return func(os, inputID.mouseMovement());
	case SourceType::mouseScroll: return func(os, inputID.mouseScroll());

	case SourceType::gamepadButton: return func(os, inputID.gamepadID(), inputID.gamepadButton());
	case SourceType::gamepadAnalog: return func(os, inputID.gamepadID(), inputID.gamepadAnalogID(), inputID.analogDimension());

	case SourceType::joystickButton: return func(os, inputID.joystickID(), inputID.joystickButton());
	case SourceType::joystickAnalog: return func(os, inputID.joystickID(), inputID.joystickAnalogID(), inputID.analogDimension());
	}

	assert(false && "Invalid source type");
	return os << Select(fmt_invalid);
}

// -------------------------------------------------------------------------------------------------

std::ostream& to_stream_symbol(std::ostream& os, const InputID inputID) {
	return dispatch_inputID_type<select_symbol>(os, inputID, [](std::ostream& osi, auto... args) -> std::ostream& {
		return aux_to_stream<select_symbol>(osi, args...);
	});
}

std::ostream& to_stream_identifier(std::ostream& os, const InputID inputID) {
	return dispatch_inputID_type<select_identifier>(os, inputID, [](std::ostream& osi, auto... args) -> std::ostream& {
		return aux_to_stream<select_identifier>(osi, args...);
	});
}

std::ostream& to_stream_name(std::ostream& os, const InputID inputID) {
	return dispatch_inputID_type<select_name>(os, inputID, [](std::ostream& osi, auto... args) -> std::ostream& {
		return aux_to_stream<select_name>(osi, args...);
	});
}

std::ostream& to_stream_description(std::ostream& os, const InputID inputID) {
	return dispatch_inputID_type<select_description>(os, inputID, [](std::ostream& osi, auto... args) -> std::ostream& {
		return aux_to_stream<select_description>(osi, args...);
	});
}

std::string to_string_symbol(const InputID inputID) {
	return dispatch_inputID_type<select_symbol>(inputID, [](auto... args) {
		return aux_to_string<select_symbol>(args...);
	});
}

std::string to_string_identifier(const InputID inputID) {
	return dispatch_inputID_type<select_identifier>(inputID, [](auto... args) {
		return aux_to_string<select_identifier>(args...);
	});
}

std::string to_string_name(const InputID inputID) {
	return dispatch_inputID_type<select_name>(inputID, [](auto... args) {
		return aux_to_string<select_name>(args...);
	});
}

std::string to_string_description(const InputID inputID) {
	return dispatch_inputID_type<select_description>(inputID, [](auto... args) {
		return aux_to_string<select_description>(args...);
	});
}

// -------------------------------------------------------------------------------------------------

std::ostream& to_stream_symbol(std::ostream& os, const Input input) {
	to_stream_symbol(os, input.id);
	if (input.dia != DigitalInputAction::auto_)
		return os << fmt_input_dia_open.symbol << to_string(input.dia) << fmt_input_dia_close.symbol;
	else
		return os;
}

std::ostream& to_stream_name(std::ostream& os, const Input input) {
	to_stream_name(os, input.id);
	if (input.dia != DigitalInputAction::auto_)
		return os << fmt_input_dia_open.name << to_string(input.dia) << fmt_input_dia_close.name;
	else
		return os;
}

std::string to_string_symbol(const Input input) {
	std::ostringstream ss;
	to_stream_symbol(ss, input);
	return std::move(ss).str();
}

std::string to_string_name(const Input input) {
	std::ostringstream ss;
	to_stream_name(ss, input);
	return std::move(ss).str();
}

// -------------------------------------------------------------------------------------------------

std::ostream& to_stream_symbol(std::ostream& os, const Combination& combination) {
	bool first = true;

	for (const Input& input : combination.inputs) {
		if (!first)
			os << fmt_combination_sep.symbol;

		to_stream_symbol(os, input);
		first = false;
	}

	return os;
}

std::ostream& to_stream_name(std::ostream& os, const Combination& combination) {
	bool first = true;

	for (const Input& input : combination.inputs) {
		if (!first)
			os << fmt_combination_sep.name;

		to_stream_name(os, input);
		first = false;
	}

	return os;
}

std::string to_string_symbol(const Combination& combination) {
	std::ostringstream ss;
	to_stream_symbol(ss, combination);
	return std::move(ss).str();
}

std::string to_string_name(const Combination& combination) {
	std::ostringstream ss;
	to_stream_name(ss, combination);
	return std::move(ss).str();
}

// -------------------------------------------------------------------------------------------------

std::ostream& to_stream_symbol(std::ostream& os, const Sequence& sequence) {
	bool first = true;

	for (const Combination& combination : sequence.combinations) {
		if (!first)
			os << fmt_sequence_sep.symbol;

		to_stream_symbol(os, combination);
		first = false;
	}

	return os;
}

std::ostream& to_stream_name(std::ostream& os, const Sequence& sequence) {
	bool first = true;

	for (const Combination& combination : sequence.combinations) {
		if (!first)
			os << fmt_sequence_sep.name;

		to_stream_name(os, combination);
		first = false;
	}

	return os;
}

std::string to_string_symbol(const Sequence& sequence) {
	std::ostringstream ss;
	to_stream_symbol(ss, sequence);
	return std::move(ss).str();
}

std::string to_string_name(const Sequence& sequence) {
	std::ostringstream ss;
	to_stream_name(ss, sequence);
	return std::move(ss).str();
}

// -------------------------------------------------------------------------------------------------

std::ostream& to_symbol(std::ostream& os, Keycode keycode) {
	return aux_to_stream<select_symbol>(os, keycode);
}
std::ostream& to_symbol(std::ostream& os, Codepoint codepoint) {
	return aux_to_stream<select_symbol>(os, codepoint);
}
std::ostream& to_symbol(std::ostream& os, Scancode scancode) {
	return aux_to_stream<select_symbol>(os, scancode);
}
std::ostream& to_symbol(std::ostream& os, Modifier modifier) {
	return aux_to_stream<select_symbol>(os, modifier);
}
std::ostream& to_symbol(std::ostream& os, MouseButton button) {
	return aux_to_stream<select_symbol>(os, button);
}
std::ostream& to_symbol(std::ostream& os, MouseMovement movement) {
	return aux_to_stream<select_symbol>(os, movement);
}
std::ostream& to_symbol(std::ostream& os, MouseScroll scroll) {
	return aux_to_stream<select_symbol>(os, scroll);
}
std::ostream& to_symbol(std::ostream& os, JoystickID joystick, JoystickButton button) {
	return aux_to_stream<select_symbol>(os, joystick, button);
}
std::ostream& to_symbol(std::ostream& os, JoystickID joystick, JoystickAnalogID id, AnalogDimension dim) {
	return aux_to_stream<select_symbol>(os, joystick, id, dim);
}

std::ostream& to_identifier(std::ostream& os, Keycode keycode) {
	return aux_to_stream<select_identifier>(os, keycode);
}
std::ostream& to_identifier(std::ostream& os, Codepoint codepoint) {
	return aux_to_stream<select_identifier>(os, codepoint);
}
std::ostream& to_identifier(std::ostream& os, Scancode scancode) {
	return aux_to_stream<select_identifier>(os, scancode);
}
std::ostream& to_identifier(std::ostream& os, Modifier modifier) {
	return aux_to_stream<select_identifier>(os, modifier);
}
std::ostream& to_identifier(std::ostream& os, MouseButton button) {
	return aux_to_stream<select_identifier>(os, button);
}
std::ostream& to_identifier(std::ostream& os, MouseMovement movement) {
	return aux_to_stream<select_identifier>(os, movement);
}
std::ostream& to_identifier(std::ostream& os, MouseScroll scroll) {
	return aux_to_stream<select_identifier>(os, scroll);
}
std::ostream& to_identifier(std::ostream& os, JoystickID joystick, JoystickButton button) {
	return aux_to_stream<select_identifier>(os, joystick, button);
}
std::ostream& to_identifier(std::ostream& os, JoystickID joystick, JoystickAnalogID id, AnalogDimension dim) {
	return aux_to_stream<select_identifier>(os, joystick, id, dim);
}

std::ostream& to_name(std::ostream& os, Keycode keycode) {
	return aux_to_stream<select_name>(os, keycode);
}
std::ostream& to_name(std::ostream& os, Codepoint codepoint) {
	return aux_to_stream<select_name>(os, codepoint);
}
std::ostream& to_name(std::ostream& os, Scancode scancode) {
	return aux_to_stream<select_name>(os, scancode);
}
std::ostream& to_name(std::ostream& os, Modifier modifier) {
	return aux_to_stream<select_name>(os, modifier);
}
std::ostream& to_name(std::ostream& os, MouseButton button) {
	return aux_to_stream<select_name>(os, button);
}
std::ostream& to_name(std::ostream& os, MouseMovement movement) {
	return aux_to_stream<select_name>(os, movement);
}
std::ostream& to_name(std::ostream& os, MouseScroll scroll) {
	return aux_to_stream<select_name>(os, scroll);
}
std::ostream& to_name(std::ostream& os, JoystickID joystick, JoystickButton button) {
	return aux_to_stream<select_name>(os, joystick, button);
}
std::ostream& to_name(std::ostream& os, JoystickID joystick, JoystickAnalogID id, AnalogDimension dim) {
	return aux_to_stream<select_name>(os, joystick, id, dim);
}

std::ostream& to_description(std::ostream& os, Keycode keycode) {
	return aux_to_stream<select_description>(os, keycode);
}
std::ostream& to_description(std::ostream& os, Codepoint codepoint) {
	return aux_to_stream<select_description>(os, codepoint);
}
std::ostream& to_description(std::ostream& os, Scancode scancode) {
	return aux_to_stream<select_description>(os, scancode);
}
std::ostream& to_description(std::ostream& os, Modifier modifier) {
	return aux_to_stream<select_description>(os, modifier);
}
std::ostream& to_description(std::ostream& os, MouseButton button) {
	return aux_to_stream<select_description>(os, button);
}
std::ostream& to_description(std::ostream& os, MouseMovement movement) {
	return aux_to_stream<select_description>(os, movement);
}
std::ostream& to_description(std::ostream& os, MouseScroll scroll) {
	return aux_to_stream<select_description>(os, scroll);
}
std::ostream& to_description(std::ostream& os, JoystickID joystick, JoystickButton button) {
	return aux_to_stream<select_description>(os, joystick, button);
}
std::ostream& to_description(std::ostream& os, JoystickID joystick, JoystickAnalogID id, AnalogDimension dim) {
	return aux_to_stream<select_description>(os, joystick, id, dim);
}

// ---

std::string to_symbol(Keycode keycode) {
	return aux_to_string<select_symbol>(keycode);
}
std::string to_symbol(Codepoint codepoint) {
	return aux_to_string<select_symbol>(codepoint);
}
std::string to_symbol(Scancode scancode) {
	return aux_to_string<select_symbol>(scancode);
}
std::string to_symbol(Modifier modifier) {
	return aux_to_string<select_symbol>(modifier);
}
std::string to_symbol(MouseButton button) {
	return aux_to_string<select_symbol>(button);
}
std::string to_symbol(MouseMovement movement) {
	return aux_to_string<select_symbol>(movement);
}
std::string to_symbol(MouseScroll scroll) {
	return aux_to_string<select_symbol>(scroll);
}
std::string to_symbol(JoystickID joystick, JoystickButton button) {
	return aux_to_string<select_symbol>(joystick, button);
}
std::string to_symbol(JoystickID joystick, JoystickAnalogID id, AnalogDimension dim) {
	return aux_to_string<select_symbol>(joystick, id, dim);
}

std::string to_identifier(Keycode keycode) {
	return aux_to_string<select_identifier>(keycode);
}
std::string to_identifier(Codepoint codepoint) {
	return aux_to_string<select_identifier>(codepoint);
}
std::string to_identifier(Scancode scancode) {
	return aux_to_string<select_identifier>(scancode);
}
std::string to_identifier(Modifier modifier) {
	return aux_to_string<select_identifier>(modifier);
}
std::string to_identifier(MouseButton button) {
	return aux_to_string<select_identifier>(button);
}
std::string to_identifier(MouseMovement movement) {
	return aux_to_string<select_identifier>(movement);
}
std::string to_identifier(MouseScroll scroll) {
	return aux_to_string<select_identifier>(scroll);
}
std::string to_identifier(JoystickID joystick, JoystickButton button) {
	return aux_to_string<select_identifier>(joystick, button);
}
std::string to_identifier(JoystickID joystick, JoystickAnalogID id, AnalogDimension dim) {
	return aux_to_string<select_identifier>(joystick, id, dim);
}

std::string to_name(Keycode keycode) {
	return aux_to_string<select_name>(keycode);
}
std::string to_name(Codepoint codepoint) {
	return aux_to_string<select_name>(codepoint);
}
std::string to_name(Scancode scancode) {
	return aux_to_string<select_name>(scancode);
}
std::string to_name(Modifier modifier) {
	return aux_to_string<select_name>(modifier);
}
std::string to_name(MouseButton button) {
	return aux_to_string<select_name>(button);
}
std::string to_name(MouseMovement movement) {
	return aux_to_string<select_name>(movement);
}
std::string to_name(MouseScroll scroll) {
	return aux_to_string<select_name>(scroll);
}
std::string to_name(JoystickID joystick, JoystickButton button) {
	return aux_to_string<select_name>(joystick, button);
}
std::string to_name(JoystickID joystick, JoystickAnalogID id, AnalogDimension dim) {
	return aux_to_string<select_name>(joystick, id, dim);
}

std::string to_description(Keycode keycode) {
	return aux_to_string<select_description>(keycode);
}
std::string to_description(Codepoint codepoint) {
	return aux_to_string<select_description>(codepoint);
}
std::string to_description(Scancode scancode) {
	return aux_to_string<select_description>(scancode);
}
std::string to_description(Modifier modifier) {
	return aux_to_string<select_description>(modifier);
}
std::string to_description(MouseButton button) {
	return aux_to_string<select_description>(button);
}
std::string to_description(MouseMovement movement) {
	return aux_to_string<select_description>(movement);
}
std::string to_description(MouseScroll scroll) {
	return aux_to_string<select_description>(scroll);
}
std::string to_description(JoystickID joystick, JoystickButton button) {
	return aux_to_string<select_description>(joystick, button);
}
std::string to_description(JoystickID joystick, JoystickAnalogID id, AnalogDimension dim) {
	return aux_to_string<select_description>(joystick, id, dim);
}

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
