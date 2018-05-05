// File:   storage_size.hpp Author: Vader Created on 2018. július 19., 4:31

#pragma once

//std
#include <cstdint>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Kibibyte
constexpr inline int64_t KB(int64_t size) noexcept {
	return size << 10;
}

/// Mebibyte
constexpr inline int64_t MB(int64_t size) noexcept {
	return size << 20;
}

/// Gibibyte
constexpr inline int64_t GB(int64_t size) noexcept {
	return size << 30;
}

/// Tebibyte
constexpr inline int64_t TB(int64_t size) noexcept {
	return size << 40;
}

/// Pebibyte
constexpr inline int64_t PB(int64_t size) noexcept {
	return size << 50;
}

/// Exbibyte
constexpr inline int64_t EB(int64_t size) noexcept {
	return size << 60;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
