// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// ext
#include <boost/container/flat_set.hpp>
// libv
#include <libv/input/input.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/observer_ptr.hpp>
// pro
#include <libv/ui/chrono.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BaseComponent;

class ContextState {
	friend class ImplUI;
	friend class UI;

	size_t frame_count_ = 0; /// Number of UI frame operations since the UI initialization
	time_point time_frame_{}; /// Time point of frame start, does not change during the same frame
	time_point time_ui_ = clock::now(); /// Time point of UI initialization
	time_duration time_{}; /// Duration between time_ui and time_frame, does not change during the same frame
	time_duration time_delta_{}; /// Duration between time_frame and previous time_frame, does not change during the same frame

	// time_duration delta_time; /// Duration since last update

	libv::vec2f mouse_position_;
	libv::vec2f scroll_position_;

	bool mouse_over_ = false; /// Mouse is over the frame content area

	boost::container::flat_set<libv::input::Key> pressed_keys;
	boost::container::flat_set<int> pressed_scancodes;
	boost::container::flat_set<libv::input::Mouse> pressed_mouses;

	libv::observer_ptr<BaseComponent> focus_ = nullptr;

	// TODO P4: libv.ui: modifier lock keys: caps-lock, num-lock, scroll-lock
	// TODO P4: libv.ui: pressed timestamps (?)

	// TODO P4: libv.ui: hotkey states (?)

	// TODO P5: libv.ui: joystick vector
	// TODO P5: libv.ui: joystick button states
	// TODO P5: libv.ui: joystick analog states

public:
	[[nodiscard]] constexpr inline size_t frame_count() const noexcept {
		return frame_count_;
	}

public:
	[[nodiscard]] constexpr inline time_point time_frame() const noexcept {
		return time_frame_;
	}

	[[nodiscard]] constexpr inline time_point time_ui() const noexcept {
		return time_ui_;
	}

	[[nodiscard]] constexpr inline time_duration time() const noexcept {
		return time_;
	}

	[[nodiscard]] constexpr inline time_duration time_delta() const noexcept {
		return time_delta_;
	}

	[[nodiscard]] constexpr inline time_duration_f time_delta_f() const noexcept {
		return std::chrono::duration_cast<time_duration_f>(time_delta_);
	}

public:
	[[nodiscard]] constexpr inline libv::vec2f mouse_position() const noexcept {
		return mouse_position_;
	}

	[[nodiscard]] constexpr inline libv::vec2f scroll_position() const noexcept {
		return scroll_position_;
	}

	[[nodiscard]] constexpr inline bool mouse_over() const noexcept {
		return mouse_over_;
	}

public:
	[[nodiscard]] inline libv::input::KeyState key(libv::input::Key key) const noexcept {
		return pressed_keys.contains(key) ? libv::input::KeyState::pressed : libv::input::KeyState::released;
	}

	[[nodiscard]] inline bool key_pressed(libv::input::Key key) const noexcept {
		return pressed_keys.contains(key);
	}

	[[nodiscard]] inline bool key_released(libv::input::Key key) const noexcept {
		return not pressed_keys.contains(key);
	}

	[[nodiscard]] inline libv::input::KeyState mouse(libv::input::Mouse key) const noexcept {
		return pressed_mouses.contains(key) ? libv::input::KeyState::pressed : libv::input::KeyState::released;
	}

	[[nodiscard]] inline bool mouse_pressed(libv::input::Mouse key) const noexcept {
		return pressed_mouses.contains(key);
	}

	[[nodiscard]] inline bool mouse_released(libv::input::Mouse key) const noexcept {
		return not pressed_mouses.contains(key);
	}

public:
	[[nodiscard]] constexpr inline libv::observer_ptr<BaseComponent> focus() const noexcept {
		return focus_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
