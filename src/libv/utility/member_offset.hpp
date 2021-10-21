// Project: libv.utility, File: src/libv/utility/member_offset.hpp

#pragma once

// std
#include <bit>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename M>
[[nodiscard]] constexpr inline size_t member_offset(M T::* member) noexcept {
	return std::bit_cast<size_t>(&(static_cast<T*>(nullptr)->*member));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
