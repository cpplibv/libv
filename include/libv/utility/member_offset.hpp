// File: member_offset.hpp - Created on 2017.09.26. 03:01 - Author: Vader

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
