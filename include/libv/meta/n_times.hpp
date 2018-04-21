// File: n_times.hpp Author: Vader Created on 2017. június 10., 19:21

#pragma once

#include <utility>

// TODO P4: Rename this file to for_constexpr.hpp, for.hpp, loop.hpp or cycle.hpp


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

/// Calls the given function with index starting from START to END including END
/// @usage for_constexpr\<0, 10>( [] (auto i) { do_work(i); } );
template <size_t Start, size_t End, typename F>
constexpr inline void for_constexpr(F&& func) {
	if constexpr(Start > End)
        static_assert(Start > End, "End has to be greater or equal to Start");
    else
		aux_for_constexpr<F, Start>(std::forward<F>(func), std::make_index_sequence<End - Start + 1>{});
}

// -------------------------------------------------------------------------------------------------

// TODO P5: Take a look at boost::callable_traits for deduction into n_times or n_times_index

/// Calls the given function N times
/// @usage n_times\<10>( [] { do_work(i); } );
template <size_t N, typename F>
constexpr inline void n_times(F&& func) {
	for_constexpr<0, N - 1>([&](auto) { func(); });
}

// TODO P5: n_times and n_times_index can be merged based on F accepting a "size_t" or not
/// Calls the given function N times with indexes starting from 0 and ending with N-1
/// @usage n_times_index\<10>( [] (auto i) { do_work(i); } );
template <size_t N, typename F>
constexpr inline void n_times_index(F&& func) {
	for_constexpr<0, N - 1>(std::forward<F>(func));
}

} // namespace detail ------------------------------------------------------------------------------

using detail::call_with_n_index;
using detail::for_constexpr;
using detail::n_times;
using detail::n_times_index;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
