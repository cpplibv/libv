// Project: libv.meta, File: src/libv/meta/lnv.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

// TODO P5: Make lnv variadic

/// leftmost-non-void
template <typename Test, typename Fallback>
struct lnv {
	using type = Test;
};

/// leftmost-non-void
template <typename Fallback>
struct lnv<void, Fallback> {
	using type = Fallback;
};

/// leftmost-non-void
template <typename Test, typename Fallback>
using lnv_t = typename lnv<Test, Fallback>::type;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
