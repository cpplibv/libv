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
	using Type = T;
};

template <> struct CType<const std::string&> {
	using Type = const char*;
};

template<typename T> inline typename CType<const T&>::Type toCType(const T& var) {
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
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

// -------------------------------------------------------------------------------------------------

template <typename = void> std::string unicode_to_utf8(unsigned int codepoint) {
	std::string out;
	if (codepoint <= 0x7f) {
		out.resize(1);
		out[0] = static_cast<char> (codepoint);
	} else if (codepoint <= 0x7ff) {
		out.resize(2);
		out[0] = static_cast<char> (0xc0 | ((codepoint >> 6) & 0x1f));
		out[1] = static_cast<char> (0x80 | (codepoint & 0x3f));
	} else if (codepoint <= 0xffff) {
		out.resize(3);
		out[0] = static_cast<char> (0xe0 | ((codepoint >> 12) & 0x0f));
		out[1] = static_cast<char> (0x80 | ((codepoint >> 6) & 0x3f));
		out[2] = static_cast<char> (0x80 | (codepoint & 0x3f));
	} else {
		out.resize(4);
		out[0] = static_cast<char> (0xf0 | ((codepoint >> 18) & 0x07));
		out[1] = static_cast<char> (0x80 | ((codepoint >> 12) & 0x3f));
		out[2] = static_cast<char> (0x80 | ((codepoint >> 6) & 0x3f));
		out[3] = static_cast<char> (0x80 | (codepoint & 0x3f));
	}
	return out;
}

} //namespace vl