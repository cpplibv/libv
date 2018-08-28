// File:   nth_element.hpp Author: Vader Created on 2018. augusztus 28., 20:09

#pragma once

// std
#include <utility>


namespace libv {
namespace meta {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <size_t I, typename T>
struct indexed {
	using type = T;
};

template <typename Is, typename... Ts>
struct indexer;

template <size_t... Is, typename... Ts>
struct indexer<std::index_sequence<Is...>, Ts...> : indexed<Is, Ts>... {};

template <size_t I, typename T>
static indexed<I, T> select(indexed<I, T>);

} // namespace detail

template <size_t I, typename... Ts>
using nth_element_t = typename decltype(detail::select<I>(detail::indexer<std::index_sequence_for<Ts...>, Ts...>{}))::type;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
