// Project: libv.math, File: src/libv/math/mix.hpp

#pragma once

#include <libv/math/sign.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline T mix(T v0, T v1, T t) noexcept {
	return (T{1} - t) * v0 + t * v1;
}

template <typename T>
[[nodiscard]] constexpr inline T mix_toward(T source, T target, T maxChange) {
	if (maxChange == T{0})
		return target;

	const auto diff = target - source;
	if (std::abs(diff) < maxChange)
		return target;
	else
		return source + maxChange * libv::sign(diff);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
