// Project: libv.utility, File: src/libv/utility/align.hpp

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline T align(const T value, const T alignment) noexcept {
	// assert(alignment != 0);
	return (alignment - 1 + value) / alignment * alignment;
}

template <typename T>
[[nodiscard]] constexpr inline T* align(T* ptr, std::uintptr_t alignment) noexcept {
	// assert(alignment != 0);
    return reinterpret_cast<T*>(align(reinterpret_cast<std::uintptr_t>(ptr), alignment));
}

template <typename T>
[[nodiscard]] constexpr inline T align_of_2(const T value, const T alignment) noexcept {
	return (alignment - 1 + value) & ~(alignment - 1);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
