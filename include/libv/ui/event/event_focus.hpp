// File:   interest.hpp Author: Vader Created on 15 April 2019, 06:10

#pragma once

// pro
#include <libv/ui/event/base_event.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct EventFocus : BaseEvent {
	bool focus = false;

public:
	constexpr inline EventFocus(bool focus) noexcept : focus(focus) { }

public:
	[[nodiscard]] constexpr inline bool gain() const noexcept {
		return focus;
	}

	[[nodiscard]] constexpr inline bool loss() const noexcept {
		return !focus;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
