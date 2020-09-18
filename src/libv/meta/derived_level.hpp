// Project: libv.meta, File: src/libv/meta/derived_level.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

template <int N> struct derived_level : derived_level<N - 1> {
	static constexpr int value = N;
};

template <> struct derived_level<0> {
	static constexpr int value = 0;
};

using derived_top = derived_level<255>;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
