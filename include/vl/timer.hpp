// File: timer.hpp, Created on 2015. május 4. 12:34, Author: Vader

#pragma once

#include <chrono>

namespace vl {

class Timer {
public:
	using clock = std::chrono::steady_clock;
private:
	clock::time_point startTime;
	clock::duration _lastTurn;
public:
	Timer() : startTime(std::chrono::steady_clock::now()) { }
	clock::duration elapsed() {
		return std::chrono::steady_clock::now() - startTime;
	}
	void reset() {
		startTime = std::chrono::steady_clock::now();
	}
	clock::duration lastTurn() {
		return _lastTurn;
	}
	clock::duration time() {
		auto ct = std::chrono::steady_clock::now();
		_lastTurn = ct - startTime;
		startTime = ct;
		return _lastTurn;
	}
};

} //namespace vl