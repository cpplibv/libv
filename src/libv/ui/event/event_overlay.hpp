// Project: libv.ui, File: src/libv/ui/event/event_focus.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/event/base_event.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class EventOverlay : BaseEvent {
private:
	bool controls_intercepted_;

public:
	explicit constexpr inline EventOverlay(bool controls_intercepted_) noexcept :
		controls_intercepted_(controls_intercepted_) { }

public:
	[[nodiscard]] constexpr inline bool controls_intercepted() const noexcept {
		return controls_intercepted_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
