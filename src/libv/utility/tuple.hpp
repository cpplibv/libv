// Project: libv.utility, File: src/libv/utility/tuple.hpp, Author: Császár Mátyás [Vader]

#pragma once

#include <tuple>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename F, template <typename...> class Tuple, typename... Args, size_t... Is>
inline auto forward_from_tuple_helper(F func, Tuple<Args...>&& tuple, std::index_sequence<Is...>) ->
decltype(func(std::forward<Args>(std::get<Is>(tuple))...)) {
	return func(std::forward<Args>(std::get<Is>(tuple))...);
}

template <typename F, template <typename...> class Tuple, typename... Args>
inline auto forward_from_tuple(F&& func, Tuple<Args...>&& tuple) ->
decltype(forward_from_tuple_helper(func, std::forward<Tuple<Args...>>(tuple), std::index_sequence_for<Args...>())) {
	return forward_from_tuple_helper(func, std::forward<Tuple<Args...>>(tuple), std::index_sequence_for<Args...>());
}

// -------------------------------------------------------------------------------------------------

template <template <typename...> class Tuple, typename... Args, size_t... Is>
inline auto mask_tuple(const Tuple<Args...>& tuple, std::index_sequence<Is...>) ->
decltype(Tuple<typename std::tuple_element<Is, Tuple<Args...>>::type...>(std::forward<typename std::tuple_element<Is, Tuple<Args...>>::type>(std::get<Is>(tuple))...)) {
	// TODO P5: static assert for over indexing
	return Tuple<typename std::tuple_element<Is, Tuple<Args...>>::type...>(std::forward<typename std::tuple_element<Is, Tuple<Args...>>::type>(std::get<Is>(tuple))...);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv