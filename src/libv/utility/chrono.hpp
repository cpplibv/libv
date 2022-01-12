// Project: libv.utility, File: src/libv/utility/chrono.hpp

#pragma once

// std
#include <chrono>
#include <cmath>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct clock {
	using rep = double;
	using period = std::chrono::seconds::period;
	using duration = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<std::chrono::steady_clock, duration>;

	static constexpr bool is_steady = true;

	[[nodiscard]] static inline time_point now() noexcept {
		return {std::chrono::steady_clock::now()};
	}
};

using time_duration_f = std::chrono::duration<float, clock::period>;
using time_duration = clock::duration;
using time_point = clock::time_point;

// -------------------------------------------------------------------------------------------------

template <typename RepX, typename PeriodX, typename RepY, typename PeriodY>
[[nodiscard]] constexpr inline auto time_mod(std::chrono::duration<RepX, PeriodX> x, std::chrono::duration<RepY, PeriodY> y) noexcept {
	using common_type = std::common_type_t<decltype(x), decltype(y)>;

	const auto commonX = common_type{x};
	const auto commonY = common_type{y};

	if constexpr (std::is_floating_point_v<typename common_type::rep>)
		return common_type{std::fmod(commonX.count(), commonY.count())};
	else
		return common_type{commonX.count() % commonY.count()};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
