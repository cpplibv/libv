// Project: libv.ctrl, File: src/libv/ctrl/controls_impl.lpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
// libv
#include <libv/utility/hash_string.hpp>
#include <libv/utility/timer.hpp>
// std
#include <unordered_map>
#include <vector>
// pro
#include <libv/ctrl/binding.hpp>
#include <libv/ctrl/duration.hpp>
#include <libv/ctrl/feature.hpp>
#include <libv/ctrl/origin.hpp>
#include <libv/ctrl/scale.hpp>
#include <libv/ctrl/scale_2d.hpp>
#include <libv/ctrl/time_point.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

// TODO P5: generalized double buffered template
class DeadkeySet {
	using container_type = boost::container::flat_set<Binding*>;

private:
	container_type foreground_;
	container_type background_;

public:
	void add_to_background(Binding* binding) {
		background_.emplace(binding);
	}

	const container_type& rotate() noexcept {
		foreground_.swap(background_);
		background_.clear();
		return foreground_;
	}

	container_type& foreground() noexcept {
		return foreground_;
	}

	const container_type& foreground() const noexcept {
		return foreground_;
	}

	size_t foreground_size() const noexcept {
		return foreground_.size();
	}

	bool foreground_empty() const noexcept {
		return foreground_.empty();
	}
};

// -------------------------------------------------------------------------------------------------

class ImplControls {
public:	// --- Feature ---
	// TODO P4: Use a better feature container to not duplicate feature names as key (and has a better iteration speed)
	std::unordered_multimap<std::string, Feature, libv::hash_string, std::equal_to<>> features;

public:	// --- Context ---
	std::unordered_map<libv::type_uid, void*> active_contexts;

public:	// --- Binding ---
	std::vector<std::unique_ptr<Binding>> bindings;
	boost::container::flat_set<Binding*> active_bindings;
	DeadkeySet deadkey_set;

public:	// --- Mapping ---
	std::unordered_map<Keycode, Codepoint> codepoint_mappings;

public:	// --- Settings ---
	duration timeout_sequence = std::chrono::milliseconds{5000};

	// Top level
	scale_type impulse_button = 1;
	scale_type_2D scale_analog = {1, 1};
	scale_type scale_time = 1;

	// Input level
	scale_type impulse_mouse_button = 1;
	scale_type_2D scale_mouse_move = {1, 1};

	scale_type_2D scale_scroll = {1, 1};

	scale_type impulse_keyboard = 1;

	scale_type impulse_gamepad_button = 1;
	scale_type_2D scale_gamepad_analog = {1, 1};

	scale_type impulse_joystick_button = 1;
	scale_type_2D scale_joystick_analog = {1, 1};

public:	// --- Misc ---
	libv::Timer last_update_timer;
	bool ignore_events = false;

public:	// --- SOW ---
	time_point input_time = time_point{};
	duration sequence_timeout_timer = duration::zero();

	analog_type_2D mouse_position;
	analog_type_2D scroll_position;

	boost::container::flat_set<Keycode> pressed_keycodes;
	boost::container::flat_set<Scancode> pressed_scancodes;
	boost::container::flat_set<Codepoint> pressed_codepoints;
	boost::container::flat_set<MouseButton> pressed_mouses;
	// TODO P5: Multiple key could be mapped for the same codepoint, it means pressed_codepoints need a counter

	struct GamepadState {
		scale_type impulse_button = 1;
		scale_type_2D scale_analog = {1, 1};

		boost::container::flat_set<GamepadButton> pressed_buttons;
		boost::container::flat_map<GamepadAnalogID, analog_type_2D> analogs;
	};
	boost::container::flat_map<GamepadID, GamepadState> gamepads;

	struct JoystickState {
		scale_type impulse_button = 1;
		scale_type_2D scale_analog = {1, 1};

		boost::container::flat_set<JoystickButton> pressed_buttons;
		boost::container::flat_map<JoystickAnalogID, analog_type_2D> analogs;
	};
	boost::container::flat_map<JoystickID, JoystickState> joysticks;

private: // --- AUX ------------------------------------------------------------------------------------
	template <typename Matcher, typename OffMatcher, typename Op>
	inline auto update_binding(const Matcher& matcher, const OffMatcher& off_matcher, Binding& binding, Op& operation);
	template <typename Matcher, typename OffMatcher>
	inline auto update_and_gather_bindings(const Matcher& matcher, const OffMatcher& off_matcher);
	template <typename T>
	inline auto select_bindings(T& candidate_bindings);
	template <typename C, typename O, typename T>
	inline auto gather_feature(const C& target_binding, const O& operation, T& target_features, Origin origin, scale_type value);
	template <typename C, typename T>
	inline auto gather_features(const C& target_bindings, T& target_features, Origin origin);
	template <typename T>
	inline auto notify_features(const T& target_features);

public: // --- AUX -------------------------------------------------------------------------------------
	void process_analog(const InputID event_X, const InputID event_Y, analog_type_2D value, scale_type_2D scale, Origin origin);
	template <typename EventMatcher>
	void process_button(const EventMatcher& event_matcher, ButtonAction action, scale_type impulse);
	void process_time(const duration delta_time, scale_type scale);
};

// -------------------------------------------------------------------------------------------------

} // namespace ctrl
} // namespace libv
