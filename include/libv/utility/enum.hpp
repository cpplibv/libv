// File: enum.hpp - Created on 2017.10.21. 03:23 - Author: Vader

#pragma once

// std
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
constexpr inline auto to_value(E e) noexcept {
	return static_cast<std::underlying_type_t<E>>(e);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
