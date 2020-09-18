// Project: libv.meta, File: src/libv/meta/resolve.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

template <typename... Args, typename R>
constexpr inline auto resolve(R fun_ptr(Args...)) -> R(*)(Args...) {
	return fun_ptr;
}

template <typename Sig>
constexpr inline Sig* resolve(Sig* fun_ptr) {
	return fun_ptr;
}

template <typename... Args, typename R, typename C>
constexpr inline auto resolve(R(C::*mem_ptr)(Args...)) -> R(C::*)(Args...) {
	return mem_ptr;
}

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
