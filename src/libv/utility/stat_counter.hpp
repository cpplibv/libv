// File: stat_counter.hpp - Created on 2018.05.03. 01:49 - Author: Vader

#pragma once

// std
#include <cstddef>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct stat_counter {
private:
	T max_{};
	T min_{};
	T sum{};
	size_t count_ = 0;

public:
	void record(T value) {
		if (value > max_ || count_ == 0)
			max_ = value;
		if (value < min_ || count_ == 0)
			min_ = value;
		sum += value;
		count_++;
	}

	void reset() {
		count_ = 0;
		max_ = T{};
		min_ = T{};
		sum = T{};
	}

	T max() {
		return max_;
	}
	T min() {
		return min_;
	}
	T avg() {
		return sum / count_;
	}
	size_t count() {
		return count_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
