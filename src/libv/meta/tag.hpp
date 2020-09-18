// Project: libv.meta, File: src/libv/meta/tag.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

template <auto...> struct Ignore {};
template <auto...> struct Tag {};
template <auto...> struct Overload {};
template <auto...> struct ArtificialOverload {};

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
