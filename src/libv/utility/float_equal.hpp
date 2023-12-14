// Project: libv.utility, File: src/libv/utility/float_equal.hpp

#pragma once

// libv
#include <libv/meta/force_inline.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool float_equal(T a, T b) noexcept {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wfloat-equal"
	return a == b;
	#pragma GCC diagnostic pop
}

template <typename T>
[[nodiscard]] constexpr LIBV_FORCE_INLINE bool float_not_equal(T a, T b) noexcept {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wfloat-equal"
	return a != b;
	#pragma GCC diagnostic pop
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
