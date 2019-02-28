// File:   concat.hpp Author: Vader Created on 2019. 02. 22., 19:03

#pragma once

// std
#include <string>
#include <sstream>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename... Args>
[[nodiscard]] inline std::string concat(Args&&... args) noexcept {
	std::ostringstream ss;
	(ss << ... << std::forward<Args>(args));
	return std::move(ss).str();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
