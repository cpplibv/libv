// File: n_times.hpp Author: Vader Created on 2017. június 10., 19:21

#pragma once

#include <utility>


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

template <typename F, size_t... Indices>
constexpr inline void n_times_helper(F&& func, std::index_sequence<Indices...>) {
	((void) func(std::integral_constant<size_t, Indices>{}), ...);
}

template <size_t N, typename F>
constexpr inline void n_times(F&& func) {
	n_times_helper(std::forward<F>(func), std::make_index_sequence<N>{});
}

// -------------------------------------------------------------------------------------------------

template <typename F, size_t... Indices>
constexpr inline decltype(auto) call_with_n_index_helper(F&& func, std::index_sequence<Indices...>) {
	return func(Indices...);
}

template <size_t N, typename F>
constexpr inline decltype(auto) call_with_n_index(F&& func) {
	return call_with_n_index_helper(std::forward<F>(func), std::make_index_sequence<N>{});
}

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
