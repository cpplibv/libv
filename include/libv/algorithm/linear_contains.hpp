// File: associative.hpp Author: Vader Created on December 26, 2018, 5:26 AM

#pragma once

// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range, typename Key>
[[nodiscard]] constexpr inline bool linear_contains(const Range& range, const Key& key) {
	const auto begin = std::begin(range);
	const auto end = std::end(range);

	const auto it = std::find(begin, end, key);
	return it != end;
}

template <typename Range, typename Pred>
[[nodiscard]] constexpr inline bool linear_contains_if(const Range& range, const Pred& pred) {
	const auto begin = std::begin(range);
	const auto end = std::end(range);

	const auto it = std::find_if(begin, end, pred);
	return it != end;
}

// -------------------------------------------------------------------------------------------------

} //namespace libv
