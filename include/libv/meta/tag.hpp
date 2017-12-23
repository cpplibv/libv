// File: tag.hpp Author: Vader Created on 2017. augusztus 3., 21:48

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
