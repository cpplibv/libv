// File: type_traits.hpp

#pragma once

// std
#include <type_traits>


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------
// is_less_comparable

template <typename L, typename R, typename = void> struct is_less_comparable : std::false_type {
};
template <typename L, typename R>
struct is_less_comparable<L, R, std::void_t<decltype(
		std::declval<const L&>() < std::declval<const R&>()
		)>> : std::true_type { };

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv