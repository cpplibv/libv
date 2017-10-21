// File: endian.hpp - Created on 2017.10.21. 06:47 - Author: Vader

#pragma once

// std
#include <cstdint>


namespace libv {

// -------------------------------------------------------------------------------------------------

inline bool is_big_endian() {
	volatile uint32_t endian_mark{0x01020304}; // Pinned in memory to read endianness at runtime
	return reinterpret_cast<volatile uint8_t&>(endian_mark) == 0x01;
}

inline bool is_little_endian() {
	volatile uint32_t endian_mark{0x01020304}; // Pinned in memory to read endianness at runtime
	return reinterpret_cast<volatile uint8_t&>(endian_mark) == 0x04;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
