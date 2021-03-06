// Project: libv.meta, File: src/libv/meta/for_constexpr.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstddef> // consider changing size_t usage to int or T in this file
#include <utility>

// TODO P5: Take a look at boost::callable_traits for deduction into n_times or n_times_index
// TODO P5: n_times and n_times_index can be merged based on F accepting a "size_t" or not


namespace libv {
namespace meta {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <typename F, size_t... Indices>
constexpr inline decltype(auto) aux_call_with_n_index(F&& func, std::index_sequence<Indices...>) {
	return func(std::integral_constant<size_t, Indices>{}...);
}

/// Calls the given function with N index at the same time starting from 0 and ending with N-1
/// @usage call_with_n_index\<10>( [] (auto... i) { do_work(i...); } );
template <size_t N, typename F>
constexpr inline decltype(auto) call_with_n_index(F&& func) {
	return aux_call_with_n_index(std::forward<F>(func), std::make_index_sequence<N>{});
}

// -------------------------------------------------------------------------------------------------

template <typename F, size_t Start, size_t... Indices>
constexpr inline void aux_for_constexpr(F&& func, std::index_sequence<Indices...>) {
	((void) func(std::integral_constant<size_t, Indices + Start>{}), ...);
}

/// Calls the given function with index starting from \c Start inclusive to \c End exclusive
/// [\c begin, \c end)
/// @usage for_constexpr\<0, 10>( [] (auto i) { do_work(i); } );
template <size_t Start, size_t End, typename F>
constexpr inline void for_constexpr(F&& func) {
	if constexpr(Start > End)
        static_assert(Start > End, "End has to be greater or equal to Start");
    else
		aux_for_constexpr<F, Start>(std::forward<F>(func), std::make_index_sequence<End - Start>{});
}

} // namespace detail ------------------------------------------------------------------------------

using detail::call_with_n_index;
using detail::for_constexpr;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
