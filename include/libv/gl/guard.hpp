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

// TODO P4: Remove this old guards and use the simple Guard
template <typename Stack>
class StackGuard {
	Stack& stack;
public:
	template <typename T>
	inline explicit StackGuard(Stack& stack, T&& v) : stack(stack) {
		stack.push(std::forward<T>(v));
	}
	inline ~StackGuard() {
		stack.pop();
	}
};

template <typename Target, typename Value>
class EnableGuard {
	Target& target;
	Value value;
public:
	inline EnableGuard(Target& target, Value value) : target(target), value(value) {
		target.enable(value);
	}
	inline ~EnableGuard() {
		target.disable(value);
	}
};

template <typename Target, typename Value>
class DisableGuard {
	Target& target;
	Value value;
public:
	inline DisableGuard(Target& target, Value value) : target(target), value(value) {
		target.disable(value);
	}
	inline ~DisableGuard() {
		target.enable(value);
	}
};

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

// TODO P2: use_guard for use / use(0)
// TODO P2: create_guard for create / destroy

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
