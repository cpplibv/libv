// File: min_max.hpp - Created on 2017.10.16. 03:45 - Author: Vader

#pragma once

// std
#include <algorithm>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
constexpr inline decltype(auto) max(T&& a) noexcept {
	return std::forward<T>(a);
}
template <typename T, typename ... Args>
constexpr inline decltype(auto) max(T&& a, Args&&... args) noexcept {
	return std::max(std::forward<T>(a), max(std::forward<Args>(args)...));
}

template <typename T>
constexpr inline decltype(auto) min(T&& a) noexcept {
	return std::forward<T>(a);
}
template <typename T, typename ... Args>
constexpr inline decltype(auto) min(T&& a, Args&&... args) noexcept {
	return std::min(min(std::forward<T>(a), std::forward<Args>(args)...));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
