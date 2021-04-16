// Project: libv.math, File: src/libv/math/exponential_moving_average.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cmath>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, int Window = 10>
// TODO P5: GCC 11, floating point types as template arguments
//template <typename T, T Window = T{10}, T Match = T{0.9}>
struct exponential_moving_average {
public:
//	static_assert(T{0} < Match && Match < T{1});
	static_assert(T{0} < Window);

//	static constexpr T match = Match; /// Convergence rate over the window
	static constexpr T match = 0.8; /// Convergence rate over the window
	static constexpr T window = Window;
	static constexpr T alpha = -std::pow(T{1} - match, T{1} / window) + T{1};

private:
	T accumulator;

public:
	explicit constexpr inline exponential_moving_average(T init = T{}) noexcept :
		accumulator(init) {}

public:
	constexpr inline void add(T value) noexcept {
		accumulator = alpha * value + (T{1} - alpha) * accumulator;
	}

	constexpr inline void add(T value, T weight) noexcept {
		const auto weighted_alpha = T{1} - std::pow(T{1} - alpha, weight);
		accumulator = weighted_alpha * value + (T{1} - weighted_alpha) * accumulator;
	}

	[[nodiscard]] constexpr inline T value() const noexcept {
		return accumulator;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv

