// Project: libv.algo, File: src/libv/algo/linear_contains.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range, typename Key>
[[nodiscard]] constexpr inline bool linear_contains(const Range& range, const Key& key) {
	const auto it = std::ranges::find(range, key);
	return it != std::end(range);
}

template <typename Range, typename Pred>
[[nodiscard]] constexpr inline bool linear_contains_if(const Range& range, const Pred& pred) {
	const auto it = std::ranges::find_if(range, pred);
	return it != std::end(range);
}

// -------------------------------------------------------------------------------------------------

} //namespace libv
