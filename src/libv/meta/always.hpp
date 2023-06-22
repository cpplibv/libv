// Project: libv.meta, File: src/libv/meta/always.hpp

#pragma once

// std
#include <utility>


namespace libv {
namespace meta {

// -------------------------------------------------------------------------------------------------

template <typename... T>
struct always_true : std::true_type {
	template <typename... Args>
	static constexpr inline bool operator()(Args&&...) const {
		return true;
	}
};

template <typename...>
static constexpr bool always_true_v = true;

template <typename... T>
struct always_false : std::false_type {
	template <typename... Args>
	static constexpr inline bool operator()(Args&&...) const {
		return false;
	}
};

template <typename...>
static constexpr bool always_false_v = false;

// -------------------------------------------------------------------------------------------------

} // namespace meta
} // namespace libv
