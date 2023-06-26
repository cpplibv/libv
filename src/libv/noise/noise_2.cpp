// Project: libv.noise, File: src/libv/noise/noise.hpp

// hpp
#include <libv/noise/noise_2.hpp>
#include <libv/noise/__fastnoiselite.hpp>


namespace libv {
namespace noise {

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
	const float SQRT3 = 1.7320508075688772935274463415059f;
	const float F2 = 0.5f * (SQRT3 - 1);
	const float t = (x + y) * F2;
	x += t;
	y += t;
	// SingleOpenSimplex2S yield a different result in their GLSL and C++ implementation
	// return FastNoiseLite::SingleOpenSimplex2S(static_cast<int>(seed), x, y);
	return FastNoiseLite::SingleSimplex(static_cast<int>(seed), x, y);
}

float SimplexFn::operator()(libv::Seed seed, float x, float y, float z) noexcept {
	// Rotating is done here instead of TransformNoiseCoordinate bc we are direct calling _fnlSingleOpenSimplex2S3D
	const float R3 = 2.f / 3.f;
	const float r = (x + y + z) * R3; // Rotation, not skew
	x = r - x;
	y = r - y;
	z = r - z;
	// Not using SingleOpenSimplex2S to keep 2D and 3D aligned
	// return FastNoiseLite::SingleOpenSimplex2S(static_cast<int>(seed), x, y, z);
	return FastNoiseLite::SingleOpenSimplex2(static_cast<int>(seed), x, y, z);
}

// --- Cellular ------------------------------------------------------------------------------------

float CellularFn::operator()(Seed seed, float x, float y,
		CellularDistanceFunction distanceFn,
		CellularReturnType returnType,
		float jitter) noexcept {

	// return FastNoiseLite::SingleCellular(static_cast<int>(seed), x, y, jitter, distanceFn, returnType);
	// return FastNoiseLite::SingleCellular(static_cast<int>(seed), x, y, static_cast<int32_t>(distanceFn), static_cast<int32_t>(returnType), jitter);
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

// -------------------------------------------------------------------------------------------------

} // namespace noise
} // namespace libv
