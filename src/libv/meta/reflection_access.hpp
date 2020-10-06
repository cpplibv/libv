// Project: libv.meta, File: src/libv/meta/reflection_access.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/meta/derived_level.hpp>
// std
#include <type_traits>


namespace libv {
namespace meta {
namespace detail {

// member_wrapper ----------------------------------------------------------------------------------

template <typename T, typename M>
struct member_wrapper {
	using self_type = T;
	using value_type = M;

	const char* name;
	M& reference;
	M T::* member_pointer;
};

template <typename T, typename M, typename M2>
constexpr inline auto make_member_wrapper(const char* name, M& reference, M2 T::* member) noexcept {
	return member_wrapper<T, M>{name, reference, member};
}

template <typename T, typename M>
struct static_wrapper {
	using self_type = T;
	using value_type = M;

	const char* name;
	M T::* member_pointer;
};

template <typename T, typename M>
constexpr inline auto make_static_wrapper(const char* name, M T::* member) noexcept {
	return static_wrapper<T, M>{name, member};
}

} // namespace detail

// -------------------------------------------------------------------------------------------------

using detail::member_wrapper;
using detail::static_wrapper;

// ADL Hideout for zero case
derived_level<0> __libv_reflection_next_index(derived_level<0>);

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv


#define LIBV_REFLECTION_ACCESS(NAME) \
        static constexpr ::libv::meta::derived_level<decltype(__libv_reflection_next_index(::libv::meta::derived_top{}))::value + 1> \
            __libv_reflection_next_index(::libv::meta::derived_level<decltype(__libv_reflection_next_index(::libv::meta::derived_top{}))::value + 1>); \
        template <typename Self> static constexpr auto __libv_reflection_static(std::integral_constant<size_t, decltype(__libv_reflection_next_index(::libv::meta::derived_top{}))::value - 1>) noexcept { \
            return ::libv::meta::detail::make_static_wrapper(#NAME, &Self:: NAME ); } \
        inline auto __libv_reflection_member(std::integral_constant<size_t, decltype(__libv_reflection_next_index(::libv::meta::derived_top{}))::value - 1>) noexcept { \
            return ::libv::meta::detail::make_member_wrapper(#NAME, NAME, &decltype(std::remove_reference_t<decltype(*this)>{}):: NAME ); } \
        inline auto __libv_reflection_member(std::integral_constant<size_t, decltype(__libv_reflection_next_index(::libv::meta::derived_top{}))::value - 1>) const noexcept { \
            return ::libv::meta::detail::make_member_wrapper(#NAME, NAME, &decltype(std::remove_reference_t<decltype(*this)>{}):: NAME ); }

//#define LIBV_REFLECTION_FAST(ID, NAME) \
//		template <typename Self> static constexpr auto __libv_reflection_static(std::integral_constant<size_t, ID>) noexcept { \
//			return ::libv::meta::detail::make_static_wrapper(#NAME, &Self:: NAME ); } \
//		inline auto __libv_reflection_member(std::integral_constant<size_t, ID>) noexcept { \
//			return ::libv::meta::detail::make_member_wrapper(#NAME, NAME, &decltype(std::remove_reference_t<decltype(*this)>{}):: NAME ); } \
//		inline auto __libv_reflection_member(std::integral_constant<size_t, ID>) const noexcept { \
//			return ::libv::meta::detail::make_member_wrapper(#NAME, NAME, &decltype(std::remove_reference_t<decltype(*this)>{}):: NAME ); }

#define LIBV_REFLECTION_EMPTY() \
        static constexpr ::libv::meta::derived_level<0> __libv_reflection_next_index(::libv::meta::derived_level<0>)
