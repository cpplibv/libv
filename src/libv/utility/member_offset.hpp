// Project: libv.utility, File: src/libv/utility/member_offset.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/bit_cast.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename M>
[[nodiscard]] constexpr inline size_t member_offset(M T::* member) noexcept {
	return libv::bit_cast<size_t>(&(static_cast<T*>(nullptr)->*member));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
