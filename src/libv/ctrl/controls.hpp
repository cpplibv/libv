// Project: libv.ctrl, File: src/libv/ctrl/controls.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// libv
#include <libv/input/event_fwd.hpp>
#include <libv/utility/function_ref.hpp>
// std
#include <memory>
#include <string>
#include <string_view>
#include <typeindex>
// pro
#include <libv/ctrl/binding_level.hpp>
#include <libv/ctrl/duration.hpp>
#include <libv/ctrl/enum.hpp>
#include <libv/ctrl/function_type.hpp>
#include <libv/ctrl/scale.hpp>
#include <libv/ctrl/scale_2d.hpp>
#include <libv/ctrl/search_mode.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

enum class binding_outcome {
	success = 0,

	failure_invalid_sequence,
	failure_invalid_sequence_str,

	//	(?) failure_incompatible_sequence_and_feature,
	//	warning_sequence_collusion,
	//	warning_sequence_collusion_prefix,
	//	warning_sequence_collusion_multi_stroke,
};

enum class unbinding_outcome {
	success = 0,

	failure_invalid_sequence_str,
	//	ambigous_sequence,
	//
	//	not_found_binding,
	//	not_found_feature,
	//	not_found_sequence,
};

// -------------------------------------------------------------------------------------------------

class Controls {
	friend class FeatureRegister;

private:
	std::unique_ptr<ImplControls> self;

public:
    Controls();
    Controls(Controls&& other) = default;
    Controls& operator=(Controls&& other) & = default;
    Controls(const Controls& other) = delete;
    Controls& operator=(const Controls& other) & = delete;
    ~Controls();

private:
	void _context_enter(std::type_index type, void* ctx);
	void _context_leave(std::type_index type);

	void _feature_action(std::type_index context, std::string&& name, ft_action function);
	void _feature_analog(std::type_index context, std::string&& name, ft_analog function, scale_type scale_impulse, scale_type scale_time, scale_type scale_analog);
	void _feature_binary(std::type_index context, std::string&& name, ft_binary function);

	void _remove_feature(std::type_index context, std::string_view name);

public:
	template <typename T>
	inline void context_enter(T* context);

	template <typename T>
	inline void context_leave();

public:
	template <typename T, typename F>
	inline void feature_action(std::string name, F&& function);

	template <typename T, typename F>
	inline void feature_analog(std::string name, F&& function, scale_type scale_impulse = 1, scale_type scale_time = 1, scale_type scale_analog = 1);

	template <typename T, typename F>
	inline void feature_analog(std::string name, F&& function, scale_group scales);

	template <typename T, typename F>
	inline void feature_analog(std::string name, scale_group scales, F&& function);

	template <typename T, typename F>
	inline void feature_binary(std::string name, F&& function);

	template <typename T>
	inline void remove_feature(std::string_view name);

public:
	binding_outcome bind(std::string feature_name, Sequence seq, binding_level level = auto_binding_level);
	binding_outcome bind(std::string feature_name, Sequence seq, scale_type scale, binding_level level = auto_binding_level);
	binding_outcome bind(std::string feature_name, Combination comb, binding_level level = auto_binding_level);
	binding_outcome bind(std::string feature_name, Combination comb, scale_type scale, binding_level level = auto_binding_level);
	binding_outcome bind(std::string feature_name, Input input, binding_level level = auto_binding_level);
	binding_outcome bind(std::string feature_name, Input input, scale_type scale, binding_level level = auto_binding_level);
	binding_outcome bind(std::string feature_name, std::string_view seq_str, binding_level level = auto_binding_level);
	binding_outcome bind(std::string feature_name, std::string_view seq_str, scale_type scale, binding_level level = auto_binding_level);

	unbinding_outcome unbind(std::string_view feature_name, Sequence seq);
	unbinding_outcome unbind(std::string_view feature_name, std::string_view seq_str);
	unbinding_outcome unbind_all(std::string_view feature_name);

public:
	void input(const libv::input::EventKey& event);
	void input(const libv::input::EventMouseButton& event);
	void input(const libv::input::EventMousePosition& event);
	void input(const libv::input::EventMouseScroll& event);
	void input(const libv::input::EventGamepadAnalog& event);
	void input(const libv::input::EventGamepadButton& event);
	void input(const libv::input::EventJoystickButton& event);
	void input(const libv::input::EventJoystickAnalog& event);

public:
	void update(duration delta_time);

public:
	/// Sets the sequence timeout period, after a valid sequence step will abort and resets to its start state
	void timeout_sequence(duration timeout) noexcept;

