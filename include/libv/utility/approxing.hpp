// File: approxing.hpp Author: Vader Created on 2017. augusztus 16., 23:59

#pragma once

// std
#include <cmath>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct Approxing {
	T value;
	T epsilon = 0.0001;

	Approxing(T value) : value(value) { }

	template <typename V>
	friend bool operator==(const Approxing& lhs, const V& rhs) {
		return std::abs(lhs.value - rhs) <= lhs.epsilon;
	}

	template <typename V>
	friend bool operator==(const V& lhs, const Approxing& rhs) {
		return rhs == lhs;
	}

	template <typename OS>
	friend OS& operator<<(OS& os, const Approxing& av) {
		os << '~' << av.value;
		return os;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv