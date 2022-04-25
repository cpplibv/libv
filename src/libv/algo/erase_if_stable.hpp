// Project: libv.algo, File: src/libv/algo/erase_if_stable.hpp

#pragma once

// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Container, typename Pred>
constexpr inline void erase_if_stable(Container& c, Pred&& pred) {
	c.erase(std::remove_if(c.begin(), c.end(), std::forward<Pred>(pred)), c.end());
}

template <typename Container, typename Pred>
constexpr inline std::size_t erase_if_stable_count(Container& c, Pred&& pred) {
	const auto it = std::remove_if(c.begin(), c.end(), std::forward<Pred>(pred));
	const auto count = std::distance(it, c.end());
	c.erase(it, c.end());
	return count;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
