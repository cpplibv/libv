// File:   container.hpp Author: Vader Created on 2018. december 29., 3:30

#pragma once

// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Container, typename Pred>
constexpr inline void erase_if_stable(Container& c, Pred&& pred) {
	c.erase(std::remove_if(c.begin(), c.end(), std::forward<Pred>(pred)), c.end());
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
