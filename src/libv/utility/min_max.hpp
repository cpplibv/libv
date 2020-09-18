// Project: libv.utility, File: src/libv/utility/min_max.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

namespace detail {

template <typename T, typename... Args>
constexpr inline auto max(const T& lhs, const Args&... args) noexcept {
	if constexpr (sizeof...(args) == 0) {
		return lhs;
	} else {
		auto rhs = max(args...);
		return lhs > rhs ? lhs : rhs;
	}
}

template <typename T, typename... Args>
constexpr inline auto min(const T& lhs, const Args&... args) noexcept {
	if constexpr (sizeof...(args) == 0) {
		return lhs;
	} else {
		auto rhs = min(args...);
		return lhs < rhs ? lhs : rhs;
	}
}

} // namespace detail

template <typename... Args>
constexpr inline auto max(const Args&... args) noexcept {
	return detail::max(std::common_type_t<Args...>{args}...);
}

template <typename... Args>
constexpr inline auto min(const Args&... args) noexcept {
	return detail::min(std::common_type_t<Args...>{args}...);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
