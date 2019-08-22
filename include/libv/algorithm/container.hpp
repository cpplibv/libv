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

template <typename Container, typename T>
void erase(Container& c, const T& value) {
	c.erase(std::remove(c.begin(), c.end(), value), c.end());
}

template <typename Container, typename Pred>
void erase_if(Container& c, Pred&& pred) {
	c.erase(std::remove_if(c.begin(), c.end(), std::forward<Pred>(pred)), c.end());
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
