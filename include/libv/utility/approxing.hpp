// File: approxing.hpp Author: Vader Created on 2017. augusztus 16., 23:59

#pragma once

// std
#include <cmath>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Epsilon is 10^Precision
/// @param T
/// @param Precision
template <typename T, int Precision = -4>
struct Approxing {
	static constexpr T epsilon = T{std::pow<T>(10, Precision)};

	T value;

	Approxing() = default;
	inline Approxing(const Approxing& orig) = default;
	inline Approxing(Approxing&& orig) = default;

	inline Approxing(T value) :
		value(std::move(value)) { }

	template <typename V>
	friend inline bool operator==(const Approxing& lhs, const V& rhs) {
		return std::abs(lhs.value - rhs) <= epsilon;
	}

	template <typename V>
	friend inline bool operator==(const V& lhs, const Approxing& rhs) {
		return rhs == lhs;
	}

	template <typename V>
	friend inline bool operator>(const Approxing& lhs, const V& rhs) {
		return lhs + epsilon > rhs;
	}

	template <typename V>
	friend inline bool operator>(const V& lhs, const Approxing& rhs) {
		return lhs > rhs - epsilon;
	}

	template <typename V>
	friend inline bool operator<(const Approxing& lhs, const V& rhs) {
		return !(rhs > lhs);
	}

	template <typename V>
	friend inline bool operator<(const V& lhs, const Approxing& rhs) {
		return !(rhs > lhs);
	}

	template <typename OS>
	friend OS& operator<<(OS& os, const Approxing& av) {
		os << av.value << "±" << av.epsilon;
		return os;
	}
};

template <typename T>
Approxing(T) -> Approxing<T>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
