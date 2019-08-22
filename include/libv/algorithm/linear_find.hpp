// File: associative.hpp Author: Vader Created on December 26, 2018, 5:26 AM

#pragma once

// std
#include <algorithm>
#include <memory>
// libv
#include <libv/utility/observer_ptr.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range, typename Key>
observer_ptr<typename Range::value_type> linear_find_optional(Range& range, const Key& key) {
	const auto begin = std::begin(range);
	const auto end = std::end(range);

	const auto it = std::find(begin, end, key);
	return libv::make_observer(it != end ? std::addressof(*it) : nullptr);
}

template <typename Range, typename Pred>
observer_ptr<typename Range::value_type> linear_find_if_optional(Range& range, const Pred& pred) {
	const auto begin = std::begin(range);
	const auto end = std::end(range);

	const auto it = std::find_if(begin, end, pred);
	return libv::make_observer(it != end ? std::addressof(*it) : nullptr);
}

// -------------------------------------------------------------------------------------------------

} //namespace libv
