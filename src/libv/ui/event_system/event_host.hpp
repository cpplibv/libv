// Project: libv.ui, File: src/libv/ui/event_system/event_host.hpp

#pragma once

// pro
#include <libv/gl/fwd.hpp>
#include <libv/ui/chrono.hpp>
#include <libv/ui/event/base_event.hpp>
#include <libv/ui/event/event_component.hpp>
#include <libv/ui/event/event_enable.hpp>
#include <libv/ui/event/event_focus.hpp>
#include <libv/ui/event/event_keyboard.hpp>
#include <libv/ui/event/event_mouse.hpp>
#include <libv/ui/event_system/basic_event_proxy.hpp>


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

struct EventBeforeUpdate : BaseEvent  {
	time_point frame_time;
	time_duration delta_time;

	constexpr inline EventBeforeUpdate(time_point frameTime, time_duration deltaTime) noexcept :
			frame_time(frameTime), delta_time(deltaTime) {}
};

struct EventAfterUpdate : BaseEvent  {
	time_point frame_time;
	time_duration delta_time;

	constexpr inline EventAfterUpdate(time_point frameTime, time_duration deltaTime) noexcept :
			frame_time(frameTime), delta_time(deltaTime) {}
};

// struct EventBeforeCreate : BaseEvent {
// 	libv::GL& gl;
// 	explicit inline EventBeforeCreate(gl::GL& gl) : gl(gl) {}
// };
struct EventBeforeRender : BaseEvent {
	libv::GL& gl;
	explicit inline EventBeforeRender(gl::GL& gl) : gl(gl) {}
};
// struct EventBeforeDestroy : BaseEvent {
// 	libv::GL& gl;
// 	explicit inline EventBeforeDestroy(gl::GL& gl) : gl(gl) {}
// };
// struct EventAfterDestroy : BaseEvent {
// 	libv::GL& gl;
// 	explicit inline EventAfterDestroy(gl::GL& gl) : gl(gl) {}
// };

// -------------------------------------------------------------------------------------------------

// NOTE: EventHost system's convenience API has low/no run-time overhead
// Creating a temporary host object with proxy members that point to the component can be optimized away
// Proof: https://godbolt.org/z/vM5zhf3Yf

template <typename ComponentT>
struct EventHostGlobal {
	using component_type = ComponentT;

public:
	ComponentT& owner;

public:
	BasicEventProxyGlobalCustom<ComponentT> global{owner};

	BasicEventProxyGlobal<ComponentT, EventBeforeUpdate> global_before_update{owner};
	BasicEventProxyGlobal<ComponentT, EventAfterUpdate> global_after_update{owner};
	// BasicEventProxyGlobal<ComponentT, EventBeforeCreate> global_before_create{owner};
	BasicEventProxyGlobal<ComponentT, EventBeforeRender> global_before_render{owner};
	// BasicEventProxyGlobal<ComponentT, EventBeforeDestroy> global_before_destroy{owner};
	// BasicEventProxyGlobal<ComponentT, EventAfterDestroy> global_after_destroy{owner};
};

// -------------------------------------------------------------------------------------------------

struct EventSelectionGroupChange : BaseEvent {};

template <typename ComponentT>
struct EventHostSelectionGroup : EventHostGlobal<ComponentT> {
	BasicEventProxy<ComponentT, EventSelectionGroupChange> change{this->owner};
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

struct BaseEventCanvasGL : BaseEvent {
	libv::GL& gl;
	explicit inline BaseEventCanvasGL(gl::GL& gl) : gl(gl) {}
};
struct EventCanvasBeforeCreate : BaseEventCanvasGL { using BaseEventCanvasGL::BaseEventCanvasGL; };
struct EventCanvasAfterCreate : BaseEventCanvasGL { using BaseEventCanvasGL::BaseEventCanvasGL; };
struct EventCanvasBeforeDestroy : BaseEventCanvasGL { using BaseEventCanvasGL::BaseEventCanvasGL; };
struct EventCanvasAfterDestroy : BaseEventCanvasGL { using BaseEventCanvasGL::BaseEventCanvasGL; };
struct EventCanvasBeforeRender : BaseEventCanvasGL { using BaseEventCanvasGL::BaseEventCanvasGL; };
struct EventCanvasAfterRender : BaseEventCanvasGL { using BaseEventCanvasGL::BaseEventCanvasGL; };

template <typename ComponentT>
struct EventHostCanvas : EventHostGeneral<ComponentT> {
	BasicEventProxy<ComponentT, EventCanvasBeforeCreate> before_create{this->owner};
	BasicEventProxy<ComponentT, EventCanvasAfterCreate> after_create{this->owner};
	BasicEventProxy<ComponentT, EventCanvasBeforeDestroy> before_destroy{this->owner};
	BasicEventProxy<ComponentT, EventCanvasAfterDestroy> after_destroy{this->owner};
	BasicEventProxy<ComponentT, EventCanvasBeforeRender> before_render{this->owner};
	BasicEventProxy<ComponentT, EventCanvasAfterRender> after_render{this->owner};
};

// -------------------------------------------------------------------------------------------------

template <typename ComponentT>
struct EventHostSubmittable : EventHostGeneral<ComponentT> {
	BasicEventProxy<ComponentT, EventSubmit> submit{this->owner};
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
