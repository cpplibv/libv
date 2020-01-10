// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// libv
#include <libv/input/input.hpp>
#include <libv/math/vec.hpp>
// pro
#include <libv/ui/event/base_event.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct BaseEventMouse : BaseEvent {
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
