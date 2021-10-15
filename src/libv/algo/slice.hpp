// Project: libv.algo, File: src/libv/algo/slice.hpp

#pragma once

// std
#include <algorithm>
#include <string>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------


/// Returns a string_view to the sub-string of characters in [\c lo, \c hi).
/// If either of \c lo or \c hi is a negative value x, x is taken to be an offset from the end,
/// and so x + size() is used instead.
/// If either \c lo or \c hi would over index than the index is clamped to the size of the string.
/// @param str - the source string
/// @param lo - the index of the first requested character
/// @param hi - the index of the last requested character
/// @example `slice_view("Hello world!",  0,  5) == "Hello"`
/// @example `slice_view("Hello world!",  5, -1) == " world"`
/// @example `slice_view("Hello world!", -7, -1) == " world"`
[[nodiscard]] constexpr inline std::string_view slice_view(const std::string_view str, const int64_t lo, const int64_t hi) noexcept {
	const auto size = static_cast<int64_t>(str.size());

	const auto index_lo = std::clamp(lo < 0 ? lo + size : lo, int64_t{0}, size);
	const auto index_hi = std::clamp(hi < 0 ? hi + size : hi, int64_t{0}, size);
	const auto length = static_cast<size_t>(std::max(index_hi - index_lo, int64_t{0}));

	return std::string_view{str.data() + index_lo, length};
}

/// Returns a string_view to the sub-string of the
/// - prefixing `abs(\c cut)` characters if \c cut is positive or
/// - suffixing `abs(\c cut)` characters if \c cut is negative
/// If `abs(\c cut)` is greater then the length of the string, the whole string will be returned
/// @param str - the source string
/// @param cut - the number of characters requested
/// @example `slice_view("Hello world!", 5) == "Hello"`
/// @example `slice_view("Hello world!", -6) == "world!"`
[[nodiscard]] constexpr inline std::string_view slice_view(const std::string_view str, const int64_t cut) noexcept {
	if (cut < 0)
		return slice_view(str, str.size() + cut, str.size());
	else
		return slice_view(str, 0, cut);
}

/// Returns a string_view to the sub-string of the
/// - prefixing `abs(\c cut)` characters cut off if \c cut is positive or
/// - suffixing `abs(\c cut)` characters cut off if \c cut is negative
/// If `abs(\c cut)` is greater then the length of the string, an empty string will be returned
/// @param str - the source string
/// @param cut - the number of characters to cut off
/// @example `slice_off_view("Hello world!", 6) == "world!"`
/// @example `slice_off_view("Hello world!", -7) == "Hello"`
[[nodiscard]] constexpr inline std::string_view slice_off_view(const std::string_view str, const int64_t cut) noexcept {
	if (cut < 0) {
		const auto cut_s = static_cast<size_t>(std::abs(cut));

		if (cut_s > str.size())
			return "";
		else
			return std::string_view(str.data(), str.size() - cut_s);

	} else {
		const auto cut_s = static_cast<size_t>(cut);

		if (cut_s > str.size())
			return "";
		else
			return std::string_view(str.data() + cut_s, str.size() - cut_s);
	}
}

[[nodiscard]] constexpr inline std::string_view slice_prefix_view(const std::string_view str, const std::string_view prefix) noexcept {
	auto result = str;
	if (result.substr(0, prefix.size()) == prefix)
		result.remove_prefix(prefix.size());
	return result;
}

[[nodiscard]] constexpr inline std::string_view slice_suffix_view(const std::string_view str, const std::string_view suffix) noexcept {
	auto result = str;
	if (result.substr(str.size() - suffix.size(), suffix.size()) == suffix)
		result.remove_prefix(suffix.size());
	return result;
}

[[nodiscard]] inline std::string slice_copy(const std::string_view str, const int64_t lo, const int64_t hi) {
	return std::string{slice_view(str, lo, hi)};
}

[[nodiscard]] inline std::string slice_copy(const std::string_view str, const int64_t cut) {
	return std::string{slice_view(str, cut)};
}

[[nodiscard]] inline std::string slice_off_copy(const std::string_view str, const int64_t cut) {
	return std::string{slice_off_view(str, cut)};
}

[[nodiscard]] inline std::string slice_prefix_copy(const std::string_view str, const std::string_view prefix) {
	return std::string{slice_prefix_view(str, prefix)};
}

[[nodiscard]] inline std::string slice_suffix_copy(const std::string_view str, const std::string_view suffix) {
	return std::string{slice_suffix_view(str, suffix)};
}

template <typename String>
inline void slice_inplace(String& str, const int64_t lo, const int64_t hi);
	// TODO P5: implement slice_inplace

template <typename String>
inline void slice_inplace(String& str, const int64_t cut);
	// TODO P5: implement slice_inplace

template <typename String>
inline void slice_off_inplace(String& str, const int64_t cut);
	// TODO P5: implement slice_off_inplace

template <typename String>
inline void slice_prefix_inplace(String& str, const std::string_view prefix) {
	if (str.size() < prefix.size())
		return;

	if (std::string_view{str.data(), prefix.size()} != prefix)
		return;

	str.erase(str.begin(), str.begin() + prefix.size());
}

template <typename String>
inline void slice_suffix_inplace(String& str, const std::string_view suffix) {
	if (str.size() < suffix.size())
		return;

	if (std::string_view{str.data() + (str.size() - suffix.size()), suffix.size()} != suffix)
		return;

	str.erase(str.begin() + (str.size() - suffix.size()), str.end());
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
