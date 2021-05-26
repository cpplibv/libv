// Project: libv.algo, File: src/libv/algo/outer_join.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <algorithm>


namespace libv {
namespace algo {

// -------------------------------------------------------------------------------------------------

// TODO P3: Implement range view adjacent pairs: for (const auto& [a, b] : points | view::adjacent);

template <typename Range, typename Func, typename Proj = std::identity>
void adjacent_pairs(const Range& range, Func&& func, Proj proj = {}) {
	auto right = std::begin(range);
	const auto end = std::end(range);
	if (right == end)
		return;

	auto left = right++;
	if (right == end)
		return;

	while (right != end) {
		func(std::invoke(proj, *left), std::invoke(proj, *right));
		left = right++;
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace algo
} // namespace libv
