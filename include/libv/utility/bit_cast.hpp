// File: bit_cast.hpp - Created on 2018.05.05. 04:31 - Author: Vader

#pragma once

// std
#include <cstring>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <class Target, class Source>
constexpr inline Target bit_cast(const Source& source) {
	static_assert(sizeof (Target) == sizeof (Source), "bit_cast the sizeof Source and the Target has to be equal");
	Target dest;
	std::memcpy(&dest, &source, sizeof (dest));
	return dest;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
