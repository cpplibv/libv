//

#pragma once

// std
#include <type_traits>


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

namespace detail {

template<template<typename...> typename Base>
struct aux_derived_from_specialization {
	template <typename... Args>
	static constexpr inline std::true_type test(const Base<Args...>&);
	static constexpr inline std::false_type test(...);
};

} // namespace detail ------------------------------------------------------------------------------

template<typename T, template<typename...> typename Template>
concept derived_from_specialization = requires (const T& t) {
	requires decltype(detail::aux_derived_from_specialization<Template>::test(t))::value;
};

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
