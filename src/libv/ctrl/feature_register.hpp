// Project: libv.ctrl, File: src/libv/ctrl/feature_register.hpp

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// libv
#include <libv/utility/type_key.hpp>
// std
#include <string>
// pro
#include <libv/ctrl/arg.hpp>
#include <libv/ctrl/function_type.hpp>
#include <libv/ctrl/scale.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

class FeatureRegister {
private:
	Controls* target;

public:
	// Implicit constructor from Controls
	constexpr inline FeatureRegister(Controls* target) noexcept : target(target) { }
	// Implicit constructor from Controls
	constexpr inline FeatureRegister(Controls& target) noexcept : target(&target) { }

public:
    constexpr inline FeatureRegister(FeatureRegister&& other) noexcept = default;
    constexpr inline FeatureRegister& operator=(FeatureRegister&& other) & noexcept = default;
    constexpr inline FeatureRegister(const FeatureRegister& other) noexcept = default;
    constexpr inline FeatureRegister& operator=(const FeatureRegister& other) & noexcept = default;
    inline ~FeatureRegister() noexcept = default;

private:
	void _feature_action(libv::type_uid context, std::string&& name, ft_action function);
	void _feature_analog(libv::type_uid context, std::string&& name, ft_analog function, scale_group multipliers);
	void _feature_binary(libv::type_uid context, std::string&& name, ft_binary function);

public:
	template <typename T, typename F>
	inline void feature_action(std::string name, F&& function);

	template <typename T, typename F>
	inline void feature_analog(std::string name, F&& function, scale_group multipliers);

	template <typename T, typename F>
	inline void feature_analog(std::string name, scale_group multipliers, F&& function);

	template <typename T, typename F>
	inline void feature_analog(std::string name, F&& function, scale_type scale_impulse = 1, scale_type scale_time = 1, scale_type scale_analog = 1);

	template <typename T, typename F>
	inline void feature_binary(std::string name, F&& function);
};

// -------------------------------------------------------------------------------------------------

template <auto GlobalFunc>
struct AutomaticFeatureRegister {
	template <typename T>
	inline explicit AutomaticFeatureRegister(const T& func) {
		auto feature_register = FeatureRegister{GlobalFunc()};
		func(feature_register);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T, typename F>
inline void FeatureRegister::feature_action(std::string name, F&& function) {
	// Concept: F is callable
	// NOTE: T could be deduced, but it is better to be explicit
	const auto context = libv::type_key<T>();

	_feature_action(context, std::move(name), [function = std::forward<F>(function)](arg_action params, void* context) mutable {
		if constexpr (std::is_void_v<T>)
			function(params), (void) context;
		else
			function(params, *static_cast<T*>(context));
	});
}

template <typename T, typename F>
inline void FeatureRegister::feature_analog(std::string name, F&& function, scale_group multipliers) {
	// Concept: F is callable
	// NOTE: T could be deduced, but it is better to be explicit
	const auto context = libv::type_key<T>();

	_feature_analog(context, std::move(name), [function = std::forward<F>(function)](arg_analog params, void* context) mutable {
		if constexpr (std::is_void_v<T>)
			function(params), (void) context;
		else
			function(params, *static_cast<T*>(context));
	}, multipliers);
}

template <typename T, typename F>
inline void FeatureRegister::feature_analog(std::string name, scale_group multipliers, F&& function) {
	feature_analog<T>(std::move(name), std::forward<F>(function), multipliers);
}

template <typename T, typename F>
inline void FeatureRegister::feature_analog(std::string name, F&& function, scale_type multi_impulse, scale_type multi_time, scale_type multi_analog) {
	const auto multipliers = scale_group{
			.impulse = multi_impulse,
			.time = multi_time,
			.mouse = multi_analog,
			.scroll = multi_analog,
			.gp_analog = multi_analog,
			.js_analog = multi_analog
	};

	feature_analog<T>(std::move(name), std::forward<F>(function), multipliers);
}

template <typename T, typename F>
inline void FeatureRegister::feature_binary(std::string name, F&& function) {
	// Concept: F is callable
	// NOTE: T could be deduced, but it is better to be explicit
	const auto context = libv::type_key<T>();

	_feature_binary(context, std::move(name), [function = std::forward<F>(function)](arg_binary params, void* context) mutable {
		if constexpr (std::is_void_v<T>)
			function(params), (void) context;
		else
			function(params, *static_cast<T*>(context));
	});
}

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
