// Project: libv.utility, File: src/libv/utility/float_equal.hpp, Author: Császár Mátyás [Vader]

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
