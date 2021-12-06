// Project: libv.meta, File: src/libv/meta/type_traits.hpp

#pragma once

// std
#include <array>
#include <type_traits>


namespace libv {
namespace meta {

// --- is_less_comparable --------------------------------------------------------------------------

template <typename L, typename R, typename = void> struct is_less_comparable : std::false_type {
};

template <typename L, typename R>
struct is_less_comparable<L, R, std::void_t<decltype(
		std::declval<const L&>() < std::declval<const R&>()
		)>> : std::true_type { };

template <typename L, typename R>
static constexpr bool is_less_comparable_v = is_less_comparable<L, R>::value;

// --- ostreamable ---------------------------------------------------------------------------------

template <typename L, typename R, typename = void> struct is_ostreamable : std::false_type {
};

template <typename L, typename R>
struct is_ostreamable<L, R, std::void_t<decltype(
		std::declval<L&>() << std::declval<const R&>()
		)>> : std::true_type { };

template <typename L, typename R>
static constexpr bool is_ostreamable_v = is_ostreamable<L, R>::value;

// --- is_same_as_any_v ----------------------------------------------------------------------------

template <typename T, typename K, typename... Args>
static constexpr bool is_same_as_any_v = std::is_same_v<T, K> || (std::is_same_v<T, Args> || ...);

// -------------------------------------------------------------------------------------------------

template <typename T> struct is_array : std::false_type {};
template <typename T, std::size_t N> struct is_array<std::array<T, N>> : std::true_type {};
template <typename T> constexpr bool is_array_v = is_array<T>::value;

template <typename T> struct array_size;
template <typename T, std::size_t N> struct array_size<std::array<T, N>> : std::integral_constant<std::size_t, N> {};
template <typename T> constexpr std::size_t array_size_v = array_size<T>::value;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
