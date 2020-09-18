// File: endian.hpp - Created on 2017.10.21. 06:47 - Author: Vader

#pragma once

// std
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

constexpr inline bool is_big_endian() {
	return std::endian::native == std::endian::big;
}

constexpr inline bool is_little_endian() {
	return std::endian::native == std::endian::little;
}

constexpr inline bool is_network_endian() {
	return is_big_endian();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
