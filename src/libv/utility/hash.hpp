// Project: libv.utility, File: src/libv/utility/hash.hpp

#pragma once

// std
#include <cstdint>
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

// Integer hashers
// Original source: https://stackoverflow.com/a/12996028

[[nodiscard]] constexpr inline uint32_t hash_int(uint32_t x) noexcept {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

[[nodiscard]] constexpr inline uint32_t unhash_int(uint32_t x) noexcept {
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = (x >> 16) ^ x;
    return x;
}

[[nodiscard]] constexpr inline uint64_t hash_int(uint64_t x) noexcept {
	x = (x ^ (x >> 30)) * uint64_t(0xbf58476d1ce4e5b9);
	x = (x ^ (x >> 27)) * uint64_t(0x94d049bb133111eb);
	x = x ^ (x >> 31);
	return x;
}

[[nodiscard]] constexpr inline uint64_t unhash_int(uint64_t x) noexcept {
	x = (x ^ (x >> 31) ^ (x >> 62)) * uint64_t(0x319642b2d24d8ec3);
	x = (x ^ (x >> 27) ^ (x >> 54)) * uint64_t(0x96de1b173f119089);
	x = x ^ (x >> 30) ^ (x >> 60);
	return x;
}

// -------------------------------------------------------------------------------------------------

template<typename... Args>
constexpr inline std::size_t hash_combine(const Args&... args) noexcept {
	std::size_t seed = 0;
	((void) (seed ^= std::hash<Args>{}(args) + 0x9e3779b9 + (seed << 6) + (seed >> 2)), ...);
	return seed;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

/// Creates std::hash support for a given type based on its members combined hash
/// @note This macro must be called from the global scope
/// @example LIBV_MAKE_HASHABLE(MyClass, t.key1, t.key2, t.key3)
#define LIBV_MAKE_HASHABLE(TYPE, ...)                                                              \
namespace std {                                                                                    \
	template <> struct hash<TYPE> {                                                                \
		static constexpr std::size_t operator()(const TYPE &t) noexcept {                          \
			return ::libv::hash_combine(__VA_ARGS__);                                              \
}};}
