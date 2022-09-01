// Project: libv.algo, File: src/libv/algo/erase_stable.hpp

#pragma once

// std
#include <algorithm>
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Container, typename Proj = std::identity>
constexpr inline void erase_stable(Container& c, const std::invoke_result_t<Proj, const typename Container::value_type&>& value, Proj&& proj = Proj{}) {
	const auto end = std::end(c);
	for (auto it = std::begin(c); it != end; ++it)
		if (std::invoke(proj, *it) == value) {
			c.erase(it);
			return;
		}
}

template <typename Container, typename Proj = std::identity>
constexpr inline void erase_all_stable(Container& c, const std::invoke_result_t<Proj, const typename Container::value_type&>& value, Proj&& proj = Proj{}) {
	auto [it, end] = std::ranges::remove(c, value, proj);
	c.erase(it, end);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
