// Project: libv.utility, File: src/libv/utility/construct.hpp

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
