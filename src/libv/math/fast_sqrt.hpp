// File:   fast_rsqrt.hpp Author: Vader Created on 09 September 2019, 10:40

#pragma once

// libv
#include <libv/utility/bit_cast.hpp>


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

// <<< P1: Cleanup
[[nodiscard]] inline float fast_sqrt(float x) noexcept {
	const float xhalf = 0.5f * x;

	union // get bits for floating value
	{
		float x;
		int i;
	} u;

	u.x = x;
	u.i = 0x5f3759df - (u.i >> 1); // gives initial guess y0

	return x * u.x * (1.5f - xhalf * u.x * u.x); // Newton step, repeating increases accuracy
}

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
