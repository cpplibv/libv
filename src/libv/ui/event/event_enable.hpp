// Project: libv.ui, File: src/libv/ui/event/event_enable.hpp

#pragma once

// pro
#include <libv/ui/event/base_event.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class EventEnable : BaseEvent {
private:
	bool enable_;

public:
	explicit constexpr inline EventEnable(bool enable_) noexcept :
		enable_(enable_) { }

public:
	[[nodiscard]] constexpr inline bool enable() const noexcept {
		return enable_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
