// Project: libv.meta, File: src/libv/meta/to_tuple.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/preprocessor/repetition/repeat.hpp>
// std
#include <cassert>
#include <tuple>
#include <type_traits>


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

struct any_type {
	template <typename T> constexpr operator T();
};

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wmissing-field-initializers"
template <typename T, size_t... I>
decltype(void(T{(I, std::declval<any_type>())...}), std::true_type{})
test_is_braces_constructible_n(std::index_sequence<I...>);
//#pragma clang diagnostic pop

template <typename, typename...>
std::false_type test_is_braces_constructible_n(...);

template <typename T, size_t N>
using is_braces_constructible_n = decltype(test_is_braces_constructible_n<T>(std::make_index_sequence<N>{}));

template <typename T, size_t L = 0u, size_t R = sizeof (T) + 1u >
inline constexpr size_t to_tuple_size_f() {
	constexpr size_t M = (L + R) / 2u;
	if constexpr (M == 0)
		return std::is_empty<T>{} ? 0u : throw std::runtime_error("Unable to determine number of elements");
	else if constexpr (L == M)
		return M;
	else if constexpr (is_braces_constructible_n<T, M>{})
		return to_tuple_size_f<T, M, R>();
	else
		return to_tuple_size_f<T, L, M>();
}

template <typename T>
using to_tuple_size = std::integral_constant<size_t, to_tuple_size_f<T>()>;

#ifndef LIBV_TO_TUPLE_MAX
#    define LIBV_TO_TUPLE_MAX 32
#endif

template <typename T>
inline auto to_tuple_impl(T&&, std::integral_constant<size_t, 0>) noexcept {
	return std::make_tuple();
}

#define LIBV_TO_TUPLE_P(Z,N,_) , p ## N
#define LIBV_TO_TUPLE_SPECIALIZATION(Z,N,_)                                                        \
template <typename T>                                                                              \
inline auto to_tuple_impl(T&& object, std::integral_constant<size_t, N + 1>) noexcept {            \
		auto&& [p BOOST_PP_REPEAT_ ## Z(N, LIBV_TO_TUPLE_P, nil)] = object;                        \
		return std::make_tuple(p BOOST_PP_REPEAT_ ## Z(N, LIBV_TO_TUPLE_P, nil));                  \
}
BOOST_PP_REPEAT(LIBV_TO_TUPLE_MAX, LIBV_TO_TUPLE_SPECIALIZATION, nil)

#undef LIBV_TO_TUPLE_SPECIALIZATION
#undef LIBV_TO_TUPLE_P

// Workaround end
// =================================================================================================

template <typename T, typename = struct current_value, size_t = LIBV_TO_TUPLE_MAX, typename = struct required_value, size_t N>
inline auto to_tuple_impl(T&&, std::integral_constant<size_t, N>) noexcept {
	static_assert(N <= LIBV_TO_TUPLE_MAX, "Please increase LIBV_TO_TUPLE_MAX");
}

//template <typename T, typename = std::enable_if_t<std::is_class<std::decay_t<T>>::value && std::is_standard_layout<std::decay_t<T>>::value>>
template <typename T>
auto to_tuple(T&& object) noexcept {
    return to_tuple_impl(std::forward<T>(object), to_tuple_size<std::decay_t<T>>{});
}

// -------------------------------------------------------------------------------------------------

template <typename T>
struct to_tuple_type {
	using type = decltype(to_tuple(std::declval<T>()));
};

template <typename T>
using to_tuple_type_t = typename to_tuple_type<T>::type;

// -------------------------------------------------------------------------------------------------

template <class T, class Tuple, std::size_t... I>
constexpr T make_from_tuple_impl(Tuple&& t, std::index_sequence<I...>) {
	return T{std::get<I>(std::forward<Tuple>(t))...};
}

template <class T, class Tuple>
constexpr T make_from_tuple_using_braces(Tuple&& t) {
	return make_from_tuple_impl<T>(std::forward<Tuple>(t),
			std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{}
	);
}

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
