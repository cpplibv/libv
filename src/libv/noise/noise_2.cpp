// Project: libv.noise, File: src/libv/noise/noise.hpp

// hpp
#include <libv/noise/noise_2.hpp>
// ext
#include <libv/noise/__fastnoiselite.hpp>
// libv
#include <libv/utility/to_underlying.hpp>


namespace libv {
namespace noise {

// --- Utility ----------------------------------------------------------------------------------------

libv::vec2f skew(float x, float y) {
	const float SQRT3 = 1.7320508075688772935274463415059f;
	const float F2 = 0.5f * (SQRT3 - 1);
	float const t = (x + y) * F2;
	x += t;
	y += t;

	return libv::vec2f{x, y};
}

libv::vec3f rotate(float x, float y, float z) {
	const float R3 = 2.f / 3.f;
	const float r = (x + y + z) * R3; // Rotation, not skew
	x = r - x;
	y = r - y;
	z = r - z;

	return libv::vec3f{x, y, z};
}

// --- Value ---------------------------------------------------------------------------------------

float ValueFn::operator()(libv::Seed seed, float x, float y) noexcept {
	return FastNoiseLite::SingleValue(static_cast<int>(seed), x, y);
}

float ValueFn::operator()(libv::Seed seed, float x, float y, float z) noexcept {
	return FastNoiseLite::SingleValue(static_cast<int>(seed), x, y, z);
}

// --- Perlin --------------------------------------------------------------------------------------

float PerlinFn::operator()(libv::Seed seed, float x, float y) noexcept {
	return FastNoiseLite::SinglePerlin(static_cast<int>(seed), x, y);
}

float PerlinFn::operator()(libv::Seed seed, float x, float y, float z) noexcept {
	return FastNoiseLite::SinglePerlin(static_cast<int>(seed), x, y, z);
}

// --- Simplex -------------------------------------------------------------------------------------

float SimplexFn::operator()(libv::Seed seed, float x, float y) noexcept {
	// Skewing is done here instead of TransformNoiseCoordinate bc we are direct calling _fnlSingleOpenSimplex2S2D
	const auto [xr, yr] = skew(x, y);
	// SingleOpenSimplex2S yield a different result in their GLSL and C++ implementation
	// return FastNoiseLite::SingleOpenSimplex2S(static_cast<int>(seed), x, y);
	return FastNoiseLite::SingleSimplex(static_cast<int>(seed), xr, yr);
}

float SimplexFn::operator()(libv::Seed seed, float x, float y, float z) noexcept {
	// Rotating is done here instead of TransformNoiseCoordinate bc we are direct calling _fnlSingleOpenSimplex2S3D
	const auto [xr, yr, zr] = rotate(x, y, z);

	// Not using SingleOpenSimplex2S to keep 2D and 3D aligned
	// return FastNoiseLite::SingleOpenSimplex2S(static_cast<int>(seed), x, y, z);
	return FastNoiseLite::SingleOpenSimplex2(static_cast<int>(seed), xr, yr, zr);
}

// --- Cellular ------------------------------------------------------------------------------------

static_assert(to_underlying(CellularDistanceFunction::euclidean) == FastNoiseLite::CellularDistanceFunction_Euclidean);
static_assert(to_underlying(CellularDistanceFunction::euclideanSq) == FastNoiseLite::CellularDistanceFunction_EuclideanSq);
static_assert(to_underlying(CellularDistanceFunction::manhattan) == FastNoiseLite::CellularDistanceFunction_Manhattan);
static_assert(to_underlying(CellularDistanceFunction::hybrid) == FastNoiseLite::CellularDistanceFunction_Hybrid);

static_assert(to_underlying(CellularReturnType::cellValue) == FastNoiseLite::CellularReturnType_CellValue);
static_assert(to_underlying(CellularReturnType::distance) == FastNoiseLite::CellularReturnType_Distance);
static_assert(to_underlying(CellularReturnType::distance2) == FastNoiseLite::CellularReturnType_Distance2);
static_assert(to_underlying(CellularReturnType::distance2Add) == FastNoiseLite::CellularReturnType_Distance2Add);
static_assert(to_underlying(CellularReturnType::distance2Sub) == FastNoiseLite::CellularReturnType_Distance2Sub);
static_assert(to_underlying(CellularReturnType::distance2Mul) == FastNoiseLite::CellularReturnType_Distance2Mul);
static_assert(to_underlying(CellularReturnType::distance2Div) == FastNoiseLite::CellularReturnType_Distance2Div);

float CellularFn::operator()(Seed seed, float x, float y,
		CellularDistanceFunction distanceFn,
		CellularReturnType returnType,
		float jitter) noexcept {

	return FastNoiseLite::SingleCellular(static_cast<int>(seed), x, y,
			static_cast<FastNoiseLite::CellularDistanceFunction>(distanceFn),
			static_cast<FastNoiseLite::CellularReturnType>(returnType),
			jitter);
}

float CellularFn::operator()(Seed seed, float x, float y, float z,
		CellularDistanceFunction distanceFn,
		CellularReturnType returnType,
		float jitter) noexcept {
	return FastNoiseLite::SingleCellular(static_cast<int>(seed), x, y, z,
			static_cast<FastNoiseLite::CellularDistanceFunction>(distanceFn),
			static_cast<FastNoiseLite::CellularReturnType>(returnType),
			jitter);

}

// --- Warp ------------------------------------------------------------------------------------


libv::vec2f WarpFn::operator()(Seed seed, float x, float y) noexcept {
	const auto [xr, yr] = skew(x, y);
	float x_out, y_out;
	FastNoiseLite::SingleDomainWarpSimplexGradient2(static_cast<int>(seed), xr, yr, x_out, y_out, false);
	return {x_out, y_out};
}

libv::vec3f WarpFn::operator()(Seed seed, float x, float y, float z) noexcept {
	const auto [xr, yr, zr] = rotate(x, y, z);
	float x_out, y_out, z_out;
	FastNoiseLite::SingleDomainWarpSimplexGradient2(static_cast<int>(seed), xr, yr, zr, x_out, y_out, z_out, false);
	return {x_out, y_out, z_out};
}

libv::vec2f warp_fractal(
		Seed seed,
		float x, float y,
		int octaves,
		float amplitude,
		float frequency,
		float lacunarity,
		float persistence) noexcept {
	libv::vec2f result(x, y);

	for (int i = 0; i < octaves; i++) {
		result += warp(seed++, result.x * frequency, result.y * frequency) * amplitude;

		frequency *= lacunarity;
		amplitude *= persistence;
	}

	return result;
}

libv::vec3f warp_fractal(
		Seed seed,
		float x, float y, float z,
		int octaves,
		float amplitude,
		float frequency,
		float lacunarity,
		float persistence) noexcept {
	libv::vec3f result(x, y, z);

	for (int i = 0; i < octaves; i++) {
		result += warp(seed++, result.x * frequency, result.y * frequency, result.z * frequency) * amplitude;

		frequency *= lacunarity;
		amplitude *= persistence;
	}

	return result;
}

} // namespace noise
} // namespace libv
