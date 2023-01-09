// Project: libv.algo, File: src/libv/algo/adjacent_unique.hpp

#pragma once

// std
#include <algorithm>
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range, typename Proj = std::identity>
[[nodiscard]] constexpr inline bool is_adjacent_unique(const Range& range, Proj&& proj = {}) {
	const auto end = std::end(range);
	return end == std::adjacent_find(std::begin(range), end, [&](const auto& lhs, const auto& rhs) {
		return std::invoke(proj, lhs) == std::invoke(proj, rhs);
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
