// File: hex_dump.hpp Author: Vader Created on 2017. május 14., 20:15

#pragma once

// std
#include <iomanip>
#include <sstream>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P5: Would be nice to append an extra column for ascii chars like:
//			B9 B5 C5 84 C5 A5 20 E1  B9 B7 C5 A7 20 E1 B8 B9 | red apple

inline std::string hex_dump(const std::string_view& s) {
	std::ostringstream ss;
	ss << std::hex << std::setfill('0') << std::setw(2) << std::uppercase;
	for (size_t i = 0; i < s.size(); ++i) {
		if (i % 32 == 0)
			ss << "\n ";
		else if (i % 8 == 0)
			ss << "  ";
		else
			ss << ' ';
		ss << std::setw(2) << uint16_t{static_cast<unsigned char>(s[i])};
	}

	return ss.str();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
