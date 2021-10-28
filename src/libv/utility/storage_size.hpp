// Project: libv.utility, File: src/libv/utility/storage_size.hpp

#pragma once

// std
#include <cstdint>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Kibibyte
[[nodiscard]] constexpr inline int64_t KB(int64_t size) noexcept {
	return size << 10;
}

/// Mebibyte
[[nodiscard]] constexpr inline int64_t MB(int64_t size) noexcept {
	return size << 20;
}

/// Gibibyte
[[nodiscard]] constexpr inline int64_t GB(int64_t size) noexcept {
	return size << 30;
}

/// Tebibyte
[[nodiscard]] constexpr inline int64_t TB(int64_t size) noexcept {
	return size << 40;
}

/// Pebibyte
[[nodiscard]] constexpr inline int64_t PB(int64_t size) noexcept {
	return size << 50;
}

/// Exbibyte
[[nodiscard]] constexpr inline int64_t EB(int64_t size) noexcept {
	return size << 60;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
