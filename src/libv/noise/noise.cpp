// Project: libv.noise, File: src/libv/noise/noise.cpp

// hpp
#include <libv/noise/noise.hpp>
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

static_assert(to_underlying(CellularDistance::euclidean) == FastNoiseLite::CellularDistanceFunction_Euclidean);
static_assert(to_underlying(CellularDistance::euclideansq) == FastNoiseLite::CellularDistanceFunction_EuclideanSq);
static_assert(to_underlying(CellularDistance::manhattan) == FastNoiseLite::CellularDistanceFunction_Manhattan);
static_assert(to_underlying(CellularDistance::hybrid) == FastNoiseLite::CellularDistanceFunction_Hybrid);

static_assert(to_underlying(CellularReturn::cellvalue) == FastNoiseLite::CellularReturnType_CellValue);
static_assert(to_underlying(CellularReturn::distance) == FastNoiseLite::CellularReturnType_Distance);
static_assert(to_underlying(CellularReturn::distance2) == FastNoiseLite::CellularReturnType_Distance2);
static_assert(to_underlying(CellularReturn::distance2add) == FastNoiseLite::CellularReturnType_Distance2Add);
static_assert(to_underlying(CellularReturn::distance2sub) == FastNoiseLite::CellularReturnType_Distance2Sub);
static_assert(to_underlying(CellularReturn::distance2mul) == FastNoiseLite::CellularReturnType_Distance2Mul);
static_assert(to_underlying(CellularReturn::distance2div) == FastNoiseLite::CellularReturnType_Distance2Div);

float CellularFn::operator()(Seed seed, float x, float y,
		CellularDistance distanceFn,
		CellularReturn returnType,
		float jitter) noexcept {

	return FastNoiseLite::SingleCellular(static_cast<int>(seed), x, y,
			static_cast<FastNoiseLite::CellularDistanceFunction>(distanceFn),
			static_cast<FastNoiseLite::CellularReturnType>(returnType),
			jitter);
}

float CellularFn::operator()(Seed seed, float x, float y, float z,
		CellularDistance distanceFn,
		CellularReturn returnType,
		float jitter) noexcept {
	return FastNoiseLite::SingleCellular(static_cast<int>(seed), x, y, z,
			static_cast<FastNoiseLite::CellularDistanceFunction>(distanceFn),
			static_cast<FastNoiseLite::CellularReturnType>(returnType),
			jitter);
}

// --- Warp ------------------------------------------------------------------------------------

libv::vec2f SimplexGradientFn::operator()(Seed seed, float x, float y) noexcept {
	const auto [xr, yr] = skew(x, y);
	float x_out = 0, y_out = 0;
	FastNoiseLite::SingleDomainWarpSimplexGradientB(static_cast<int>(seed), xr, yr, x_out, y_out, false);
	return {x_out, y_out};
}

libv::vec3f SimplexGradientFn::operator()(Seed seed, float x, float y, float z) noexcept {
	const auto [xr, yr, zr] = rotate(x, y, z);
	float x_out = 0, y_out = 0, z_out = 0;
	FastNoiseLite::SingleDomainWarpSimplexGradientB(static_cast<int>(seed), xr, yr, zr, x_out, y_out, z_out, false);
	return {x_out, y_out, z_out};
}

} // namespace noise
} // namespace libv
