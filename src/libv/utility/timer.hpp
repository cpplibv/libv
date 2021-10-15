// Project: libv.utility, File: src/libv/utility/timer.hpp

#pragma once

// std
#include <chrono>


namespace libv {

// -------------------------------------------------------------------------------------------------

class Timer {
public:
	using clock = std::chrono::steady_clock;

private:
	clock::time_point time_ = clock::now();

public:
	[[nodiscard]] inline clock::duration elapsed() const noexcept {
		return clock::now() - time_;
	}

	inline void adjust(clock::duration diff) noexcept {
		time_ += diff;
	}

	inline void reset() noexcept {
		time_ = clock::now();
	}

	[[nodiscard]] inline clock::duration time() noexcept {
		auto ct = clock::now();
		auto lastTurn = ct - time_;
		time_ = ct;
		return lastTurn;
	}

	[[nodiscard]] inline auto time_s() noexcept {
		return std::chrono::duration_cast<std::chrono::seconds>(time());
	}
	[[nodiscard]] inline auto time_ms() noexcept {
		return std::chrono::duration_cast<std::chrono::milliseconds>(time());
	}
	[[nodiscard]] inline auto time_us() noexcept {
		return std::chrono::duration_cast<std::chrono::microseconds>(time());
	}
	[[nodiscard]] inline auto time_ns() noexcept {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(time());
	}

	[[nodiscard]] inline auto timef_s() noexcept {
		return std::chrono::duration_cast<std::chrono::duration<float>>(time());
	}
	[[nodiscard]] inline auto timef_ms() noexcept {
		return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(time());
	}
	[[nodiscard]] inline auto timef_us() noexcept {
		return std::chrono::duration_cast<std::chrono::duration<float, std::micro>>(time());
	}
	[[nodiscard]] inline auto timef_ns() noexcept {
		return std::chrono::duration_cast<std::chrono::duration<float, std::nano>>(time());
	}

	[[nodiscard]] inline auto timed_s() noexcept {
		return std::chrono::duration_cast<std::chrono::duration<double>>(time());
	}
	[[nodiscard]] inline auto timed_ms() noexcept {
		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(time());
	}
	[[nodiscard]] inline auto timed_us() noexcept {
		return std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(time());
	}
	[[nodiscard]] inline auto timed_ns() noexcept {
		return std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(time());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
