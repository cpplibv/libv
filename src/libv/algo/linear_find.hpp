// Project: libv.algo, File: src/libv/algo/linear_find.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <algorithm>
#include <memory>
// libv
#include <libv/utility/observer_ptr.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range, typename Key>
constexpr inline observer_ptr<typename Range::value_type> linear_find_optional(Range& range, const Key& key) {
	const auto begin = std::begin(range);
	const auto end = std::end(range);

	const auto it = std::find(begin, end, key);
	return libv::make_observer(it != end ? std::addressof(*it) : nullptr);
}

template <typename Range, typename Proj, typename Key>
constexpr inline auto* linear_find_optional(Range& range, const Key& key, const Proj& proj) {
	const auto begin = std::begin(range);
	const auto end = std::end(range);

	const auto it = std::ranges::find(begin, end, key, proj);
	return it != end ? std::addressof(*it) : nullptr;
}

template <typename Range, typename Pred>
constexpr inline observer_ptr<typename Range::value_type> linear_find_if_optional(Range& range, const Pred& pred) {
	const auto begin = std::begin(range);
	const auto end = std::end(range);

	const auto it = std::find_if(begin, end, pred);
	return libv::make_observer(it != end ? std::addressof(*it) : nullptr);
}

template <typename Range, typename Key>
constexpr inline auto linear_find_iterator(Range& range, const Key& key) {
	const auto begin = std::begin(range);
	const auto end = std::end(range);

	return std::find(begin, end, key);
}

template <typename Range, typename Pred>
constexpr inline auto linear_find_if_iterator(Range& range, const Pred& pred) {
	const auto begin = std::begin(range);
	const auto end = std::end(range);

	return std::find_if(begin, end, pred);
}

// -------------------------------------------------------------------------------------------------

} //namespace libv
