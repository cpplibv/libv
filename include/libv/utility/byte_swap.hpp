// File: byte_swap.hpp - Created on 2018.05.05. 02:54 - Author: Vader

#pragma once

// libv
#include <libv/utility/endian.hpp>
#include <libv/utility/bit_cast.hpp>
// std
#include <cstdint>


namespace libv {

// -------------------------------------------------------------------------------------------------

constexpr inline uint16_t byte_swap(uint16_t var) {
	return
			((var << 1 * 8) & 0xFF00) |
			((var >> 1 * 8) & 0x00FF);
}

constexpr inline uint32_t byte_swap(uint32_t var) {
	return
			((var << 3 * 8) & 0xFF000000) |
			((var << 1 * 8) & 0x00FF0000) |
			((var >> 1 * 8) & 0x0000FF00) |
			((var >> 3 * 8) & 0x000000FF);
}

constexpr inline uint64_t byte_swap(uint64_t var) {
	return
			((var << 7 * 8) & 0xFF00000000000000) |
			((var << 5 * 8) & 0x00FF000000000000) |
			((var << 3 * 8) & 0x0000FF0000000000) |
			((var << 1 * 8) & 0x000000FF00000000) |
			((var >> 1 * 8) & 0x00000000FF000000) |
			((var >> 3 * 8) & 0x0000000000FF0000) |
			((var >> 5 * 8) & 0x000000000000FF00) |
			((var >> 7 * 8) & 0x00000000000000FF);
}

template <typename T>
constexpr inline T byte_swap(T var) {
    if constexpr (sizeof (T) == 1)
        return var;
    else if constexpr (sizeof (T) == 2)
        return bit_cast<T>(byte_swap(bit_cast<uint16_t>(var)));
    else if constexpr (sizeof (T) == 4)
        return bit_cast<T>(byte_swap(bit_cast<uint32_t>(var)));
    else if constexpr (sizeof (T) == 8)
        return bit_cast<T>(byte_swap(bit_cast<uint64_t>(var)));
    else
        static_assert(sizeof (T) == sizeof (T), "byte_swap is only supported for 1, 2, 4, 8 byte types");
}

template <typename T>
constexpr inline T network_to_host(T var) {
	if constexpr (is_network_endian())
		return var;
	else
		return byte_swap(var);
}

template <typename T>
constexpr inline T host_to_network(T var) {
	if constexpr (is_network_endian())
		return var;
	else
		return byte_swap(var);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
