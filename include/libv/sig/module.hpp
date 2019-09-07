// File: module.hpp, Created on 2014. augusztus 9. 16:07, Author: Vader

#pragma once

// std
#include <functional>


namespace libv {
namespace sig {

// -------------------------------------------------------------------------------------------------

// --- Accumulator ---
//template <typename T>
//struct _accumulator_traits_add_helper {
//	template <typename A, typename F, typename... Args>
//	static inline bool add(A& acc, F&& func, Args&&... args) {
//		return acc.add(func(std::forward<Args>(args)...));
//	}
//};
//
//template <>
//struct _accumulator_traits_add_helper<void> {
//	template <typename A, typename F, typename... Args>
//	static inline bool add(A& acc, F&& func, Args&&... args) {
//		func(std::forward<Args>(args)...);
//		return acc.add();
//	}
//};
//
//template <typename Accumulator>
//struct accumulator_traits {
//};

// -------------------------------------------------------------------------------------------------

template <typename T, typename Operation = std::plus<>>
struct Accumulator {
	using result_type = decltype(std::declval<Accumulator>().result());

	static inline Accumulator create() {
		return Accumulator();
	}

	template <typename R, typename... FArgs, typename... Args>
	static inline bool add(Accumulator& acc, std::function<R(FArgs...)>& func, Args&&... args) {
		if constexpr (std::is_void_v<R>)
			;
//			return _accumulator_traits_add_helper<R>::add(acc, func, std::forward<Args>(args)...);
		else
			;
//			return _accumulator_traits_add_helper<R>::add(acc, func, std::forward<Args>(args)...);
	}

//	static inline result_type result(Accumulator& acc) {
//		return acc.result();
//	}

	static constexpr T operator()(const T& lhs, const T& rhs) {
		return T{};
	}
};

//template <typename T>
//struct AccumulatorSum {
//	T value{};
//
//	inline T result() const {
//		return value;
//	}
//	inline bool add(const T& value) {
//		value += value;
//		return false;
//	}
//};
//
//template <>
//struct AccumulatorSum<void> {
//};
//
//template <typename T>
//struct AccumulatorLast {
//	T value{};
//
//	inline T result() const {
//		return value;
//	}
//	inline bool add(const T& value) {
//		value = value;
//		return false;
//	}
//};
//
//template <>
//struct AccumulatorLast<void> {
//};
//
//template <typename T>
//struct AccumulatorAnd {
//	T value{true};
//
//	inline T result() const {
//		return value;
//	}
//	inline bool add(const T& value) {
//		value = value && value;
//		return !value;
//	}
//};
//
//template <>
//struct AccumulatorAnd<void> {
//};
//
//template <typename T>
//struct AccumulatorOr {
//	T value{false};
//
//	inline T result() const {
//		return value;
//	}
//	inline bool add(const T& value) {
//		value = value || value;
//		return value;
//	}
//};
//
//template <>
//struct AccumulatorOr<void> {
//};

// --- History Size ---
template <size_t N>
struct history_size {
	using module = tag_type<tag::history_size>;
	static constexpr size_t value = N;
};

// -------------------------------------------------------------------------------------------------

} // namespace sig
} //namespace libv
