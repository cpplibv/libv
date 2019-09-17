// File: String.hpp, Created on 2013. szeptember 1. 0:29, Author: Vader

#pragma once

// ext
#include <utf8cpp/utf8.h>
// std
#include <array>
#include <string>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// output size has to be at least 5 char (four octet + terminating zero)
template <typename = void> void unicode_to_utf8(uint32_t unicode, char* out) {
	const auto end = utf8::append(unicode, out);
	*end = '\0';
}

template <typename = void> std::string unicode_to_utf8(uint32_t unicode) {
	char buf[5];
	unicode_to_utf8(unicode, buf);
	return std::string(buf);
}

template <typename = void> auto unicode_to_utf8_array(uint32_t unicode) {
	std::array<char, 5> buf;
	unicode_to_utf8(unicode, buf.data());
	return buf;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
