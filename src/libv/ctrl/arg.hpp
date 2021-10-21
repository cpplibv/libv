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
	[[nodiscard]] constexpr inline bool operator==(const arg_action_t& other) const noexcept = default;
};

struct arg_analog_t {
	scale_type value;

	[[nodiscard]] constexpr inline auto value_d() const noexcept {
		return static_cast<double>(value);
	}

	[[nodiscard]] constexpr inline auto value_f() const noexcept {
		return static_cast<float>(value);
	}

	[[nodiscard]] constexpr inline bool operator==(const arg_analog_t& other) const noexcept = default;
};

struct arg_binary_t {
	bool value;

	[[nodiscard]] constexpr inline bool operator==(const arg_binary_t& other) const noexcept = default;
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
