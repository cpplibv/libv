// Project: libv.utility, File: src/libv/utility/to_underlying.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename E>
[[nodiscard]] constexpr inline auto to_underlying(E e) noexcept {
	static_assert(std::is_enum_v<E>, "E has to be an enum type");
	return static_cast<std::underlying_type_t<E>>(e);
}

template <typename E>
[[nodiscard]] constexpr inline auto to_value(E e) noexcept {
	return to_underlying(e);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
