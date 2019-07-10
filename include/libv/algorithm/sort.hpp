// File:   partial_sum.hpp Author: Vader Created on 08 August 2019, 07:06

#pragma once

// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range>
constexpr inline void sort(Range& range) {
	std::sort(range.begin(), range.end());
}

template <typename Range, typename Compare>
constexpr inline void sort(Range& range, Compare&& compare) {
	std::sort(range.begin(), range.end(), std::forward<Compare>(compare));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
