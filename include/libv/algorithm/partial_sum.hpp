// File:   partial_sum.hpp Author: Vader Created on 08 August 2019, 07:06

#pragma once

// std
#include <numeric>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Range>
constexpr inline void partial_sum(Range& range) {
	std::partial_sum(std::begin(range), std::end(range), std::begin(range));
}

template <typename Range, typename BinaryOperation>
constexpr inline void partial_sum(Range& range, BinaryOperation op) {
	std::partial_sum(std::begin(range), std::end(range), std::begin(range), std::move(op));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
