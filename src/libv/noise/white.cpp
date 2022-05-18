// Project: libv.math, File: src/libv/noise/white.cpp

// hpp
#include <libv/noise/white.hpp>
// libv
#include <libv/utility/random/uniform_distribution.hpp>
// std
#include <random>


namespace libv {

// -------------------------------------------------------------------------------------------------

//std::vector<double> noise_white_2D(uint64_t seed, uint32_t X, uint32_t Y) {
//	std::mt19937_64 rng(seed);
//	auto dist = libv::make_uniform_distribution_inclusive(0.0, 1.0);
//
//	std::vector<double> result;
//	result.resize(X * Y);
//
//	for (uint32_t y = 0; y < Y; y++)
//		for (uint32_t x = 0; x < X; x++)
//			result[y * Y + x] = dist(rng);
//
//	return result;
//}

std::vector<libv::vec3uc> noise_white_2D_3uc(uint64_t seed, uint32_t X, uint32_t Y) {
	std::mt19937_64 rng(seed);
	auto dist = libv::make_uniform_distribution_inclusive(uint8_t(0), uint8_t(255));

	std::vector<libv::vec3uc> result;
	result.resize(X * Y);

	for (uint32_t i = 0; i < X * Y; i++) {
		result[i].x = dist(rng);
		result[i].y = dist(rng);
		result[i].z = dist(rng);
	}

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
