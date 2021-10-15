// Project: libv.frame, File: src/libv/frame/events_variant.hpp

#pragma once

// libv
#include <libv/frame/events.hpp>
// std
#include <variant>


namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

using Event = std::variant<
		libv::input::EventChar,
		EventContentScale,
		libv::input::EventDrop,
		EventFocus,
		EventFramebufferSize,
		libv::input::EventKey,
		EventMaximize,
		EventMinimize,
		libv::input::EventMouseButton,
		libv::input::EventMouseEnter,
		libv::input::EventMousePosition,
		libv::input::EventMouseScroll,
		libv::input::EventGamepadAnalog,
		libv::input::EventGamepadButton,
		libv::input::EventJoystickAnalog,
		libv::input::EventJoystickButton,
		EventPosition,
		EventRefresh,
		EventSize
>;

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
