// File: type_traits.hpp

#pragma once

// vl
#include <vl/efl.hpp>
// std
#include <type_traits>

namespace vl {

// -------------------------------------------------------------------------------------------------

template <typename...>
struct voider { //CWG 1558
	using type = void;
};

template <typename... Args>
using void_t = typename voider<Args...>::type;

// =================================================================================================

template <typename T>
using disable_if = typename std::enable_if<!T::value>::type;

template <typename T>
using enable_if = typename std::enable_if<T::value>::type;

// =================================================================================================

template <typename T, typename L, typename = void>
struct is_less_comparable : std::false_type {
};
template <typename T, typename L>
struct is_less_comparable<T, L, void_t<decltype(
		std::declval<const T&>() < std::declval<const L&>()
		)>> : std::true_type { };

// =================================================================================================

template <typename T, typename = void>
struct is_efl_loadable : std::false_type {
};
template <typename T>
struct is_efl_loadable<T, void_t<decltype(
		::vl::efl::load(std::declval<T&>())
		)>> : std::true_type { };

// -------------------------------------------------------------------------------------------------

template <typename T, typename = void>
struct is_efl_unloadable : std::false_type {
};
template <typename T>
struct is_efl_unloadable<T, void_t<decltype(
		::vl::efl::unload(std::declval<T&>())
		)>> : std::true_type { };

// =================================================================================================

template <typename T, typename = void>
struct is_member_loadable : std::false_type {
};
template <typename T>
struct is_member_loadable<T, void_t<decltype(
		std::declval<T&>().load()
		)>> : std::true_type { };

// -------------------------------------------------------------------------------------------------

template <typename T, typename = void>
struct is_member_unloadable : std::false_type {
};
template <typename T>
struct is_member_unloadable<T, void_t<decltype(
		std::declval<T&>().unload()
		)>> : std::true_type { };

// -------------------------------------------------------------------------------------------------

} //namespace vl