// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// std
#include <string>
#include <typeindex>
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
	void _feature_action(std::type_index context, std::string&& name, ft_action function);
	void _feature_binary(std::type_index context, std::string&& name, ft_binary function);
	void _feature_analog(std::type_index context, std::string&& name, ft_analog function, scale_type scale_impulse, scale_type scale_time, scale_type scale_analog);

public:
	template <typename T, typename F>
	inline void feature_action(std::string name, F&& function);

	template <typename T, typename F>
	inline void feature_binary(std::string name, F&& function);

	template <typename T, typename F>
	inline void feature_analog(std::string name, F&& function, scale_type scale_impulse = 1, scale_type scale_time = 1, scale_type scale_analog = 1);
};

// -------------------------------------------------------------------------------------------------

template <auto GlobalFunc>
struct AutomaticFeatureRegister {
	template <typename T>
	AutomaticFeatureRegister(const T& func) {
		auto feature_register = FeatureRegister{GlobalFunc()};
		func(feature_register);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T, typename F>
inline void FeatureRegister::feature_action(std::string name, F&& function) {
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
inline void FeatureRegister::feature_analog(std::string name, F&& function, scale_type scale_impulse, scale_type scale_time, scale_type scale_analog) {
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
inline void FeatureRegister::feature_binary(std::string name, F&& function) {
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

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
