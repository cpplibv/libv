// Project: libv.algo, File: src/libv/algo/linear_find.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range, typename Key, typename Proj = std::identity>
[[nodiscard]] constexpr inline auto linear_find_iterator(Range& range, const Key& key, Proj proj = {}) {

//	const auto begin = std::begin(range);
//	const auto end = std::end(range);

//	return std::find(begin, end, key);
//	return std::ranges::find(range, key, proj);
//	return std::ranges::find(begin, end, key, proj);
	return std::ranges::find(range, key, proj);
}

template <typename Range, typename Pred, typename Proj = std::identity>
[[nodiscard]] constexpr inline auto linear_find_if_iterator(Range& range, Pred pred, Proj proj = {}) {
	return std::ranges::find_if(range, pred, proj);
}

template <typename Range, typename Key, typename Proj = std::identity>
[[nodiscard]] constexpr inline auto* linear_find_optional(Range& range, const Key& key, Proj proj = {}) {
	const auto it = std::ranges::find(range, key, proj);
	return it != std::end(range) ? std::addressof(*it) : nullptr;
}

template <typename Range, typename Pred, typename Proj = std::identity>
[[nodiscard]] constexpr inline auto* linear_find_if_optional(Range& range, Pred pred, Proj proj = {}) {
	const auto it = std::ranges::find_if(range, pred, proj);
	return it != std::end(range) ? std::addressof(*it) : nullptr;
}

// -------------------------------------------------------------------------------------------------

} //namespace libv
