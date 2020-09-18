// Project: libv.utility, File: src/libv/utility/enum.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename E>
[[nodiscard]] constexpr inline auto to_value(E e) noexcept {
	static_assert(std::is_enum_v<E>, "E has to be an enum type");
	return static_cast<std::underlying_type_t<E>>(e);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
