// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// std
#include <functional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class EventMouse;

struct MouseWatcher {
	std::function<void(const EventMouse&)> callback;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
