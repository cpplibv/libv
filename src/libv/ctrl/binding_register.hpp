// Project: libv.ctrl, File: src/libv/ctrl/binding_register.hpp

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/ctrl/binding_level.hpp>
#include <libv/ctrl/binding_outcome.hpp>
#include <libv/ctrl/scale.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

class BindingRegister {
private:
	Controls* target;

public:
	// Implicit constructor from Controls
	explicit(false) constexpr inline BindingRegister(Controls* target) noexcept : target(target) { }
	// Implicit constructor from Controls
	explicit(false) constexpr inline BindingRegister(Controls& target) noexcept : target(&target) { }

public:
    constexpr inline BindingRegister(BindingRegister&& other) noexcept = default;
    constexpr inline BindingRegister& operator=(BindingRegister&& other) & noexcept = default;
    constexpr inline BindingRegister(const BindingRegister& other) noexcept = default;
    constexpr inline BindingRegister& operator=(const BindingRegister& other) & noexcept = default;
    inline ~BindingRegister() noexcept = default;

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
};

// -------------------------------------------------------------------------------------------------

template <auto GlobalFunc>
struct AutomaticBindingRegister {
	template <typename T>
	inline explicit AutomaticBindingRegister(const T& func) {
		auto feature_register = BindingRegister{GlobalFunc()};
		func(feature_register);
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
