// File: fixed.hpp Author: Vader Created on 2017. január 26., 0:20

#pragma once

// std
#include <cmath>


namespace libv {

// -------------------------------------------------------------------------------------------------

inline uint32_t convert_to_s_24_8(double value) {
	auto tmp = static_cast<uint32_t> (std::fabs(value) * 256.0 + 0.5);
	return tmp ^ ((-std::signbit(value) ^ tmp) & 0x80000000);
}

inline uint32_t convert_to_s_24_8(float value) {
	auto tmp = static_cast<uint32_t> (std::fabs(value) * 256.0f + 0.5f);
	return tmp ^ ((-std::signbit(value) ^ tmp) & 0x80000000);
}

template <typename T>
inline T convert_from_s_24_8(uint32_t value) {
	auto tmp =
		static_cast<T>((value & 0x7FFFFF00) >> 8) +
		static_cast<T>((value & 0x000000FF) >> 0) * (T{1} / 0xFF);
    tmp *= ((value & 0x80000000) > 0) ? T{-1} : T{1};
	return tmp;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
