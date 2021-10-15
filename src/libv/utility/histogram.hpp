// Project: libv.utility, File: src/libv/utility/histogram.hpp

#pragma once

// std
#include <array>
#include <chrono>
#include <ostream>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <size_t N>
struct Histogram {
	std::array<size_t, N> data;

	std::chrono::nanoseconds min;
	std::chrono::nanoseconds max;

	Histogram(std::chrono::nanoseconds min, std::chrono::nanoseconds max) : min(min), max(max) {
		data.fill(0);
	}

	void sample(std::chrono::nanoseconds value) {
		if (value > max)
			data[N - 1]++;
		else if (value < min)
			data[0]++;
		else
			data[(value - min).count() / ((max - min).count() / N)]++;
	}

	friend std::ostream& operator<<(std::ostream& os, const Histogram& var) {
		for (size_t i = 0; i < N; ++i)
			os << var.data[i] << ' ';
		return os;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
