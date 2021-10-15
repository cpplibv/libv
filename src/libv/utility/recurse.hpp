// Project: libv.utility, File: src/libv/utility/recurse.hpp

// std
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// higher-order function (Y-combinator)
/// \usage
/// 	auto f = recurse{[](auto&& self, auto n) -> int { return n == 0 ? 0 : n + self(n - 1); }};
/// 	f(100);
template <typename F>
struct recurse : F {
	template <typename... Args>
	decltype(auto) operator()(Args&& ...args) const {
		return F::operator()(*this, std::forward<Args>(args)...);
	}
};

template <typename F>
recurse(F) -> recurse<F>; // Deduction Guide

// -------------------------------------------------------------------------------------------------

} // namespace libv
