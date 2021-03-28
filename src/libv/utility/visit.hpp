// Project: libv.utility, File: src/libv/utility/endian.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <libv/utility/overload.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

// - Switch arguments order
// - implicit overload set creation
// - only single variant branching (with possibility to have more on different name: visit2, visit3)
// - Additional version where instead of overload set, use first callable function (visit_with_first)

template <typename Variant, typename Funcs>
constexpr inline auto visit(Variant&& variant, Funcs&& funcs) {
	// instead of std::visit use hand crafted faster version
	return std::visit(std::forward<Funcs>(funcs), std::forward<Variant>(variant));
}

template <typename Variant, typename... Funcs>
constexpr inline auto visit(Variant&& variant, Funcs&&... funcs) {
	// instead of std::visit use hand crafted faster version
	return std::visit(libv::overload(std::forward<Funcs>(funcs)...), std::forward<Variant>(variant));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
