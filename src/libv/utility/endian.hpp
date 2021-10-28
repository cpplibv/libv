// Project: libv.utility, File: src/libv/utility/endian.hpp

#pragma once

// std
#include <bit>


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
