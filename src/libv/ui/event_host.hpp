// Project: libv.ui, File: src/libv/ui/event_host.hpp

#pragma once

// pro
#include <libv/ui/basic_event_proxy.hpp>
#include <libv/ui/event/base_event.hpp>
#include <libv/ui/event/event_component.hpp>
#include <libv/ui/event/event_enable.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

// IDEA from nana:
//
//Mouse events
//	mouse_enter     - Cursor enters or leaves the component
//	mouse_move      - Cursor moves over the component
//	mouse_button    - User presses the mouse button
//	mouse_wheel     - Mouse wheel rotates while (?) the component was focused
//	drop            - Mouse drops some external data while (?)
//
//AUX Mouse events
//	click           - (?) The click event occurs after mouse_down and before mouse_up
//	dbl_click       - (?)
//
//Keyboard events
//	key             - A key is pressed, released or repeated while the component has the focus
//	key_char        - A character input while the component has the focus
//	shortkey        - (?) A defined short key is pressed
//
//Status events
//	expose          - Visibility changes
//	focus           - Component receives or loses keyboard focus
//	move            - Component changes position
//	resizing        - Window is changing its size
//	resized         - Window is changing its size
//	destroy         - Window is destroy. When this event occurs, the all children have been destroyed
//
//Interaction
//	submit          - (?)
//	value_change    - (?)

// -------------------------------------------------------------------------------------------------

struct EventSubmit : BaseEvent {
private:
	mutable bool submit_rejected_ = false;

public:
	constexpr inline void reject_submit() const noexcept {
		submit_rejected_ = true;
	}

	[[nodiscard]] constexpr inline bool submit_rejected() const noexcept {
		return submit_rejected_;
	}
};

// -------------------------------------------------------------------------------------------------

// NOTE: EventHost system's convenience API has no/low run-time overhead
// Creating a temporary host object with proxy members that point to the component can be optimized away
// Proof: https://godbolt.org/z/vM5zhf3Yf

template <typename ComponentT>
struct EventHostGlobal {
	using component_type = ComponentT;

public:
	ComponentT& owner;

public:
	BasicEventProxyGlobal<ComponentT> global{owner};
};

// -------------------------------------------------------------------------------------------------

template <typename ComponentT>
struct EventHostUI : EventHostGlobal<ComponentT> {
	struct EventBeforeUpdate{};
	BasicEventProxy<ComponentT, EventBeforeUpdate> before_update{this->owner};
	struct EventAfterUpdate{};
	BasicEventProxy<ComponentT, EventAfterUpdate> after_update{this->owner};
};

// -------------------------------------------------------------------------------------------------

template <typename ComponentT>
struct EventHostGeneral : EventHostGlobal<ComponentT> {
	BasicEventProxy<ComponentT, EventChar> char_{this->owner};
	BasicEventProxy<ComponentT, EventKey> key{this->owner};

	BasicEventProxy<ComponentT, EventMouseButton> mouse_button{this->owner};
	BasicEventProxy<ComponentT, EventMouseMovement> mouse_movement{this->owner};
	BasicEventProxy<ComponentT, EventMouseScroll> mouse_scroll{this->owner};

	BasicEventProxy<ComponentT, EventAttach> attach{this->owner};
	BasicEventProxy<ComponentT, EventDetach> detach{this->owner};

	BasicEventProxy<ComponentT, EventEnable> enable{this->owner};
	BasicEventProxy<ComponentT, EventFocus> focus{this->owner};
};

// -------------------------------------------------------------------------------------------------

template <typename ComponentT>
struct EventHostSubmittable : EventHostGeneral<ComponentT> {
	BasicEventProxy<ComponentT, EventSubmit> submit{this->owner};
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
