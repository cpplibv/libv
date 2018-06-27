// File:   slice.hpp Author: Vader Created on 2018. május 28., 19:03

#pragma once

// std
#include <cassert>
#include <string>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Returns a string_view to the substring taken from the range of chars at offsets [lo, hi).
/// If either of lo or hi is a negative value x, x is taken to be an offset from the end,
/// and so x + size() is used instead.
/// These preconditions apply to the values used after size() is added to any negative arguments.
/// \pre 0 \<= lo && lo \<= size()
/// \pre 0 \<= hi && lhi \<= size()
/// \pre lo \<= hi
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

/// Returns a string_view to to substring taken from the first cut chars when cut => 0,
/// or the last -cut chars when cut \< 0.
/// \pre 0 \<= cut && cut \<= size() || 0 \<= -cut && -cut \<= size()
constexpr inline std::string_view slice_view(const std::string_view str, int64_t cut) noexcept {
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

constexpr inline std::string_view slice_prefix_view(std::string_view str, std::string_view prefix) {
	if (str.substr(0, prefix.size()) == prefix)
		str.remove_prefix(prefix.size());
	return str;
}

constexpr inline std::string_view slice_suffix_view(std::string_view str, const std::string_view suffix) {
	if (str.substr(str.size() - suffix.size(), suffix.size()) == suffix)
		str.remove_prefix(suffix.size());
	return str;
}

inline std::string slice_copy(const std::string_view str, int64_t lo, int64_t hi) {
	return std::string{slice_view(str, lo, hi)};
}

inline std::string slice_copy(const std::string_view str, int64_t cut) {
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
inline void slice_inplace(String& str, int64_t lo, int64_t hi);

template <typename String>
inline void slice_inplace(String& str, int64_t cut);

template <typename String>
inline void slice_prefix_inplace(String& str, const std::string_view prefix);

template <typename String>
inline void slice_suffix_inplace(String& str, const std::string_view suffix);

// -------------------------------------------------------------------------------------------------

} // namespace libv
