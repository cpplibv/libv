// File: bind_lite.hpp Author: Vader Created on 2017. május 7., 14:01

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