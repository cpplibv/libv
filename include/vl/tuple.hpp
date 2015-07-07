// File:   tuple.hpp

#pragma once

#include <tuple>
#include <utility>

namespace vl {

// -------------------------------------------------------------------------------------------------

template<typename F, template<typename...> typename Tuple, typename... Args, std::size_t... Is>
inline auto forward_from_tuple_helper(F func, Tuple<Args...>&& tuple, std::index_sequence<Is...>) ->
decltype(func(std::forward<Args>(std::get<Is>(tuple))...)) {
	return func(std::forward<Args>(std::get<Is>(tuple))...);
}

template<typename F, template<typename...> typename Tuple, typename... Args>
auto forward_from_tuple(F&& func, Tuple<Args...>&& tuple) ->
decltype(forward_from_tuple_helper(func, std::forward<Tuple<Args...>>(tuple), std::index_sequence_for<Args...>())) {
	return forward_from_tuple_helper(func, std::forward<Tuple<Args...>>(tuple), std::index_sequence_for<Args...>());
}

// -------------------------------------------------------------------------------------------------

} //namespace vl


//
//no matching function for call to 'forward_from_tuple(
//	TestRA* (*&)(string&&),
//	std::remove_reference<std::tuple<string&&>&>::type)'
// 