	/// Sets the top level impulse, effects every button based impulse levels
	void impulse_button(scale_type impulse) noexcept;
	/// Sets the top level scale, effects every analog based scale levels
	void scale_analog(scale_type scale_xy) noexcept;
	/// Sets the top level scale, effects every analog based scale levels
	void scale_analog(scale_type scale_x, scale_type scale_y) noexcept;
	/// Sets the top level scale, effects every analog based scale levels
	void scale_analog(scale_type_2D scale) noexcept;
	/// Sets the top level scale, effects every time based scale levels
	void scale_time(scale_type scale) noexcept;

	void impulse_gamepad_button(GamepadID gamepadID, scale_type impulse) noexcept;
	void impulse_gamepad_button(scale_type impulse) noexcept;
	void impulse_joystick_button(JoystickID joystickID, scale_type impulse) noexcept;
	void impulse_joystick_button(scale_type impulse) noexcept;
	void impulse_keyboard(scale_type impulse) noexcept;
	void impulse_mouse_button(scale_type impulse) noexcept;

	void scale_gamepad_analog(GamepadID gamepadID, scale_type scale_xy) noexcept;
	void scale_gamepad_analog(GamepadID gamepadID, scale_type scale_x, scale_type scale_y) noexcept;
	void scale_gamepad_analog(GamepadID gamepadID, scale_type_2D scale) noexcept;
	void scale_gamepad_analog(scale_type scale_xy) noexcept;
	void scale_gamepad_analog(scale_type scale_x, scale_type scale_y) noexcept;
	void scale_gamepad_analog(scale_type_2D scale) noexcept;
	void scale_joystick_analog(JoystickID joystickID, scale_type scale_xy) noexcept;
	void scale_joystick_analog(JoystickID joystickID, scale_type scale_x, scale_type scale_y) noexcept;
	void scale_joystick_analog(JoystickID joystickID, scale_type_2D scale) noexcept;
	void scale_joystick_analog(scale_type scale_xy) noexcept;
	void scale_joystick_analog(scale_type scale_x, scale_type scale_y) noexcept;
	void scale_joystick_analog(scale_type_2D scale) noexcept;
	void scale_mouse_move(scale_type scale_xy) noexcept;
	void scale_mouse_move(scale_type scale_x, scale_type scale_y) noexcept;
	void scale_mouse_move(scale_type_2D scale) noexcept;
	void scale_scroll(scale_type scale_xy) noexcept;
	void scale_scroll(scale_type scale_x, scale_type scale_y) noexcept;
	void scale_scroll(scale_type_2D scale) noexcept;

public:
	/// Returns the sequence timeout period, after a valid sequence step will abort and resets to its start state
	[[nodiscard]] duration timeout_sequence() const noexcept;

	/// Returns the top level impulse that effects every button based impulse levels
	[[nodiscard]] scale_type impulse_button() const noexcept;
	/// Returns the top level scale that effects every analog based scale levels
	[[nodiscard]] scale_type_2D scale_analog() const noexcept;
	/// Returns the top level scale that effects every time based scale levels
	[[nodiscard]] scale_type scale_time() const noexcept;

	[[nodiscard]] scale_type impulse_keyboard() const noexcept;
	[[nodiscard]] scale_type impulse_mouse_button() const noexcept;
	[[nodiscard]] scale_type impulse_gamepad_button() const noexcept;
	[[nodiscard]] scale_type impulse_gamepad_button(GamepadID gamepadID) const noexcept;
	[[nodiscard]] scale_type impulse_joystick_button() const noexcept;
	[[nodiscard]] scale_type impulse_joystick_button(JoystickID joystickID) const noexcept;

