// File: type_traits.hpp

#pragma once

// std
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
using disable_if = typename std::enable_if<!T::value>::type;

template <typename T>
using enable_if = typename std::enable_if<T::value>::type;

// =================================================================================================
// is_less_comparable

template <typename L, typename R, typename = void> struct is_less_comparable : std::false_type {
};
template <typename L, typename R>
struct is_less_comparable<L, R, std::void_t<decltype(
		std::declval<const L&>() < std::declval<const R&>()
		)>> : std::true_type { };

// -------------------------------------------------------------------------------------------------

} // namespace libv