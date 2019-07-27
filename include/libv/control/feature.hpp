// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/control/fwd.hpp>
// std
#include <typeindex>
// pro
#include <libv/control/enum.hpp>
#include <libv/control/function_type.hpp>
#include <libv/control/scale.hpp>


namespace libv {
namespace control {

// -------------------------------------------------------------------------------------------------

enum class Origin {
	analog,
	impulse,
	time,
};

struct Feature {
	std::type_index context;
	std::string name_;

	ft_action function_action;

	ft_binary function_binary;
	bool binary_state = false;

	ft_analog function_analog;
	scale_type scale_impulse;
	scale_type scale_time;
	scale_type scale_analog;

public:
	Feature(std::type_index context, std::string&& name, ft_action function);
	Feature(std::type_index context, std::string&& name, ft_analog function, scale_type scale_impulse, scale_type scale_time, scale_type scale_analog);
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

} //namespace control
} //namespace libv
