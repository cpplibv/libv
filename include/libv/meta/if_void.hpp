// File:   if_void.hpp Author: Vader Created on 31 May 2019, 22:38

#pragma once


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

template <typename Test, typename Fallback>
struct if_void {
	using type = Test;
};

template <typename Fallback>
struct if_void<void, Fallback> {
	using type = Fallback;
};

template <typename Test, typename Fallback>
using if_void_t = typename if_void<Test, Fallback>::type;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
