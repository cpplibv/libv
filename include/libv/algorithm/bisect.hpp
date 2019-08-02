// File:   bisect.hpp Author: Vader Created on 02 August 2019, 05:44

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

///
/// Bisect with initial up ramping
/// If there are multiple roots, one will be chosen at pseudo random
/// @param func - signature: Integral(T), negative return value root is greater, positive return value root is smaller, zero as return value root is found
/// @param guess - initial hint
/// @param step - initial step, if zero the first function result will be used as step
/// @return the first found root
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

	// Binary search
	auto min = guess - step / 2 + 1;
	auto max = guess - 1;

	if (min > max)
		std::swap(min, max);

	while (hit != 0) {
		if (min == max)
			return min;

		guess = (max + min) / 2;
		hit = func(guess);

		if (hit < 0)
			min = guess + 1;
		else
			max = guess - 1;
	}

	return guess;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
