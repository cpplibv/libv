// File: n_times.hpp Author: Vader Created on 2017. június 10., 19:21

#pragma once

#include <utility>


namespace libv {
namespace meta {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <typename F, size_t... Indices>
constexpr inline decltype(auto) call_with_n_index_helper(F&& func, std::index_sequence<Indices...>) {
	return func(std::integral_constant<size_t, Indices>{}...);
}

template <size_t N, typename F>
constexpr inline decltype(auto) call_with_n_index(F&& func) {
	return call_with_n_index_helper(std::forward<F>(func), std::make_index_sequence<N>{});
}

// -------------------------------------------------------------------------------------------------

template <typename F, size_t... Indices>
constexpr inline void n_times_index_helper(F&& func, std::index_sequence<Indices...>) {
	((void) func(std::integral_constant<size_t, Indices>{}), ...);
}

template <size_t N, typename F>
constexpr inline void n_times(F&& func) {
	n_times_index_helper([&](auto) { func(); }, std::make_index_sequence<N>{});
}

// TODO P5: n_times and n_times_index can be merged based on F accepting a "size_t" or not
template <size_t N, typename F>
constexpr inline void n_times_index(F&& func) {
	n_times_index_helper(std::forward<F>(func), std::make_index_sequence<N>{});
}

} // namespace detail ------------------------------------------------------------------------------

using detail::call_with_n_index;
using detail::n_times;
using detail::n_times_index;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
