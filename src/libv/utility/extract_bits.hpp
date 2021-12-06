// Project: libv.utility, File: src/libv/utility/extract_bits.hpp

#pragma once

// std
#include <cstdint>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P2: extract is the wrong name, it suggests removal. Rename to observe
template <typename T, std::size_t Start, std::size_t Count = sizeof(T) * 8, typename Value>
[[nodiscard]] constexpr inline T extract_high_bits(Value value) noexcept {
	static_assert(Count >= 0);
	static_assert(Start >= 0);
	static_assert(Start <= sizeof(Value) * 8);
	static_assert(Start + Count <= sizeof(Value) * 8);
	static_assert(sizeof(T) * 8 >= Count);
	static_assert(std::is_unsigned_v<Value>);

	Value mask = 0;

	for (std::size_t i = 0; i < Count; ++i)
		mask |= 1ull << i;

	return static_cast<T>(value >> (sizeof(Value) * 8 - Start - Count) & mask);
}

//template <typename T, int Start, int Count = sizeof(T) * 8, typename Value>
//[[nodiscard]] constexpr inline T extract_low_bits(Value value) noexcept {
//}

//template <typename T, int Target, int Start = 0, int Count = sizeof(Value) * 8, typename Value>
//[[nodiscard]] constexpr inline T assemble_high_bits(Value value) noexcept {
//}

//template <typename T, int Target, int Start = 0, int Count = sizeof(Value) * 8, typename Value>
//[[nodiscard]] constexpr inline T assemble_low_bits(Value value) noexcept {
//}

// -------------------------------------------------------------------------------------------------

} // namespace libv
