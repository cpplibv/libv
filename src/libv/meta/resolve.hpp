// Project: libv.meta, File: src/libv/meta/resolve.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

// TODO P5: resolve might not support empty list of Args: so to resolve int foo() you have to resolve<int()>(foo) but cant just resolve<>(foo)

/// Template parameter R is deduced
template <typename... Args, typename R>
[[nodiscard]] constexpr inline auto resolve(R fun_ptr(Args...)) -> R(*)(Args...) noexcept {
	return fun_ptr;
}

template <typename Sig>
[[nodiscard]] constexpr inline Sig* resolve(Sig* fun_ptr) noexcept {
	return fun_ptr;
}

/// Template parameter C and R are deduced
template <typename... Args, typename R, typename C>
[[nodiscard]] constexpr inline auto resolve(R(C::*mem_ptr)(Args...)) -> R(C::*)(Args...) noexcept {
	return mem_ptr;
}

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
