// Project: libv.noise, File: src/libv/noise/noise_texture.cpp

// hpp
#include <libv/noise/noise_texture.hpp>
// std
#include <random>
#include <algorithm>
// libv
#include <libv/utility/random/uniform_distribution.hpp>


namespace libv {
namespace {

// -------------------------------------------------------------------------------------------------

class PerlinNoise {
	std::vector<int> p;

public:
	explicit PerlinNoise(uint64_t seed);

	double noise(double x, double y, double z);

private:
	static inline double fade(double t) noexcept;
	static inline double lerp(double t, double a, double b) noexcept;
	static inline double grad(int hash, double x, double y, double z) noexcept;
};

PerlinNoise::PerlinNoise(uint64_t seed) {
	p.resize(256);
	std::iota(p.begin(), p.end(), 0);
	std::mt19937_64 engine(seed);
	std::shuffle(p.begin(), p.end(), engine);
	p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y, double z) {
	auto X = static_cast<uint8_t>(std::floor(x));
	auto Y = static_cast<uint8_t>(std::floor(y));
	auto Z = static_cast<uint8_t>(std::floor(z));

	x -= std::floor(x);
	y -= std::floor(y);
	z -= std::floor(z);

	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)), lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))), lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)), lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
	return (res + 1.0) / 2.0;
}

inline double PerlinNoise::fade(double t) noexcept {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

inline double PerlinNoise::lerp(double t, double a, double b) noexcept {
	return a + t * (b - a);
}

inline double PerlinNoise::grad(int hash, double x, double y, double z) noexcept {
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

} // namespace -------------------------------------------------------------------------------------

std::vector<double> noise_texture_perlin2D(uint64_t seed, uint32_t X, uint32_t Y) {
	PerlinNoise gen(seed);

	std::vector<double> value;
	value.resize(X * Y);

	for (uint32_t y = 0; y < Y; y++) {
		for (uint32_t x = 0; x < X; x++) {
			double nx = x / X - 0.5;
			double ny = y / Y - 0.5;
			value[y * Y + x] = gen.noise(nx, ny, 0) * 0.5 + 0.5;
		}
	}
	return value;
}

// =================================================================================================

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

std::vector<libv::vec3uc> noise_white_2D_3uc(uint64_t seed, uint32_t x, uint32_t y) {
	std::mt19937_64 rng(seed);
	auto dist = libv::make_uniform_distribution_inclusive(uint8_t(0), uint8_t(255));

	std::vector<libv::vec3uc> result;
	result.resize(x * y);

	for (uint32_t i = 0; i < x * y; i++) {
		result[i].x = dist(rng);
		result[i].y = dist(rng);
		result[i].z = dist(rng);
	}

	return result;
}

// -------------------------------------------------------------------------------------------------


} // namespace libv
