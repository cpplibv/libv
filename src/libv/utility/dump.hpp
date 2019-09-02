// File:   enclose_delimit.hpp Author: Vader Created on 02 September 2019, 02:34

#pragma once

// ext
#include <range/v3/utility/iterator.hpp> // Ranges version (remove after C++20)
// std
#include <algorithm>
//#include <iterator> // C++20 version
#include <ostream>
#include <string_view>
//#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range>
std::ostream& enclose_delimit(
		std::ostream& os,
		const Range& range,
		const std::string_view delimit = ", ",
		const std::string_view start = "{",
		const std::string_view end = "}") {
	os << start;
//	std::ostream_joiner oj{os, delimit}; // C++20 version
	ranges::ostream_joiner oj{os, delimit}; // Ranges version (remove after C++20)
	std::copy(range.begin(), range.end(), oj);
	os << end;
	return os;
}

// -------------------------------------------------------------------------------------------------

//template <typename T,
//		typename = std::enable_if_t<std::ar>>
//std::ostream& dump(std::ostream& os, const T& variable) {
//	os << variable;
//	return os;
//}

//template <typename T>
//std::ostream& dump(std::ostream& os, const T& variable) {
//	os << variable;
//	return os;
//}

// -------------------------------------------------------------------------------------------------

} // namespace libv
