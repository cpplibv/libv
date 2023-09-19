// Project: libv.re, File: src/libv/re/uniform/concept.hpp

#pragma once

#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/type_traits.hpp>
#include <libv/re/common/block_binding.hpp>

#include <concepts>
#include <string_view>


namespace libv {
namespace re {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <typename T> struct is_vec234 : std::false_type {};
template <std::size_t N, typename T>
	requires (2 <= N && N <= 4)
struct is_vec234<libv::vec_t<N, T>> : std::true_type {};
template <typename T> constexpr bool is_vec234_v = is_vec234<T>::value;

template <typename T> struct is_mat234 : std::false_type {};
template <std::size_t R, std::size_t C, typename T>
	requires (2 <= R && R <= 4 && 2 <= C && C <= 4)
struct is_mat234<libv::mat_t<R, C, T>> : std::true_type {};
template <typename T> constexpr bool is_mat234_v = is_mat234<T>::value;

template <typename T>
concept has_access_uniforms_fn = requires (T s, int& fn) { s.access_uniforms(fn); };

template <typename T>
concept is_glsl_scalar = libv::meta::is_same_as_any_v<T, bool, int32_t, uint32_t, int64_t, uint64_t, float, double>;

template <typename T>
concept is_glsl_mat_scalar = libv::meta::is_same_as_any_v<T, float, double>;

} // namespace detail ------------------------------------------------------------------------------

template <typename T>
concept is_uniform_padding = requires { typename T::is_padding; };

template <typename T>
concept is_uniform_primitive =
		detail::is_glsl_scalar<T> ||
		std::is_enum_v<T> && detail::is_glsl_scalar<std::underlying_type_t<T>> ||
		detail::is_vec234_v<T> && detail::is_glsl_scalar<typename T::value_type> ||
		detail::is_mat234_v<T> && detail::is_glsl_mat_scalar<typename T::value_type>;

template <typename T>
concept is_uniform_wrapper = requires { typename T::uniform_wrapper; };

template <typename T>
concept is_uniform_block_wrapper = requires { typename T::uniform_block_wrapper; };

template <typename T>
concept is_uniform_structure = detail::has_access_uniforms_fn<T>;

template <typename T>
concept is_uniform_array = libv::meta::is_array_v<T>;

template <typename T>
concept is_uniform_block = is_uniform_structure<T> && requires {
	{T::name} -> std::convertible_to<std::string_view>;
	{T::binding} -> std::convertible_to<BlockBinding>;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
