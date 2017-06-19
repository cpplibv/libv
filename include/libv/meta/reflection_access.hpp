// File: member_access.hpp Author: Vader Created on 2017. június 18., 21:38

#pragma once

// std
#include <type_traits>
#include <utility>
// pro
#include <libv/meta/reflection.hpp>


namespace libv {
namespace meta {
namespace detail {

// member ------------------------------------------------------------------------------------------

template <typename T>
constexpr inline size_t member_count() {
	return decltype(std::decay_t<T>::__libv_reflection_next_index(derived_top{}))::value;
}

template <typename T>
constexpr inline size_t member_count_v = member_count<T>();

template <size_t Index, typename T>
inline auto member_info(T&& object) {
	static_assert(Index < member_count_v<T>, "Invalid member index used for T object");
	return object.__libv_reflection_member(std::integral_constant<size_t, Index>{});
}

template <typename T, size_t Index>
using member_tpye_t = typename decltype(member_info<Index, T>(std::declval<T&>()))::value_type;

template <size_t Index, typename T>
inline decltype(auto) member_reference(T&& object) {
	return member_info<Index>(object).reference;
}

// foreach -----------------------------------------------------------------------------------------

template <typename T, typename F, size_t... Indices>
inline void foreach_member_helper(T&& object, F&& func, std::index_sequence<Indices...>) {
	(func(member_info<Indices>(object)), ...);
}

template <typename T, typename F>
inline void foreach_member(T&& object, F&& func) {
	foreach_member_helper(std::forward<T>(object), std::forward<F>(func), std::make_index_sequence<member_count_v<T>>{});
}

template <typename T, typename F>
inline void foreach_member_nrp(T&& object, F&& func) {
	foreach_member(std::forward<T>(object), [&](auto&& member) {
		func(member.name, member.reference);
	});
}

// interleave --------------------------------------------------------------------------------------

template <size_t Index, typename Func, typename... Args>
inline void interleave_member_value_helper2(Func&& func, Args&&... args) {
	func(member_reference<Index>(args)...);
}

template <typename Func, typename... Args, size_t... Indices>
inline void interleave_member_value_helper(Func&& func, std::index_sequence<Indices...>, Args&&... args) {
	(interleave_member_value_helper2<Indices>(func, args...), ...);
}

template <typename Func, typename Head, typename... Tail>
inline void interleave_member_reference(Func&& func, Head&& head, Tail&&... tail) {
	static_assert(((member_count_v<Head> == member_count_v<Tail>) && ...), "Different number of members in interleaved objects");
	interleave_member_value_helper(func, std::make_index_sequence<member_count_v<Head>>{}, head, tail...);
}

} // namespace detail

// -------------------------------------------------------------------------------------------------

using detail::member_count_v;
using detail::member_info;
using detail::member_reference;
using detail::member_tpye_t;
using detail::foreach_member;
using detail::foreach_member_nrp;
using detail::interleave_member_reference;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
