// Project: libv.ui, File: src/libv/ui/event/event_keyboard.hpp

#pragma once

// libv
#include <libv/input/event.hpp>
// pro
#include <libv/ui/event/base_event.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct BaseEventKeyboard : BaseEvent {
};

struct EventKey : BaseEventKeyboard, libv::input::EventKey {
	inline constexpr explicit EventKey(const libv::input::EventKey& event) noexcept :
		libv::input::EventKey(event) {}
};

struct EventChar : BaseEventKeyboard, libv::input::EventChar {
	inline constexpr explicit EventChar(const libv::input::EventChar& event) noexcept :
		libv::input::EventChar(event) {}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
