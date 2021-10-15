// Project: libv.meta, File: src/libv/meta/index_to_type.hpp

#pragma once

// std
#include <utility>


namespace libv {
namespace meta {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <size_t I, typename T>
struct indexed {
	static constexpr size_t index = I;
	using type = T;
};

template <typename Is, typename... Ts>
struct indexer;

template <size_t... Is, typename... Ts>
struct indexer<std::index_sequence<Is...>, Ts...> : indexed<Is, Ts>... {};

template <size_t I, typename T>
static indexed<I, T> select(indexed<I, T>);

//

template <typename...>
struct to_index;

template <typename I, typename... Types>
struct to_index<I, I, Types...> {
    static constexpr std::size_t value = 0;
};

template <typename I, typename U, typename... Types>
struct to_index<I, U, Types...> {
    static constexpr std::size_t value = 1 + to_index<I, Types...>::value;
};

} // namespace detail

template <size_t I, typename... Ts>
using index_to_type_t = typename decltype(detail::select<I>(detail::indexer<std::index_sequence_for<Ts...>, Ts...>{}))::type;

template <typename I, typename... Ts>
constexpr inline size_t type_to_index_v = detail::to_index<I, Ts...>::value;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