	[[nodiscard]] scale_type_2D scale_gamepad_analog(GamepadID gamepadID) const noexcept;
	[[nodiscard]] scale_type_2D scale_gamepad_analog() const noexcept;
	[[nodiscard]] scale_type_2D scale_joystick_analog(JoystickID joystickID) const noexcept;
	[[nodiscard]] scale_type_2D scale_joystick_analog() const noexcept;
	[[nodiscard]] scale_type_2D scale_mouse_move() const noexcept;
	[[nodiscard]] scale_type_2D scale_scroll() const noexcept;

public:
	void register_codepoint(Keycode keycode, Codepoint codepoint);
	void unregister_codepoint(Keycode keycode);
	[[nodiscard]] Codepoint codepoint(Keycode keycode);

public:
	void clear_bindings();
//	void clear_contexts();
//	void clear_features();
//	void clear_sow();
	void clear_codepoints();

public:
	void foreach_codepoints(libv::function_ref<void(Keycode, Codepoint)> function);

	[[nodiscard]] bool is_feature(std::string_view name) const noexcept;
	void foreach_features(libv::function_ref<void(const Feature&)> function);
	void foreach_features_in_context(libv::function_ref<void(const Feature&)> function);
	void foreach_features_search(libv::function_ref<void(const Feature&)> function, std::string_view name, search_mode mode = search_mode::contains);

	void foreach_bindings(libv::function_ref<void(const Binding&)> function);
	void foreach_bindings_search(libv::function_ref<void(const Binding&)> function, const Sequence& seq, search_mode mode = search_mode::contains);
	void foreach_bindings_search(libv::function_ref<void(const Binding&)> function, std::string_view name, search_mode mode = search_mode::contains);

public:
	[[nodiscard]] std::string export_bindings();
	[[nodiscard]] std::string export_settings();
	void import_bindings(std::string_view data);
	void import_settings(std::string_view data);
};

// -------------------------------------------------------------------------------------------------

template <typename T>
inline void Controls::context_enter(T* context) {
	_context_enter(std::type_index(typeid(T)), static_cast<void*>(context));
}

template <typename T>
inline void Controls::context_leave() {
	_context_leave(std::type_index(typeid(T)));
}

// -------------------------------------------------------------------------------------------------

template <typename T, typename F>
inline void Controls::feature_action(std::string name, F&& function) {
	// Concept: F is callable
	// NOTE: T could be deduced, but it is better to be explicit
	const auto context = std::type_index(typeid(T));

	_feature_action(context, std::move(name), [function = std::forward<F>(function)](arg_action params, void* context) mutable {
		if constexpr (std::is_void_v<T>)
			function(params), (void) context;
		else
			function(params, *static_cast<T*>(context));
	});
}

template <typename T, typename F>
inline void Controls::feature_analog(std::string name, F&& function, scale_type scale_impulse, scale_type scale_time, scale_type scale_analog) {
	// Concept: F is callable
	// NOTE: T could be deduced, but it is better to be explicit
	const auto context = std::type_index(typeid(T));

	_feature_analog(context, std::move(name), [function = std::forward<F>(function)](arg_analog params, void* context) mutable {
		if constexpr (std::is_void_v<T>)
			function(params), (void) context;
		else
			function(params, *static_cast<T*>(context));
	}, scale_impulse, scale_time, scale_analog);
}

template <typename T, typename F>
inline void Controls::feature_analog(std::string name, F&& function, scale_group scales) {
	feature_analog<T>(std::move(name), std::forward<F>(function), scales.impulse, scales.time, scales.scale);
}

template <typename T, typename F>
inline void Controls::feature_analog(std::string name, scale_group scales, F&& function) {
	feature_analog<T>(std::move(name), std::forward<F>(function), scales.impulse, scales.time, scales.scale);
}

template <typename T, typename F>
inline void Controls::feature_binary(std::string name, F&& function) {
	// Concept: F is callable
	// NOTE: T could be deduced, but it is better to be explicit
	const auto context = std::type_index(typeid(T));

	_feature_binary(context, std::move(name), [function = std::forward<F>(function)](arg_binary params, void* context) mutable {
		if constexpr (std::is_void_v<T>)
			function(params), (void) context;
		else
			function(params, *static_cast<T*>(context));
	});
}

template <typename T>
inline void Controls::remove_feature(std::string_view name) {
	const auto context = std::type_index(typeid(T));

	_remove_feature(context, name);
}

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
