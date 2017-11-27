// File: guard.hpp

#pragma once

// std
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename F>
class Guard {
	F action;
public:
	Guard() = delete;
	Guard(const Guard&) = delete;
	Guard(Guard&&) = delete;
	Guard& operator=(const Guard&) = delete;
	Guard& operator=(Guard&&) = delete;

	template <typename CF>
	inline explicit Guard(CF&& func) : action(std::forward<CF>(func)) {}

	inline ~Guard() {
		action();
	}
};

template <typename CF> Guard(CF&&) -> Guard<std::decay_t<CF>>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
