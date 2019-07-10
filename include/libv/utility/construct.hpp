// File:   construct.hpp Author: Vader Created on 16 August 2019, 23:07

#pragma once

// std
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename... Args>
constexpr inline T construct(Args&&... args) {
	return T{std::forward<Args>(args)...};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
