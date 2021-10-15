// Project: libv.meta, File: src/libv/meta/identity.hpp

#pragma once


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct identity {
	using type = T;
};

template <typename T>
using identity_t = typename identity<T>::type;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
