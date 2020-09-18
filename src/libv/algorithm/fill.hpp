// File:   fill.hpp Author: Vader Created on 2018. december 29., 3:30

#pragma once

// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range, typename T>
constexpr inline void fill(Range& range, const T& value) {
	std::fill(range.begin(), range.end(), value);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
