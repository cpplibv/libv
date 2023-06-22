// Project: libv.meta, File: src/libv/meta/for_constexpr.hpp

#pragma once

// std
#include <cstddef> // consider changing std::size_t usage to int or T in this file
#include <utility>
// pro
#include <libv/meta/force_inline.hpp>


namespace libv {
namespace meta {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <typename F, std::size_t... Indices>
constexpr LIBV_FORCE_INLINE decltype(auto) aux_call_with_n_index(F&& func, std::index_sequence<Indices...>) {
	return func(std::integral_constant<std::size_t, Indices>{}...);
}

/// Calls the given function with N index at the same time starting from 0 and ending with N-1
/// @usage call_with_n_index\<10>( [] (auto... i) { do_work(i...); } );
template <std::size_t N, typename F>
constexpr LIBV_FORCE_INLINE decltype(auto) call_with_n_index(F&& func) {
	return aux_call_with_n_index(std::forward<F>(func), std::make_index_sequence<N>{});
}

// -------------------------------------------------------------------------------------------------

template <typename F, std::size_t Start, std::size_t... Indices>
constexpr LIBV_FORCE_INLINE void aux_for_constexpr(F&& func, std::index_sequence<Indices...>) {
	((void) func(std::integral_constant<std::size_t, Indices + Start>{}), ...);
}

/// Calls the given function with index starting from \c Start inclusive to \c End exclusive
/// [\c begin, \c end)
/// @usage for_constexpr\<0, 10>( [] (auto i) { do_work(i); } );
template <std::size_t Start, std::size_t End, typename F>
constexpr LIBV_FORCE_INLINE void for_constexpr(F&& func) {
	static_assert(Start <= End, "End has to be greater or equal to Start");
	aux_for_constexpr<F, Start>(std::forward<F>(func), std::make_index_sequence<End - Start>{});
}

} // namespace detail ------------------------------------------------------------------------------

using detail::call_with_n_index;
using detail::for_constexpr;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
