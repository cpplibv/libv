// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// std
#include <functional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class EventMouseButton;
class EventMouseMovement;
class EventMouseScroll;

struct MouseWatcher {
	std::function<void(const EventMouseButton&)> cb_button;
	std::function<void(const EventMouseMovement&)> cb_movement;
	std::function<void(const EventMouseScroll&)> cb_scroll;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
