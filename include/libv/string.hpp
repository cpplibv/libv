// File: String.hpp, Created on 2013. szeptember 1. 0:29, Author: Vader

#pragma once

// std
#include <algorithm>
#include <string>

namespace libv {

// -------------------------------------------------------------------------------------------------
inline std::string& trim_begin(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}
inline std::string& trim_end(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}
inline std::string& trim(std::string &s) {
	return trim_begin(trim_end(s));
}

// -------------------------------------------------------------------------------------------------
template <typename = void> void unicode_to_utf8(char* out, uint32_t unicode) {
	if (unicode <= 0x7f) {
		out[0] = static_cast<char> (unicode);
		out[1] = '\0';
	} else if (unicode <= 0x7ff) {
		out[0] = static_cast<char> (0xc0 | ((unicode >> 6) & 0x1f));
		out[1] = static_cast<char> (0x80 | (unicode & 0x3f));
		out[2] = '\0';
	} else if (unicode <= 0xffff) {
		out[0] = static_cast<char> (0xe0 | ((unicode >> 12) & 0x0f));
		out[1] = static_cast<char> (0x80 | ((unicode >> 6) & 0x3f));
		out[2] = static_cast<char> (0x80 | (unicode & 0x3f));
		out[3] = '\0';
	} else {
		out[0] = static_cast<char> (0xf0 | ((unicode >> 18) & 0x07));
		out[1] = static_cast<char> (0x80 | ((unicode >> 12) & 0x3f));
		out[2] = static_cast<char> (0x80 | ((unicode >> 6) & 0x3f));
		out[3] = static_cast<char> (0x80 | (unicode & 0x3f));
		out[4] = '\0';
	}
}
template <typename = void> std::string unicode_to_utf8(uint32_t unicode) {
	char buf[5];
	unicode_to_utf8(buf, unicode);
	return std::string(buf);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv