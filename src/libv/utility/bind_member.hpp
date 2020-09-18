// Project: libv.utility, File: src/libv/utility/bind_member.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename R, typename... Args>
constexpr inline auto bind_member(T* obj, R(T::*func)(Args...)) {
	return [obj, func] (auto&&... args) {
		return (obj->*func)(std::forward<decltype(args)>(args)...);
	};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv