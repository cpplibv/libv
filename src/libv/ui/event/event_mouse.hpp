// Project: libv.ui, File: src/libv/ui/event/event_mouse.hpp

#pragma once

// libv
#include <libv/input/input.hpp>
#include <libv/math/vec.hpp>
// pro
#include <libv/ui/event/base_event.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct BaseEventMouse : BaseEvent {
//private:
//	mutable bool shield_mouse_ = false;
//
//public:
//	constexpr inline void shield_mouse() const noexcept {
//		shield_mouse_ = true;
//	}
//
//	[[nodiscard]] constexpr inline bool mouse_shielded() const noexcept {
//		return !shield_mouse_;
//	}
};

struct EventMouseButton : BaseEventMouse {
	libv::vec2f mouse_position;
	libv::vec2f local_position;

	libv::input::MouseButton button;
	libv::input::Action action;
};

struct EventMouseMovement : BaseEventMouse {
	friend struct AccessEventMouseMovement;

	libv::vec2f mouse_position;
	libv::vec2f local_position;

	libv::vec2f mouse_movement;
	bool enter = false;
	bool leave = false;

private:
	mutable bool pass_through_ = false;

public:
	constexpr inline void pass_through() const noexcept {
		pass_through_ = true;
	}

	[[nodiscard]] constexpr inline bool is_pass_through() const noexcept {
		return pass_through_;
	}
};

struct EventMouseScroll : BaseEventMouse {
	libv::vec2f mouse_position;
	libv::vec2f local_position;

	libv::vec2f scroll_position;
	libv::vec2f scroll_movement;
	//TYPE scroll_unit;
};

// -------------------------------------------------------------------------------------------------

struct AccessEventMouseMovement {
	constexpr static inline void reset_pass_through(EventMouseMovement& event) noexcept {
		event.pass_through_ = false;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
