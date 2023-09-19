//

#pragma once

#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// \return -1, 0, 1
template <typename T>
	requires std::is_unsigned_v<T>
[[nodiscard]] constexpr inline T sign(T x) {
	return T{0} < x;
}

/// \return -1, 0, 1
template <typename T>
	requires std::is_signed_v<T>
[[nodiscard]] constexpr inline T sign(T x) {
	if (x < T{0})
		return T{-1};
	if (x > T{0})
		return T{+1};
	return T{0};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
