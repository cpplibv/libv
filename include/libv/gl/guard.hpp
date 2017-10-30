// File: guard.hpp

#pragma once

#include <utility>


namespace libv {
namespace gl {

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

template <typename Target>
class BindGuard {
	Target& target;
public:
	inline BindGuard(Target& target) : target(target) {
		target.bind();
	}
	inline ~BindGuard() {
		target.unbind();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
