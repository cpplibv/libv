// File: type_traits.hpp

#pragma once

// std
#include <type_traits>

namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename...>
struct voider { //CWG 1558
	using type = void;
};

template <typename... Args>
using void_t = typename voider<Args...>::type;

// -------------------------------------------------------------------------------------------------

template <typename... T>
struct always_true : std::true_type {
};

template <typename... T>
struct always_false : std::false_type {
};

// =================================================================================================

template <typename T>
using disable_if = typename std::enable_if<!T::value>::type;

template <typename T>
using enable_if = typename std::enable_if<T::value>::type;

// =================================================================================================
// is_less_comparable

template <typename L, typename R, typename = void> struct is_less_comparable : std::false_type {
};
template <typename L, typename R>
struct is_less_comparable<L, R, void_t<decltype(
		std::declval<const L&>() < std::declval<const R&>()
		)>> : std::true_type { };

// =================================================================================================
// -------------------------------------------------------------------------------------------------
// is_member_loadable

template <typename T, typename = void> struct is_member_loadable : std::false_type {
};
template <typename T>
struct is_member_loadable<T, void_t<decltype(
		std::declval<T&>().load()
		)>> : std::true_type { };

// -------------------------------------------------------------------------------------------------
// is_member_unloadable

template <typename T, typename = void> struct is_member_unloadable : std::false_type {
};
template <typename T>
struct is_member_unloadable<T, void_t<decltype(
		std::declval<T&>().unload()
		)>> : std::true_type { };

// -------------------------------------------------------------------------------------------------

} //namespace libv