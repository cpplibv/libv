// File: hash.hpp - Created on 2017.10.21. 03:06 - Author: Vader

#pragma once

// std
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

template<typename... Args>
constexpr inline size_t hash_combine(const Args&... args) noexcept {
	size_t seed = 0;
	((void) (seed ^= std::hash<Args>{}(args) + 0x9e3779b9 + (seed << 6) + (seed >> 2)), ...);
	return seed;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

/// Creates std::hash support for a given type based on its members combined hash
/// @note This macro has to be called from the global scope
/// @example LIBV_MAKE_HASHABLE(MyClass, t.key1, t.key2, t.key3)
#define LIBV_MAKE_HASHABLE(TYPE, ...)                                                              \
namespace std {                                                                                    \
	template <> struct hash<TYPE> {                                                                \
		size_t operator()(const TYPE &t) const {                                                   \
			return ::libv::hash_combine(__VA_ARGS__);                                              \
}};}
