// File: String.hpp, Created on 2013. szeptember 1. 0:29, Author: Vader

#pragma once

// ext
#include <utf8cpp/utf8.h>
// std
#include <array>
#include <string>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// output size has to be at least 5 char (four octet + terminating zero)
template <typename = void>
void unicode_to_utf8(uint32_t unicode, char* out) {
	const auto end = utf8::append(unicode, out);
	*end = '\0';
}

template <typename = void>
[[nodiscard]] std::string unicode_to_utf8(uint32_t unicode) {
	char buf[5];
	unicode_to_utf8(unicode, buf);
	return std::string(buf);
}

template <typename = void>
[[nodiscard]] auto unicode_to_utf8_array(uint32_t unicode) {
	std::array<char, 5> buf;
	unicode_to_utf8(unicode, buf.data());
	return buf;
}

// -------------------------------------------------------------------------------------------------

/// output size has to be at least 5 char (four octet + terminating zero)
template <typename = void>
void unicode_to_utf8_unchecked(uint32_t unicode, char* out) noexcept {
	const auto end = utf8::unchecked::append(unicode, out);
	*end = '\0';
}

template <typename = void>
[[nodiscard]] std::string unicode_to_utf8_unchecked(uint32_t unicode) noexcept {
	char buf[5];
	unicode_to_utf8_unchecked(unicode, buf);
	return std::string(buf);
}

template <typename = void>
[[nodiscard]] auto unicode_to_utf8_array_unchecked(uint32_t unicode) noexcept {
	std::array<char, 5> buf;
	unicode_to_utf8_unchecked(unicode, buf.data());
	return buf;
}

template <typename = void>
void insert_utf8_unchecked(std::string& string, size_t position, std::string_view segment) {
	auto it = string.begin();
	utf8::unchecked::advance(it, position);
	string.insert(it, segment.begin(), segment.end());
}

template <typename = void>
void insert_utf8_unchecked(std::string& string, size_t position, uint32_t unicode) {
	const auto utf8array = libv::unicode_to_utf8_array_unchecked(unicode);

	insert_utf8_unchecked(string, position, utf8array.data());
}

// -------------------------------------------------------------------------------------------------

/// Returns true on success, returns false if end was reached before count step were taken
template <typename octet_iterator>
constexpr inline bool advance_utf8_unchecked(octet_iterator& it, const octet_iterator end, size_t count) noexcept {
	for (size_t i = 0; i < count; ++i) {
		if (it == end)
			return false;
		it += utf8::internal::sequence_length(it);
	}
	return true;
}

template <typename octet_iterator>
[[nodiscard]] constexpr inline size_t distance_utf8_unchecked(octet_iterator it, const octet_iterator end) noexcept {
	size_t result = 0;

	for (; it != end; it += utf8::internal::sequence_length(it))
		result++;

	return result;
}

template <typename octet_range>
[[nodiscard]] constexpr inline size_t distance_utf8_unchecked(const octet_range& string) noexcept {
	return distance_utf8_unchecked(string.begin(), string.end());
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
