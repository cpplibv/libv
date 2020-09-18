// Project: libv.ui, File: src/libv/ui/event_host.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/basic_event_proxy.hpp>
#include <libv/ui/event/base_event.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

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
};

// -------------------------------------------------------------------------------------------------

template <typename ComponentT>
struct EventHostGeneral {
	using component_type = ComponentT;

public:
	BasicEventProxy<ComponentT, EventChar> char_;
	BasicEventProxy<ComponentT, EventKey> key;

	BasicEventProxy<ComponentT, EventFocus> focus;

	BasicEventProxy<ComponentT, EventMouseButton> mouse_button;
	BasicEventProxy<ComponentT, EventMouseMovement> mouse_movement;
	BasicEventProxy<ComponentT, EventMouseScroll> mouse_scroll;

public:
	explicit inline EventHostGeneral(ComponentT& core) noexcept :
		char_(core),
		key(core),
		focus(core),
		mouse_button(core),
		mouse_movement(core),
		mouse_scroll(core) {}
};

// -------------------------------------------------------------------------------------------------

template <typename ComponentT>
struct EventHostSubmitable : EventHostGeneral<ComponentT> {
	using component_type = ComponentT;

public:
	BasicEventProxy<ComponentT, EventSubmit> submit;

public:
	explicit inline EventHostSubmitable(ComponentT& core) : EventHostGeneral<ComponentT>(core),
		submit(core) {}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
