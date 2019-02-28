// File:   chrono.hpp Author: Vader Created on 01 March 2019, 10:48

#pragma once

// std
#include <chrono>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct clock {
	using rep = double;
	using period = std::chrono::seconds::period;
	using duration = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<std::chrono::steady_clock, duration>;

	static constexpr bool is_steady = true;

	inline static time_point now() noexcept {
		return {std::chrono::steady_clock::now()};
	}
};

using time_duration = clock::duration;
using time_point = clock::time_point;

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
