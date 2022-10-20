// Project: libv.utility, File: src/libv/utility/approx.hpp

#pragma once

// libv
#include <libv/meta/force_inline.hpp>
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

	constexpr LIBV_FORCE_INLINE Approx() noexcept = default;
	constexpr LIBV_FORCE_INLINE Approx(const Approx& orig) noexcept = default;
	constexpr LIBV_FORCE_INLINE Approx(Approx&& orig) noexcept = default;

	constexpr LIBV_FORCE_INLINE Approx(T value) noexcept :
		value(std::move(value)) { }

	template <typename V>
	constexpr friend LIBV_FORCE_INLINE bool operator==(const Approx& lhs, const V& rhs) noexcept {
		return std::abs(lhs.value - rhs) <= epsilon;
	}

	template <typename V>
	constexpr friend LIBV_FORCE_INLINE bool operator==(const V& lhs, const Approx& rhs) noexcept {
		return rhs == lhs;
	}

	template <typename V>
	constexpr friend LIBV_FORCE_INLINE bool operator!=(const Approx& lhs, const V& rhs) noexcept {
		return std::abs(lhs.value - rhs) > epsilon;
	}

	template <typename V>
	constexpr friend LIBV_FORCE_INLINE bool operator!=(const V& lhs, const Approx& rhs) noexcept {
		return rhs != lhs;
	}

	template <typename V>
	constexpr friend LIBV_FORCE_INLINE bool operator>(const Approx& lhs, const V& rhs) noexcept {
		return lhs + epsilon > rhs;
	}

	template <typename V>
	constexpr friend LIBV_FORCE_INLINE bool operator>(const V& lhs, const Approx& rhs) noexcept {
		return lhs > rhs - epsilon;
	}

	template <typename V>
	constexpr friend LIBV_FORCE_INLINE bool operator<(const Approx& lhs, const V& rhs) noexcept {
		return !(rhs > lhs);
	}

	template <typename V>
	constexpr friend LIBV_FORCE_INLINE bool operator<(const V& lhs, const Approx& rhs) noexcept {
		return !(rhs > lhs);
	}

	template <typename OS>
	friend inline OS& operator<<(OS& os, const Approx& av) {
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
