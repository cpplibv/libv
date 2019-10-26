// File:   slice.hpp Author: Vader Created on 2018. május 28., 19:03

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline bool float_equal(T a, T b) noexcept {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wfloat-equal"
	return a == b;
	#pragma GCC diagnostic pop
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
