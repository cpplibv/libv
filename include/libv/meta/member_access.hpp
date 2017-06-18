// File: member_access.hpp Author: Vader Created on 2017. június 18., 21:38

#pragma once

// std
#include <type_traits>
#include <utility>
// pro
#include <libv/meta/member.hpp>


namespace libv {
namespace meta {
namespace detail {

// member ------------------------------------------------------------------------------------------

template <typename T, size_t Index>
using member_raw_tpye_t = decltype(std::declval<const T&>().template __libv_meta_member<Index>());

template <typename T> struct member_tpye_indirect { using type = typename T::value_type; };
template <> struct member_tpye_indirect<void> { using type = void; };

template <typename T, size_t Index>
using member_tpye_t = typename member_tpye_indirect<member_raw_tpye_t<T, Index>>::type;

template <typename T, size_t Index = 0>
constexpr inline size_t member_count() {
	if constexpr (std::is_same_v<member_tpye_t<T, Index>, void>)
		return Index;
	else
		return member_count<T, Index + 1>();
}

template <typename T>
constexpr inline size_t member_count_v = member_count<T>();

template <size_t Index, typename T>
inline auto member_info(T& object) {
	static_assert(Index < member_count_v<T>, "Invalid member index used for T object");
	auto omw = object.template __libv_meta_member<Index>();
	return make_member_wrapper<Index>(omw.name,
			const_cast<std::remove_const_t<member_tpye_t<T, Index>>&>(omw.reference));
}

template <size_t Index, typename T>
inline auto member_info(const T& object) {
	static_assert(Index < member_count_v<T>, "Invalid member index used for T object");
	return object.template __libv_meta_member<Index>();
}

template <size_t Index, typename T>
inline decltype(auto) member_value(T&& object) {
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
inline void foreach_member_nvp(T&& object, F&& func) {
	foreach_member(std::forward<T>(object), [&](auto&& member) {
		func(member.name, member.reference);
	});
}

// interleave --------------------------------------------------------------------------------------

template <size_t Index, typename Func, typename... Args>
inline void interleave_member_value_helper2(Func&& func, Args&&... args) {
	func(member_value<Index>(args)...);
}

template <typename Func, typename... Args, size_t... Indices>
inline void interleave_member_value_helper(Func&& func, std::index_sequence<Indices...>, Args&&... args) {
	(interleave_member_value_helper2<Indices>(func, args...), ...);
}

template <typename Func, typename Head, typename... Tail>
inline void interleave_member_value(Func&& func, Head&& head, Tail&&... tail) {
	static_assert(((member_count_v<Head> == member_count_v<Tail>) && ...), "Different number of members in interleaved objects");
	interleave_member_value_helper(func, std::make_index_sequence<member_count_v<Head>>{}, head, tail...);
}

} // namespace detail

// -------------------------------------------------------------------------------------------------

using detail::member_count_v;
using detail::member_info;
using detail::member_tpye_t;
using detail::member_value;
using detail::foreach_member;
using detail::foreach_member_nvp;
using detail::interleave_member_value;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
