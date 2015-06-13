// File: type_traits.hpp

#pragma once

#include <type_traits>

namespace vl {

// -------------------------------------------------------------------------------------------------

template <typename...>
struct voider { //CWG 1558
	using type = void;
};

template <typename... Args>
using void_t = typename voider<Args...>::type;

// -------------------------------------------------------------------------------------------------

template <typename T>
using disable_if = typename std::enable_if<!T::value>::type;

template <typename T>
using enable_if = typename std::enable_if<T::value>::type;

// -------------------------------------------------------------------------------------------------

template <typename T, typename L>
using less_comparable_result_t = decltype(std::declval<const T&>() < std::declval<const L&>());

template <typename T, typename L, typename = void>
struct is_less_comparable : std::false_type {};

template <typename T, typename L>
struct is_less_comparable<T, L, void_t<vl::less_comparable_result_t<T, L>>> : std::true_type {};

//template <typename T, typename K>
//using is_less_comparable_t = typename is_less_comparable<T, K>::type;

// -------------------------------------------------------------------------------------------------

} //namespace vl