// Project: libv.ctrl, File: src/libv/ctrl/feature.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// std
#include <string>
#include <typeindex>
// pro
#include <libv/ctrl/enum.hpp>
#include <libv/ctrl/function_type.hpp>
#include <libv/ctrl/origin.hpp>
#include <libv/ctrl/scale.hpp>


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

struct Feature {
	std::type_index context;
	std::string name_;

	ft_action function_action;

	ft_binary function_binary;
	bool binary_state = false;

	ft_analog function_analog;
	scale_group feature_multiplier;

public:
	Feature(std::type_index context, std::string&& name, ft_action function);
	Feature(std::type_index context, std::string&& name, ft_analog function, scale_group feature_multiplier);
	Feature(std::type_index context, std::string&& name, ft_binary function);

public:
	void fire_action(void* ctx_ptr);
	void fire_binary(void* ctx_ptr, OperationBinary operation, bool operand_);
	void fire_analog(void* ctx_ptr, scale_type scale, Origin origin);

public:
	[[nodiscard]] inline bool is_action() const noexcept {
		return static_cast<bool>(function_action);
	}

	[[nodiscard]] inline bool is_binary() const noexcept {
		return static_cast<bool>(function_binary);
	}

	[[nodiscard]] inline bool is_analog() const noexcept {
		return static_cast<bool>(function_analog);
	}

	[[nodiscard]] inline const std::string& name() const noexcept {
		return name_;
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
