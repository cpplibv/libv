// Project: libv.algo, File: src/libv/algo/fill.hpp

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
