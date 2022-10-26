// Project: libv.algo, File: src/libv/algo/linear_find.hpp

#pragma once

// std
#include <algorithm>
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range, typename Key, typename Proj = std::identity>
[[nodiscard]] constexpr inline auto linear_find_iterator(Range&& range, const Key& key, Proj proj = {}) {
	// NOTE: The concepts on std::ranges::find gives trouble if only operator==(const ValueType&, const Key&) is provided
	//	return std::ranges::find(range, key, proj);

	const auto end = std::end(range);
	for (auto it = std::begin(range); it != end; ++it)
		if (std::invoke(proj, *it) == key)
			return it;

	return end;
}

template <typename Range, typename Pred, typename Proj = std::identity>
[[nodiscard]] constexpr inline auto linear_find_if_iterator(Range&& range, Pred pred, Proj proj = {}) {
	return std::ranges::find_if(range, pred, proj);
}

template <typename Int = int, typename Range, typename Key, typename Proj = std::identity>
[[nodiscard]] constexpr inline Int linear_find_index(Range&& range, const Key& key, Proj proj = {}) {
	const auto end = std::end(range);
	for (auto it = std::begin(range); it != end; ++it)
		if (std::invoke(proj, *it) == key)
			return static_cast<Int>(std::distance(std::begin(range), it));

	return Int{-1};
}

template <typename Range, typename Key, typename Proj = std::identity>
[[nodiscard]] constexpr inline auto* linear_find_optional(Range&& range, const Key& key, Proj proj = {}) {
	// NOTE: The concepts on std::ranges::find gives trouble if only operator==(const ValueType&, const Key&) is provided
	//	const auto it = std::ranges::find(range, key, proj);

	const auto it = linear_find_iterator(range, key, proj);
	return it != std::end(range) ? std::addressof(*it) : nullptr;
}

template <typename Range, typename Pred, typename Proj = std::identity>
[[nodiscard]] constexpr inline auto* linear_find_if_optional(Range&& range, Pred pred, Proj proj = {}) {
	const auto it = std::ranges::find_if(range, pred, proj);
	return it != std::end(range) ? std::addressof(*it) : nullptr;
}

// -------------------------------------------------------------------------------------------------

template <typename Range, typename Key, typename Proj = std::identity>
[[nodiscard]] constexpr inline auto linear_find_last_iterator(Range&& range, const Key& key, Proj proj = {}) {
	// NOTE: The concepts on std::ranges::find gives trouble if only operator==(const ValueType&, const Key&) is provided
	//	return std::ranges::find(range, key, proj);

	const auto end = std::rend(range);
	for (auto it = std::rbegin(range); it != end; ++it)
		if (std::invoke(proj, *it) == key)
			return it;

	return end;
}

template <typename Range, typename Key, typename Proj = std::identity>
[[nodiscard]] constexpr inline auto* linear_find_last_optional(Range&& range, const Key& key, Proj proj = {}) {
	// NOTE: The concepts on std::ranges::find gives trouble if only operator==(const ValueType&, const Key&) is provided
	//	const auto it = std::ranges::find(range, key, proj);

	const auto it = linear_find_last_iterator(range, key, proj);
	return it != std::rend(range) ? std::addressof(*it) : nullptr;
}

// -------------------------------------------------------------------------------------------------

} //namespace libv
