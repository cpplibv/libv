// Project: libv.algo, File: src/libv/algo/sort.hpp

#pragma once

// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range>
constexpr inline void sort_stable(Range& range) {
	std::stable_sort(range.begin(), range.end());
}

template <typename Range, typename Compare>
constexpr inline void sort_stable(Range& range, Compare&& compare) {
	std::stable_sort(range.begin(), range.end(), std::forward<Compare>(compare));
}

template <typename Range>
constexpr inline void sort_unstable(Range& range) {
	std::sort(range.begin(), range.end());
}

template <typename Range, typename Compare>
constexpr inline void sort_unstable(Range& range, Compare&& compare) {
	std::sort(range.begin(), range.end(), std::forward<Compare>(compare));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
