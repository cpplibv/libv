// Project: libv.algo, File: src/libv/algo/sort.hpp

#pragma once

// std
#include <algorithm>
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range, typename Comp = std::less<>, typename Proj = std::identity>
[[nodiscard]] constexpr inline bool is_sorted(const Range& range, Comp&& comp = {}, Proj&& proj = {}) {
	return std::is_sorted(std::begin(range), std::end(range), [&](const auto& lhs, const auto& rhs) {
		return comp(std::invoke(proj, lhs), std::invoke(proj, rhs));
	});
}

template <typename Range, typename Comp = std::less<>, typename Proj = std::identity>
constexpr inline void sort_stable(Range& range, Comp&& comp = {}, Proj&& proj = {}) {
    return std::stable_sort(std::begin(range), std::end(range), [&](const auto& lhs, const auto& rhs) {
        return comp(std::invoke(proj, lhs), std::invoke(proj, rhs));
    });
}

template <typename Range, typename Comp = std::less<>, typename Proj = std::identity>
constexpr inline void sort_unstable(Range& range, Comp&& comp = {}, Proj&& proj = {}) {
    return std::sort(std::begin(range), std::end(range), [&](const auto& lhs, const auto& rhs) {
        return comp(std::invoke(proj, lhs), std::invoke(proj, rhs));
    });
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
