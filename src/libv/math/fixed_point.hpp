// Project: libv.math, File: src/libv/math/fixed_point.hpp

#pragma once

// std
// #include <cassert>
#include <cmath>
#include <cstdint>


namespace libv {

// -------------------------------------------------------------------------------------------------

// template <typename T>
// [[nodiscard]] constexpr inline T extract_bits(const void* data, uint32_t offset, uint32_t numBits) {
// 	assert(numBits <= sizeof(T) * 8);
//
// 	const auto* source = static_cast<const uint8_t*>(data);
// 	uint8_t target[sizeof(T)]{0};
//
// 	for (uint32_t i = 0; i < numBits; ++i) {
// 		const auto sourceBitIndex = offset + i;
// 		const auto sourceByteIndex = sourceBitIndex / 8;
// 		const auto sourceBitSubByteIndex = sourceBitIndex % 8;
// 		const auto sourceBitSubByteMask = 1u << sourceBitSubByteIndex;
// 		const auto sourceBitValue = (source[sourceByteIndex] & sourceBitSubByteMask) != 0;
// 		const auto targetBitIndex = i;
// 		const auto targetByteIndex = targetBitIndex / 8;
// 		const auto targetBitSubByteIndex = targetBitIndex % 8;
// 		target[targetByteIndex] |= sourceBitValue ? 1u << targetBitSubByteIndex : 0u;
// 	}
//
// 	return std::bit_cast<T>(target);
// }

// -------------------------------------------------------------------------------------------------

inline uint32_t convert_to_s24_8(double value) {
	auto tmp = static_cast<uint32_t>(std::fabs(value) * 256.0 + 0.5);
	return tmp ^ ((-std::signbit(value) ^ tmp) & 0x80000000);
}

inline uint32_t convert_to_s24_8(float value) {
	auto tmp = static_cast<uint32_t>(std::fabs(value) * 256.0f + 0.5f);
	return tmp ^ ((-std::signbit(value) ^ tmp) & 0x80000000);
}

template <typename T>
inline T convert_from_s24_8(uint32_t value) {
	auto tmp =
		static_cast<T>((value & 0x7FFFFF00) >> 8) +
		static_cast<T>((value & 0x000000FF) >> 0) * (T{1} / 0xFF);
    tmp *= ((value & 0x80000000) > 0) ? T{-1} : T{1};
	return tmp;
}

template <typename T>
inline T convert_from_16_16(uint32_t value) {
	return static_cast<T>((value & 0xFFFF0000) >> 16) +
		static_cast<T>((value & 0x0000FFFF) >> 0) * (T{1} / 0xFFFF);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
