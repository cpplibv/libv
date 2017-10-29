// File: String.hpp, Created on 2013. szeptember 1. 0:29, Author: Vader

#pragma once

// ext
#include <utf8cpp/utf8.h>
// std
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

// -------------------------------------------------------------------------------------------------

} // namespace libv
