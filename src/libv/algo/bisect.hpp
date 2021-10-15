// Project: libv.algo, File: src/libv/algo/bisect.hpp

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

///
/// Bisect
/// If there are multiple roots, one will be chosen at pseudo random
/// @param func - signature: Integral(T), negative return value root is greater, positive return value root is smaller, zero as return value root is found
/// @param min - minimal value inclusive
/// @param max - maximal value inclusive
/// @return the first root found
/// @example `bisect_3way([](int guess) { return guess - 5; }, 0, 10) == 5`
///
template <typename T, typename F>
constexpr inline T bisect_3way(const F& func, T min, T max) noexcept(noexcept(func(min))) {
	if (min > max)
		std::swap(min, max);

	while (true) {
		if (min == max)
			return min;

		const auto guess = (min + max) / 2;
		const auto hit = func(guess);

		if (hit == 0)
			return guess;
		else if (hit < 0)
			min = guess + 1;
		else
			max = guess - 1;
	}

	return min; // unreachable
}

///
/// Bisect with initial up ramping
/// If there are multiple roots, one will be chosen at pseudo random
/// @param func - signature: Integral(T), negative return value root is greater, positive return value root is smaller, zero as return value root is found
/// @param guess - initial hint
/// @param step - initial step, if its zero the result of the first function call (with 0 as guess) multiplied with -1 will be used
/// @return the first root found
/// @example `bisect_rampup_3way([](int guess) { return guess - 5; }) == 5`
///
template <typename T, typename F>
constexpr inline T bisect_rampup_3way(const F& func, T guess = T{0}, T step = T{0}) noexcept(noexcept(func(guess))) {
	auto hit = func(guess);
	if (hit == 0)
		return guess;

	// Initial step cannot be zero
	if (step == 0)
		step = -hit;

	// Step hint facing the wrong direction
	if ((step < 0) == (hit < 0))
		step = -step;

	// Overshoot to find range
	if (step < 0)
		while (hit > 0) {
			guess += step;
			hit = func(guess);
			step *= 2;
		}
	else
		while (hit < 0) {
			guess += step;
			hit = func(guess);
			step *= 2;
		}

	if (hit == 0)
		return guess;

	// Binary search
	auto min = guess - step / 2 + 1;
	auto max = guess - 1;

	return bisect_3way(func, min, max);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
