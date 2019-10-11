// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// libv
#include <libv/input/input.hpp>
#include <libv/math/vec.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct BaseEventMouse {
//	libv::vec2f mouse_position;
//	libv::vec2f scroll_position;
//	boost::container::flat_map<libv::input::Mouse, libv::input::Action> mouse_buttons;
	// access to key states
	// access to joystick states
	// access to mouse states
	// access to time states
	// access to hotkey states (?)
};

struct EventMouseButton : BaseEventMouse {
	libv::input::Mouse button;
	libv::input::Action action;
};

struct EventMouseMovement : BaseEventMouse {
	libv::vec2f mouse_position;
	libv::vec2f mouse_movement;

	bool enter = false;
	bool leave = false;
};

struct EventMouseScroll : BaseEventMouse {
	libv::vec2f scroll_position;
	libv::vec2f scroll_movement;
	//TYPE scroll_unit;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
