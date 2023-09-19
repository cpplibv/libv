// Project: libv.algo, File: src/libv/algo/erase_stable.hpp

#pragma once

#include <algorithm>
#include <concepts>
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Container, typename Value, typename Proj = std::identity>
constexpr inline void erase_stable(Container& c, const Value& value, Proj&& proj = Proj{}) {
	static_assert(
			std::equality_comparable_with<Value, std::invoke_result_t<Proj, const typename Container::value_type&>>,
			"Value must be equality comparable with the projection result.");

	const auto end = std::end(c);
	for (auto it = std::begin(c); it != end; ++it)
		if (std::invoke(proj, *it) == value) {
			c.erase(it);
			return;
		}
}

template <typename Container, typename Value, typename Proj = std::identity>
constexpr inline void erase_all_stable(Container& c, const Value& value, Proj&& proj = Proj{}) {
	static_assert(
			std::equality_comparable_with<Value, std::invoke_result_t<Proj, const typename Container::value_type&>>,
			"Value must be equality comparable with the projection result.");

	auto [it, end] = std::ranges::remove(c, value, proj);
	c.erase(it, end);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
