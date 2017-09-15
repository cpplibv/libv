// File: member.hpp Author: Vader Created on 2017. június 18., 6:53

#pragma once

// libv
#include <libv/meta/derived_level.hpp>
// std
#include <type_traits>


namespace libv {
namespace meta {
namespace detail {

// member_wrapper ----------------------------------------------------------------------------------

template <typename T>
struct member_wrapper {
	using value_type = T;

	const char* name;
	T& reference;
};

template <typename T>
constexpr inline auto make_member_wrapper(const char* name, T& reference) {
	return member_wrapper<T>{name, reference};
}

} // namespace detail

// -------------------------------------------------------------------------------------------------

using detail::member_wrapper;

// ADL Hideout for zero case
derived_level<0> __libv_reflection_next_index(derived_level<0>);

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv

#define LIBV_REFLECTION_ACCESS(NAME) \
		static constexpr ::libv::meta::derived_level<decltype(__libv_reflection_next_index(::libv::meta::derived_top{}))::value + 1> \
			__libv_reflection_next_index(::libv::meta::derived_level<decltype(__libv_reflection_next_index(::libv::meta::derived_top{}))::value + 1>); \
		constexpr inline auto __libv_reflection_member(std::integral_constant<size_t, decltype(__libv_reflection_next_index(::libv::meta::derived_top{}))::value - 1>) { \
			return ::libv::meta::detail::make_member_wrapper(#NAME, NAME); } \
		constexpr inline auto __libv_reflection_member(std::integral_constant<size_t, decltype(__libv_reflection_next_index(::libv::meta::derived_top{}))::value - 1>) const { \
			return ::libv::meta::detail::make_member_wrapper(#NAME, NAME); }

#define LIBV_REFLECTION_EMPTY() \
		static constexpr ::libv::meta::derived_level<0> __libv_reflection_next_index(::libv::meta::derived_level<0>)
