// Project: libv.ctrl, File: src/libv/ctrl/arg.hpp

#pragma once

// fwd
#include <libv/ctrl/fwd.hpp>
// pro
#include <libv/ctrl/scale.hpp>


namespace libv {
namespace ctrl {

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
	
	[[nodiscard]] constexpr inline auto value_d() const noexcept {
		return static_cast<double>(value);
	}

	[[nodiscard]] constexpr inline auto value_f() const noexcept {
		return static_cast<float>(value);
	}

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

} //namespace ctrl
} //namespace libv
