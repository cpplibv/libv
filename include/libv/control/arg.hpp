// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/control/fwd.hpp>
// pro
#include <libv/control/scale.hpp>


namespace libv {
namespace control {

// -------------------------------------------------------------------------------------------------

struct arg_action_t {
	[[nodiscard]] friend constexpr inline bool operator==(const arg_action_t& lhs, const arg_action_t& rhs) noexcept {
		(void) lhs;
		(void) rhs;
		return true;
	}
};

struct arg_analog_t {
	scale_type value;

	[[nodiscard]] friend constexpr inline bool operator==(const arg_analog_t& lhs, const arg_analog_t& rhs) noexcept {
		return lhs.value == rhs.value;
	}
};

struct arg_binary_t {
	bool value;

	[[nodiscard]] friend constexpr inline bool operator==(const arg_binary_t& lhs, const arg_binary_t& rhs) noexcept {
		return lhs.value == rhs.value;
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace control
} //namespace libv
