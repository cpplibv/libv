// File:   container.hpp Author: Vader Created on 2018. december 29., 3:30

#pragma once

// std
#include <algorithm>

// TODO P5: Filename is a placeholder while a pattern is being established


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename RangeTarget, typename RangeSource>
constexpr inline void insert_all(RangeTarget& target, const RangeSource& source) {
	target.insert(target.end(), source.begin(), source.end());
}

template <typename Container>
constexpr inline void erase_unstable(Container& container, typename Container::iterator it) {
	const auto last = container.begin() + container.size();
	if (it != last)
		*it = std::move(*last);

	container.erase(last);
}

template <typename Container, typename T, typename Comparator>
constexpr inline typename Container::iterator lower_bound(Container& container, const T& value, Comparator c) {
	return std::lower_bound(container.begin(), container.end(), value, c);
}

template <typename Container, typename T, typename Comparator>
constexpr inline typename Container::iterator upper_bound(Container& container, const T& value, Comparator c) {
	return std::upper_bound(container.begin(), container.end(), value, c);
}

template <typename Container, typename T>
constexpr inline typename Container::iterator lower_bound(Container& container, const T& value) {
	return std::lower_bound(container.begin(), container.end(), value);
}

template <typename Container, typename T>
constexpr inline typename Container::iterator upper_bound(Container& container, const T& value) {
	return std::upper_bound(container.begin(), container.end(), value);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv