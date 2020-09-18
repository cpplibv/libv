// Project: libv.utility, File: src/libv/utility/concat.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <string>
#include <sstream>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename... Args>
[[nodiscard]] inline std::string concat(Args&&... args) noexcept {
	// TODO P4: libv.utility: Improve implementation to a real str cat, ostringstream performance is unacceptable
	std::ostringstream ss;
	(ss << ... << std::forward<Args>(args));
	return std::move(ss).str();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
