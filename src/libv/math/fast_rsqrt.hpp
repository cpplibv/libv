// Project: libv.math, File: src/libv/math/fast_rsqrt.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/bit_cast.hpp>


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

/// Calculates the fast inverse square root approximation.
/// Error rate is around 0.175% for inputs above 1.0 and around flat 0.01 for inputs below 1.0
/// For more information see: https://en.wikipedia.org/wiki/Fast_inverse_square_root
[[nodiscard]] constexpr inline float fast_rsqrt(float value) noexcept {
	int32_t i = libv::bit_cast<int32_t>(value);
	i = 0x5f3759df - (i >> 1); // evil floating point bit level hacking

	float y = libv::bit_cast<float>(i);
	y = y * (1.5f - (value * 0.5f * y * y)); // One iteration of Newton's method, repeated application would reduce error rate

	return y;
}

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
