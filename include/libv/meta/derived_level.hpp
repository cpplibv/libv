// File: derived_level.hpp Author: Vader Created on 2017. június 20., 1:41

#pragma once

// std
#include <cstddef>


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

template <size_t N> struct derived_level : derived_level<N - 1> {
	static constexpr size_t value = N;
};

template <> struct derived_level<0> {
	static constexpr size_t value = 0;
};

using derived_top = derived_level<255>;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
