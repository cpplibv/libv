// Project: libv.ui, File: src/libv/ui/event/event_focus.hpp

#pragma once

// pro
#include <libv/ui/event/base_event.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class EventFocus : public BaseEvent {
private:
	bool focus_;

public:
	explicit constexpr inline EventFocus(bool focus) noexcept : focus_(focus) { }

public:
	[[nodiscard]] constexpr inline bool gain() const noexcept {
		return focus_;
	}

	[[nodiscard]] constexpr inline bool loss() const noexcept {
		return !focus_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
