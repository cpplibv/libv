// File: member_offset.hpp - Created on 2017.09.26. 03:01 - Author: Vader

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename M>
constexpr inline size_t member_offset(M T::* member) {
	return reinterpret_cast<size_t>(&(static_cast<T*>(nullptr)->*member));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
