// Project: libv.math, File: src/libv/math/exponential_moving_average.hpp

#pragma once

// std
#include <cassert>
#include <cmath>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Exponential moving average
/// A constant space/time average accumulator which uses 'rate' convergence
/// during a 'window' time instead of an exact mathematical average
template <typename T>
struct exp_moving_avg {
private:
	T accumulator;
	T alpha;

public:
	/// \param init - Initial value of the accumulator
	/// \param window - Size of the window in which 'rate' convergence occurs toward the new values.
	/// 		Higher values mean slower response
	/// \param rate - Convergence rate over the window. Must be between 0..1.
	/// 		Higher values mean faster response
	explicit constexpr inline exp_moving_avg(T init = T{}, T window = T{10}, T rate = T{0.9}) noexcept :
		accumulator(init),
		alpha(-std::pow(T{1} - rate, T{1} / window) + T{1}) {
		assert(T{0} < rate && rate < T{1});
		assert(T{0} < window);
	}

public:
	constexpr inline void add(T value) noexcept {
		accumulator = alpha * value + (T{1} - alpha) * accumulator;
	}

	/// \param value
	/// \param weight time to which the value was held. Must be positive
	constexpr inline void add(T value, T weight) noexcept {
		assert(T{0} < weight);

		const auto weighted_alpha = T{1} - std::pow(T{1} - alpha, weight);
		accumulator = weighted_alpha * value + (T{1} - weighted_alpha) * accumulator;
	}

	[[nodiscard]] constexpr inline T value() const noexcept {
		return accumulator;
	}
};

template <typename T>
using exp_moving_avg = struct exp_moving_avg<T>;

using exp_moving_avgf = struct exp_moving_avg<float>;
using exp_moving_avgd = struct exp_moving_avg<double>;

// -------------------------------------------------------------------------------------------------

} // namespace libv

