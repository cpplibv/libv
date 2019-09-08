// File:   focus_watcher.hpp Author: Vader Created on 08 September 2019, 07:34

#pragma once

// std
#include <functional>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class EventFocus;

struct FocusWatcher {
	std::function<void(const EventFocus&)> callback;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
