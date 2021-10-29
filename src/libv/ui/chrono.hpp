// Project: libv.ui, File: src/libv/ui/chrono.hpp

#pragma once

// libv
#include <libv/utility/chrono_mod.hpp>
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

	[[nodiscard]] static inline time_point now() noexcept {
		return {std::chrono::steady_clock::now()};
	}
};

using time_duration_f = std::chrono::duration<float, clock::period>;
using time_duration = clock::duration;
using time_point = clock::time_point;

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
