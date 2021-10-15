// Project: libv.utility, File: src/libv/utility/approx.hpp

#pragma once

// std
#include <cmath>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Epsilon is 10^Precision
/// @param T
/// @param Precision
template <typename T, int Precision = -5>
struct Approx {
	static constexpr T epsilon = T{std::pow<T>(10, Precision)};

	T value;

	Approx() = default;
	inline Approx(const Approx& orig) = default;
	inline Approx(Approx&& orig) = default;

	inline Approx(T value) :
		value(std::move(value)) { }

	template <typename V>
	friend inline bool operator==(const Approx& lhs, const V& rhs) {
		return std::abs(lhs.value - rhs) <= epsilon;
	}

	template <typename V>
	friend inline bool operator==(const V& lhs, const Approx& rhs) {
		return rhs == lhs;
	}

	template <typename V>
	friend inline bool operator!=(const Approx& lhs, const V& rhs) {
		return std::abs(lhs.value - rhs) > epsilon;
	}

	template <typename V>
	friend inline bool operator!=(const V& lhs, const Approx& rhs) {
		return rhs != lhs;
	}

	template <typename V>
	friend inline bool operator>(const Approx& lhs, const V& rhs) {
		return lhs + epsilon > rhs;
	}

	template <typename V>
	friend inline bool operator>(const V& lhs, const Approx& rhs) {
		return lhs > rhs - epsilon;
	}

	template <typename V>
	friend inline bool operator<(const Approx& lhs, const V& rhs) {
		return !(rhs > lhs);
	}

	template <typename V>
	friend inline bool operator<(const V& lhs, const Approx& rhs) {
		return !(rhs > lhs);
	}

	template <typename OS>
	friend OS& operator<<(OS& os, const Approx& av) {
		os << av.value << "±" << av.epsilon;
		return os;
	}
};

template <typename T>
Approx(T) -> Approx<T>;

// --- Aliases -------------------------------------------------------------------------------------

template <typename T, int Precision = -5>
using approx = Approx<T, Precision>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
