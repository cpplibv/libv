// Project: libv.utility, File: src/libv/utility/min_max.hpp

#pragma once

// std
#include <bit>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline T* int_to_ptr(intptr_t i) noexcept {
	return std::bit_cast<T*>(i);
}

//template <typename T>
//[[nodiscard]] constexpr inline T* int_to_ptr(uintptr_t i) noexcept {
//	return std::bit_cast<T*>(i);
//}

template <typename Int, typename T>
[[nodiscard]] constexpr inline Int ptr_to_int(T* ptr) noexcept {
	return static_cast<Int>(std::bit_cast<intptr_t>(ptr));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
