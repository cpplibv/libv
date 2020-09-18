// Project: libv.utility, File: src/libv/utility/timer.hpp, Author: Császár Mátyás [Vader]

#pragma once

#include <chrono>


namespace libv {

// -------------------------------------------------------------------------------------------------

class Timer {
public:
	using clock = std::chrono::steady_clock;
private:
	clock::time_point time_;
public:
	inline Timer() : time_(std::chrono::steady_clock::now()) { }

	inline clock::duration elapsed() {
		return std::chrono::steady_clock::now() - time_;
	}

	inline void adjust(clock::duration diff) {
		time_ += diff;
	}

	inline void reset() {
		time_ = std::chrono::steady_clock::now();
	}

	inline clock::duration time() {
		auto ct = std::chrono::steady_clock::now();
		auto lastTurn = ct - time_;
		time_ = ct;
		return lastTurn;
	}

	inline auto time_s() {
		return std::chrono::duration_cast<std::chrono::seconds>(time());
	}
	inline auto time_ms() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(time());
	}
	inline auto time_us() {
		return std::chrono::duration_cast<std::chrono::microseconds>(time());
	}
	inline auto time_ns() {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(time());
	}

	inline auto timef_s() {
		return std::chrono::duration_cast<std::chrono::duration<float>>(time());
	}
	inline auto timef_ms() {
		return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(time());
	}
	inline auto timef_us() {
		return std::chrono::duration_cast<std::chrono::duration<float, std::micro>>(time());
	}
	inline auto timef_ns() {
		return std::chrono::duration_cast<std::chrono::duration<float, std::nano>>(time());
	}

	inline auto timed_s() {
		return std::chrono::duration_cast<std::chrono::duration<double>>(time());
	}
	inline auto timed_ms() {
		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(time());
	}
	inline auto timed_us() {
		return std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(time());
	}
	inline auto timed_ns() {
		return std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(time());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
