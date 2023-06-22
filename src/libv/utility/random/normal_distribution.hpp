// Project: libv.utility, File: src/libv/utility/random/normal_distribution.hpp

#pragma once

// std
#include <functional> // For std::clamp only
#include <random>
//#include <chrono>
//#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

//namespace detail {
//
//// -------------------------------------------------------------------------------------------------
//
//class normal_distribution_clamp : std::normal_distribution {
//	operator()(RNG&& rng) {
//		...
//		clamp
//	}
//};
//
//class normal_distribution_repeat : std::normal_distribution {
//	operator()(RNG&& rng) {
//		...
//		repeat
//	}
//};
//
//} // namespace detail ------------------------------------------------------------------------------

/// Probability: 0.995
template <typename T, typename RNG>
	requires std::floating_point<T>
[[nodiscard]] constexpr inline T normal995_clamp(RNG&& rng, T mean, T range) noexcept {
	const auto dev = range * static_cast<T>(0.356);
	return std::clamp(std::normal_distribution(mean, dev)(rng), mean - range, mean + range);
}

/// Probability: 0.995
template <typename T, typename RNG>
	requires std::floating_point<T>
[[nodiscard]] constexpr inline T normal995_repeat(RNG&& rng, T mean, T range) noexcept {
	const auto dev = range * static_cast<T>(0.356);
	auto dist = std::normal_distribution(mean, dev);
	while (true) {
		const auto result = dist(rng);
		if (mean - range <= result && result <= mean + range)
			return result;
	}
}

/// Probability: 0.998
template <typename T, typename RNG>
	requires std::floating_point<T>
[[nodiscard]] constexpr inline T normal998_clamp(RNG&& rng, T mean, T range) noexcept {
	const auto dev = range * static_cast<T>(1.0 / 3.0);
	return std::clamp(std::normal_distribution(mean, dev)(rng), mean - range, mean + range);
}

/// Probability: 0.998
template <typename T, typename RNG>
	requires std::floating_point<T>
[[nodiscard]] constexpr inline T normal998_repeat(RNG&& rng, T mean, T range) noexcept {
	const auto dev = range * static_cast<T>(1.0 / 3.0);
	auto dist = std::normal_distribution(mean, dev);
	while (true) {
		const auto result = dist(rng);
		if (mean - range <= result && result <= mean + range)
			return result;
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

