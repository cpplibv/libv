// File: endian.hpp - Created on 2017.10.21. 06:47 - Author: Vader

#pragma once

// ext
// TODO P5: C++20 replace boost with standard
#include <boost/predef/other/endian.h>
// std
#include <cstdint>


namespace libv {

// -------------------------------------------------------------------------------------------------

constexpr inline bool is_big_endian() {
#ifdef BOOST_BIG_ENDIAN
	return true;
#else
	return false;
#endif
}

constexpr inline bool is_little_endian() {
#ifdef BOOST_LITTLE_ENDIAN
	return true;
#else
	return false;
#endif
}

constexpr inline bool is_network_endian() {
	return is_big_endian();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
