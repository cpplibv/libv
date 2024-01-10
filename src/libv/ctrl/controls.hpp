// Project: libv.ctrl, File: src/libv/ctrl/controls.hpp

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// libv
#include <libv/input/event_fwd.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
#include <libv/utility/type_key.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/ctrl/binding_level.hpp>
#include <libv/ctrl/binding_outcome.hpp>
#include <libv/ctrl/duration.hpp>
#include <libv/ctrl/enum.hpp>
#include <libv/ctrl/function_type.hpp>
#include <libv/ctrl/scale.hpp>
#include <libv/ctrl/scale_2d.hpp>
#include <libv/ctrl/search_mode.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

class Controls {
	friend FeatureRegister;

private:
	libv::intrusive2_ptr<ImplControls> self;

public:
    Controls();
    Controls(Controls&& other) = default;
    Controls& operator=(Controls&& other) & = default;
    Controls(const Controls& other) = default;
    Controls& operator=(const Controls& other) & = default;
    ~Controls();

private:
	void _context_enter(libv::type_uid type, void* ctx);
	void _context_leave(libv::type_uid type);
	void _context_leave_if_matches(libv::type_uid type, void* ctx);

	void _feature_action(libv::type_uid context, std::string&& name, ft_action function);
	void _feature_analog(libv::type_uid context, std::string&& name, ft_analog function, scale_group multipliers);
	void _feature_binary(libv::type_uid context, std::string&& name, ft_binary function);

	void _remove_feature(libv::type_uid context, std::string_view name);

	void aux_update(duration delta_time);

public:
	template <typename Context>
	inline void context_enter(Context* context);

	template <typename Context>
	inline void context_leave();

	template <typename Context>
	inline void context_leave_if_matches(Context* context);

public:
	template <typename Context, typename F>
	inline void feature_action(std::string name, F&& function);

	template <typename Context, typename F>
	inline void feature_analog(std::string name, F&& function, scale_group multipliers);

	template <typename Context, typename F>
	inline void feature_analog(std::string name, scale_group multipliers, F&& function);

	template <typename Context, typename F>
	inline void feature_analog(std::string name, F&& function, scale_type multi_impulse = 1, scale_type multi_time = 1, scale_type multi_analog = 1);

	template <typename Context, typename F>
	inline void feature_binary(std::string name, F&& function);

	template <typename Context>
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
	template <typename Frame>
	void attach(Frame& frame);

public:
	void ignore_events(bool value);

public:
	void event(const libv::input::EventKey& event, bool allowProcess = true);
	void event(const libv::input::EventMouseButton& event, bool allowProcess = true);
	void event(const libv::input::EventMousePosition& event, bool allowProcess = true);
	void event(const libv::input::EventMouseScroll& event, bool allowProcess = true);
	void event(const libv::input::EventGamepadAnalog& event, bool allowProcess = true);
	void event(const libv::input::EventGamepadButton& event, bool allowProcess = true);
	void event(const libv::input::EventJoystickButton& event, bool allowProcess = true);
	void event(const libv::input::EventJoystickAnalog& event, bool allowProcess = true);

	void update(duration delta_time);
	void update_since_last_update();

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
	void foreach_bindings_search(libv::function_ref<void(const Binding&)> function, std::string_view feature_name, search_mode mode = search_mode::contains);

public:
	[[nodiscard]] std::string export_bindings();
	[[nodiscard]] std::string export_settings();
	void import_bindings(std::string_view data);
	void import_settings(std::string_view data);
};

// -------------------------------------------------------------------------------------------------

template <typename Frame>
void Controls::attach(Frame& frame) {
	frame.onKey.output([this](const auto& e) {
		this->event(e);
	});

	frame.onMouseButton.output([this](const auto& e) {
		this->event(e);
	});
	frame.onMousePosition.output([this](const auto& e) {
		this->event(e);
	});
	frame.onMouseScroll.output([this](const auto& e) {
		this->event(e);
	});

	frame.onGamepadAnalog.output([this](const auto& e) {
		this->event(e);
	});
	frame.onGamepadButton.output([this](const auto& e) {
		this->event(e);
	});
	frame.onJoystickAnalog.output([this](const auto& e) {
		this->event(e);
	});
	frame.onJoystickButton.output([this](const auto& e) {
		this->event(e);
	});

	frame.onContextUpdate.output([this](const auto&) {
		this->update_since_last_update();
	});

	// TODO P5: libv.ctrl: Joystick/Gamepad support: Might need to observe Connect-Disconnect events
	//          | No need: Frame should release everything upon disconnect
}

// -------------------------------------------------------------------------------------------------

template <typename Context>
inline void Controls::context_enter(Context* context) {
	_context_enter(libv::type_key<Context>(), static_cast<void*>(context));
}

template <typename Context>
inline void Controls::context_leave() {
	_context_leave(libv::type_key<Context>());
}

template <typename Context>
inline void Controls::context_leave_if_matches(Context* context) {
	_context_leave_if_matches(libv::type_key<Context>(), static_cast<void*>(context));
}

// -------------------------------------------------------------------------------------------------

template <typename Context, typename F>
inline void Controls::feature_action(std::string name, F&& function) {
	// Concept: F is callable
	// NOTE: T could be deduced, but it is better to be explicit
	const auto context = libv::type_key<Context>();

	_feature_action(context, std::move(name), [function = std::forward<F>(function)](arg_action params, void* context) mutable {
		if constexpr (std::is_void_v<Context>)
			function(params), (void) context;
		else
			function(params, *static_cast<Context*>(context));
	});
}

template <typename Context, typename F>
inline void Controls::feature_analog(std::string name, F&& function, scale_group multipliers) {
	// Concept: F is callable
	// NOTE: T could be deduced, but it is better to be explicit
	const auto context = libv::type_key<Context>();

	_feature_analog(context, std::move(name), [function = std::forward<F>(function)](arg_analog params, void* context) mutable {
		if constexpr (std::is_void_v<Context>)
			function(params), (void) context;
		else
			function(params, *static_cast<Context*>(context));
	}, multipliers);
}

template <typename Context, typename F>
inline void Controls::feature_analog(std::string name, scale_group multipliers, F&& function) {
	feature_analog<Context>(std::move(name), std::forward<F>(function), multipliers);
}

template <typename Context, typename F>
inline void Controls::feature_analog(std::string name, F&& function, scale_type multi_impulse, scale_type multi_time, scale_type multi_analog) {
	const auto multipliers = scale_group{
			.impulse = multi_impulse,
			.time = multi_time,
			.mouse = multi_analog,
			.scroll = multi_analog,
			.gp_analog = multi_analog,
			.js_analog = multi_analog
	};

	feature_analog<Context>(std::move(name), std::forward<F>(function), multipliers);
}

template <typename Context, typename F>
inline void Controls::feature_binary(std::string name, F&& function) {
	// Concept: F is callable
	// NOTE: T could be deduced, but it is better to be explicit
	const auto context = libv::type_key<Context>();

	_feature_binary(context, std::move(name), [function = std::forward<F>(function)](arg_binary params, void* context) mutable {
		if constexpr (std::is_void_v<Context>)
			function(params), (void) context;
		else
			function(params, *static_cast<Context*>(context));
	});
}

template <typename Context>
inline void Controls::remove_feature(std::string_view name) {
	const auto context = libv::type_key<Context>();

	_remove_feature(context, name);
}

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
