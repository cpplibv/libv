// File: always.hpp Author: Vader Created on 2017. június 10., 21:09

#pragma once

#include <utility>


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

template <typename... T>
struct always_true : std::true_type {
	template <typename... Args>
	constexpr inline bool operator()(Args&&...) const {
		return true;
	}
};

template <typename... T>
struct always_false : std::false_type {
	template <typename... Args>
	constexpr inline bool operator()(Args&&...) const {
		return false;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv