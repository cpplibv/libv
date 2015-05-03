// File: String.hpp, Created on 2013. szeptember 1. 0:29, Author: Vader

#pragma once

// std
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <functional> 
#include <cctype>
#include <locale>

namespace vl {
namespace detail {

//Template magic for snprintf and std::string

template <typename T> struct CType {
	using type = T;
};

template <> struct CType<const std::string&> {
	using type = const char*;
};

template<typename T> inline typename CType<const T&>::type toCType(const T& var) {
	return var;
}

template<> inline const char* toCType<std::string>(const std::string& var) {
	return var.c_str();
}

} //namespace detail

// -------------------------------------------------------------------------------------------------
template<typename... Args>
std::string format(const std::string& pattern, const Args&... args) {
	size_t size = snprintf(nullptr, 0, pattern.c_str(), detail::toCType<Args>(args)...) + 1;
	std::unique_ptr<char[] > buf(new char[size]);
	snprintf(buf.get(), size, pattern.c_str(), detail::toCType<Args>(args)...);
	return std::string(buf.get(), buf.get() + size);
}

// -------------------------------------------------------------------------------------------------

// trim from start
inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

// -------------------------------------------------------------------------------------------------

template <typename = void> void unicode_to_utf8(char* out, unsigned int unicode) {
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

template <typename = void> std::string unicode_to_utf8(unsigned int unicode) {
	char buf[5];
	unicode_to_utf8(buf, unicode);
	return std::string(buf);
}

} //namespace vl