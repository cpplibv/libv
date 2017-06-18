// File: member.hpp Author: Vader Created on 2017. június 18., 6:53

#pragma once

// std
#include <type_traits>
#include <utility>


namespace libv {
namespace meta {
namespace detail {

// member_wrapper ----------------------------------------------------------------------------------

template <size_t Index, typename T>
struct member_wrapper {
	using value_type = T;

	const char* name;
	T& reference;
};

template <size_t Index, typename T>
auto make_member_wrapper(const char* name, T& reference) {
	return member_wrapper<Index, T>{name, reference};
}

} // namespace detail

// -------------------------------------------------------------------------------------------------

using detail::member_wrapper;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv

#define LIBV_META_MEMBER_ACCESS using __libv_meta_member_list = void;                                \
		template <size_t Index> inline auto __libv_meta_member() const

#define LIBV_META_MEMBER(INDEX, MEMBER)                                                            \
		constexpr bool __libv_index_tag_ ## INDEX = true; (void) __libv_index_tag_ ## INDEX;       \
		constexpr bool __libv_index_tag_ ## MEMBER = true; (void) __libv_index_tag_ ## MEMBER;     \
		if constexpr (INDEX == Index)                                                              \
			return ::libv::meta::detail::make_member_wrapper<INDEX>(#MEMBER, MEMBER)
// TODO P5: I think it is possible to generate the indices on the fly. (If possible, its freaking hard)
