// Project: libv.ui, File: src/libv/ui/event/event_component.hpp

#pragma once

// pro
#include <libv/ui/event/base_event.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

/// Happens after a component's doAttach is called
struct EventAttach : BaseEvent {};

/// Happens before a component's doDetach is called
struct EventDetach : BaseEvent {};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
