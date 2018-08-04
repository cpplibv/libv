// File:   slice.hpp Author: Vader Created on 2018. május 28., 19:03

#pragma once

// std
#include <cassert>
#include <string>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Returns a string_view to the sub-string of characters in [lo, hi).
/// If either of lo or hi is a negative value x, x is taken to be an offset from the end,
/// and so x + size() is used instead.
/// @param str - the source string
/// @param lo - the index of the first requested character
/// @param hi - the index of the last requested character
/// @pre `0 \<= abs(lo) \<= size()`
/// @pre `0 \<= abs(hi) \<= size()`
/// @pre `lo \<= hi` This preconditions apply to the values used after size() is added to any negative number
/// @example `slice_view("Hello world!",  0,  5) == "Hello"`
/// @example `slice_view("Hello world!",  5, -1) == " world"`
/// @example `slice_view("Hello world!", -7, -1) == " world"`
constexpr inline std::string_view slice_view(const std::string_view str, int64_t lo, int64_t hi) noexcept {
	if (lo < 0)
		lo += str.size();
	if (hi < 0)
		hi += str.size();

	assert(0 <= lo && lo <= str.size());
	assert(0 <= hi && hi <= str.size());
	assert(lo <= hi);

	return std::string_view(str.data() + lo, hi - lo);
}

/// Returns a string_view to the sub-string of the
/// - prefixing `abs(cut)` characters if `cut` is positive or
/// - suffixing `abs(cut)` characters if `cut` is negative
/// @param str - the source string
/// @param cut - the number of characters requested
/// @pre `0 \<= abs(cut) \<= size()`
/// @example `slice_view("Hello world!", 5) == "Hello"`
/// @example `slice_view("Hello world!", -6) == "world!"`
constexpr inline std::string_view slice_view(const std::string_view str, const int64_t cut) noexcept {
	int64_t lo = 0;
	int64_t hi = cut;

	if (cut < 0) {
		lo = cut + str.size();
		hi = str.size();
	}

	assert(0 <= lo && lo <= str.size());
	assert(0 <= hi && hi <= str.size());

	return std::string_view(str.data() + lo, hi - lo);
}

constexpr inline std::string_view slice_prefix_view(const std::string_view str, const std::string_view prefix) {
	auto result = str;
	if (result.substr(0, prefix.size()) == prefix)
		result.remove_prefix(prefix.size());
	return result;
}

constexpr inline std::string_view slice_suffix_view(const std::string_view str, const std::string_view suffix) {
	auto result = str;
	if (result.substr(str.size() - suffix.size(), suffix.size()) == suffix)
		result.remove_prefix(suffix.size());
	return result;
}

inline std::string slice_copy(const std::string_view str, const int64_t lo, const int64_t hi) {
	return std::string{slice_view(str, lo, hi)};
}

inline std::string slice_copy(const std::string_view str, const int64_t cut) {
	return std::string{slice_view(str, cut)};
}

inline std::string slice_prefix_copy(const std::string_view str, const std::string_view prefix) {
	return std::string{slice_prefix_view(str, prefix)};
}

inline std::string slice_suffix_copy(const std::string_view str, const std::string_view suffix) {
	return std::string{slice_suffix_view(str, suffix)};
}

// TODO P5: implement inplace slices

template <typename String>
inline void slice_inplace(String& str, const int64_t lo, const int64_t hi);

template <typename String>
inline void slice_inplace(String& str, const int64_t cut);

template <typename String>
inline void slice_prefix_inplace(String& str, const std::string_view prefix);

template <typename String>
inline void slice_suffix_inplace(String& str, const std::string_view suffix);

// -------------------------------------------------------------------------------------------------

} // namespace libv
