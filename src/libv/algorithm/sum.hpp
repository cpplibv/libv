// File:   partial_sum.hpp Author: Vader Created on 08 August 2019, 07:06

#pragma once

// std
#include <numeric>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range>
constexpr inline auto sum(const Range& range, typename Range::value_type init) {
	return std::accumulate(std::begin(range), std::end(range), init);
}

template <typename Range, typename BinaryOperation>
constexpr inline void sum(const Range& range, typename Range::value_type init, BinaryOperation op) {
	return std::accumulate(std::begin(range), std::end(range), init, std::move(op));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
