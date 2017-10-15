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
	constexpr static const T epsilon = std::pow<T>(10, Precision);

	T value;

	Approxing() = default;
	template <typename TC> Approxing(TC&& value) : value(std::forward<TC>(value)) { }

	template <typename V> friend inline bool operator==(const Approxing& lhs, const V& rhs) {
		return std::abs(lhs.value - rhs) <= epsilon;
	}

	template <typename V> friend inline bool operator==(const V& lhs, const Approxing& rhs) {
		return rhs == lhs;
	}

	template <typename V> friend inline bool operator>(const Approxing& lhs, const V& rhs) {
		return lhs + epsilon > rhs;
	}

	template <typename V> friend inline bool operator>(const V& lhs, const Approxing& rhs) {
		return lhs > rhs - epsilon;
	}

	template <typename V> friend inline bool operator<(const Approxing& lhs, const V& rhs) {
		return !(rhs > lhs);
	}

	template <typename V> friend inline bool operator<(const V& lhs, const Approxing& rhs) {
		return !(rhs > lhs);
	}

	template <typename OS> friend OS& operator<<(OS& os, const Approxing& av) {
		os << '~' << av.value;
		return os;
	}
};

template <typename TC>
Approxing(TC&& value) -> Approxing<std::decay_t<TC>>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
