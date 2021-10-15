// Project: libv.algo, File: src/libv/algo/outer_join.hpp

#pragma once

// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename RangeLeft, typename RangeRight, typename Func,
		typename Comp = std::ranges::less, typename Proj = std::identity>
void outer_join(const RangeLeft& range_left, const RangeRight& range_right, Func&& func, Comp comp = {}, Proj proj = {}) {
	auto left = std::begin(range_left);
	auto right = std::begin(range_right);
	const auto left_end = std::end(range_left);
	const auto right_end = std::end(range_right);

	const auto left_null = static_cast<decltype(&*left)>(nullptr);
	const auto right_null = static_cast<decltype(&*right)>(nullptr);

	if (left != left_end && right != right_end)
		while (true) {
			if (comp(std::invoke(proj, *left), std::invoke(proj, *right))) {
				func(&*left, right_null);
				++left;
				if (left == left_end)
					break;
				continue;

			} else if (comp(std::invoke(proj, *right), std::invoke(proj, *left))) {
				func(left_null, &*right);
				++right;
				if (right == right_end)
					break;
				continue;

			} else {
				func(&*left, &*right);
				++left;
				++right;
				if (left == left_end || right == right_end)
					break;
				continue;
			}
		}

	while (left != left_end) {
		func(&*left, right_null);
		++left;
	}

	while (right != right_end) {
		func(left_null, &*right);
		++right;
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
