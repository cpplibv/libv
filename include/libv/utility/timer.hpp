// File: timer.hpp, Created on 2015. május 4. 12:34, Author: Vader

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

	inline void reset() {
		time_ = std::chrono::steady_clock::now();
	}

	inline clock::duration time() {
		auto ct = std::chrono::steady_clock::now();
		auto lastTurn = ct - time_;
		time_ = ct;
		return lastTurn;
	}

	inline decltype(auto) time_s() {
		return std::chrono::duration_cast<std::chrono::seconds>(time());
	}
	inline decltype(auto) time_ms() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(time());
	}
	inline decltype(auto) time_us() {
		return std::chrono::duration_cast<std::chrono::microseconds>(time());
	}
	inline decltype(auto) time_ns() {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(time());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv