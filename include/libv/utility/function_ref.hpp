// File:   function_ref.hpp Author: Vader Created on 30 August 2019, 00:10

#pragma once

// std
#include <functional>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// A lightweight non-owning reference to a callable.
template <typename F>
class function_ref;

/// A lightweight non-owning reference to a callable. Specialization for function signature.
template <typename R, typename... Args>
class function_ref<R(Args...)> {
private:
	void* obj_ = nullptr;
	R(*callback_)(void*, Args...) = nullptr;

public:
	constexpr inline function_ref() noexcept = delete;
	constexpr inline function_ref(const function_ref<R(Args...)>& rhs) noexcept = default;

	template <typename F,
			typename = std::enable_if_t<
				!std::is_same<std::decay_t<F>, function_ref>::value &&
				std::is_invocable_r<R, F &&, Args...>::value
			>
	>

	constexpr inline function_ref(F &&f) noexcept :
		obj_(const_cast<void*> (reinterpret_cast<const void*> (std::addressof(f)))) {
		callback_ = [](void* obj, Args... args) -> R {
			return std::invoke(
					*reinterpret_cast<typename std::add_pointer<F>::type> (obj),
					std::forward<Args>(args)...);
		};
	}

	constexpr inline function_ref<R(Args...)>&	operator=(const function_ref<R(Args...)> &rhs) noexcept = default;

	template <typename F,
			typename = std::enable_if_t<std::is_invocable_r<R, F &&, Args...>::value>>
	constexpr inline function_ref<R(Args...)>& operator=(F &&f) noexcept {
		obj_ = reinterpret_cast<void*>(std::addressof(f));
		callback_ = [](void* obj, Args... args) {
			return std::invoke(
					*reinterpret_cast<typename std::add_pointer<F>::type> (obj),
					std::forward<Args>(args)...);
		};

		return *this;
	}

	constexpr inline void swap(function_ref<R(Args...)> &rhs) noexcept {
		std::swap(obj_, rhs.obj_);
		std::swap(callback_, rhs.callback_);
	}

	inline R operator()(Args... args) const {
		return callback_(obj_, std::forward<Args>(args)...);
	}
};

template <typename R, typename... Args>
constexpr inline void swap(function_ref<R(Args...)> &lhs, function_ref<R(Args...)> &rhs) noexcept {
	lhs.swap(rhs);
}

template <typename R, typename... Args>
function_ref(R(*)(Args...)) -> function_ref<R(Args...)>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
