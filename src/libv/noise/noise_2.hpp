// Project: libv.noise, File: src/libv/noise/noise.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/meta/force_inline.hpp>
// pro
#include <libv/noise/seed.hpp>


namespace libv {
namespace noise {

// --- Value ---------------------------------------------------------------------------------------

constexpr inline struct ValueFn {
	[[nodiscard]] static float operator()(Seed seed, float x, float y) noexcept;
	[[nodiscard]] static float operator()(Seed seed, float x, float y, float z) noexcept;

	[[nodiscard]] static LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec2f coord) noexcept {
		return operator()(seed, coord.x, coord.y);
	}
	[[nodiscard]] static LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec3f coord) noexcept {
		return operator()(seed, coord.x, coord.y, coord.z);
	}
} value;

// --- Perlin --------------------------------------------------------------------------------------

constexpr inline struct PerlinFn {
	[[nodiscard]] static float operator()(Seed seed, float x, float y) noexcept;
	[[nodiscard]] static float operator()(Seed seed, float x, float y, float z) noexcept;

	[[nodiscard]] LIBV_FORCE_INLINE static float operator()(Seed seed, libv::vec2f coord) noexcept {
		return operator()(seed, coord.x, coord.y);
	}
	[[nodiscard]] LIBV_FORCE_INLINE static float operator()(Seed seed, libv::vec3f coord) noexcept {
		return operator()(seed, coord.x, coord.y, coord.z);
	}
} perlin;

// --- Simplex -------------------------------------------------------------------------------------

constexpr inline struct SimplexFn {
	[[nodiscard]] static float operator()(Seed seed, float x, float y) noexcept;
	[[nodiscard]] static float operator()(Seed seed, float x, float y, float z) noexcept;

	[[nodiscard]] LIBV_FORCE_INLINE static float operator()(Seed seed, libv::vec2f coord) noexcept {
		return operator()(seed, coord.x, coord.y);
	}

	[[nodiscard]] LIBV_FORCE_INLINE static float operator()(Seed seed, libv::vec3f coord) noexcept {
		return operator()(seed, coord.x, coord.y, coord.z);
	}
} simplex;

// --- Cellular ------------------------------------------------------------------------------------

enum class CellularDistanceFunction {
	euclidean = 0,
	euclideanSq = 1,
	manhattan = 2,
	hybrid = 3,
};

enum class CellularReturnType {
	cellValue = 0,
	distance = 1,
	distance2 = 2,
	distance2Add = 3,
	distance2Sub = 4,
	distance2Mul = 5,
	distance2Div = 6,
};

constexpr inline struct CellularFn {
private:
	CellularDistanceFunction _distanceFn = CellularDistanceFunction::euclidean;
	CellularReturnType _returnType = CellularReturnType::cellValue;
	float _jitter = 1.0f;

public:
	CellularFn() = default;
private:
	CellularFn(
			CellularDistanceFunction distanceFn,
			CellularReturnType returnType,
			float jitter) :
		_distanceFn(distanceFn),
		_returnType(returnType),
		_jitter(jitter) {}

public:
	[[nodiscard]] static constexpr LIBV_FORCE_INLINE CellularFn operator()(
			CellularDistanceFunction distanceFn = CellularDistanceFunction::euclidean,
			CellularReturnType returnType = CellularReturnType::cellValue,
			float jitter = 1.0f) noexcept {
		return CellularFn{distanceFn, returnType, jitter};
	}

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, float x, float y) const noexcept {
		return operator()(seed, x, y, _distanceFn, _returnType, _jitter);
	}

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, float x, float y, float z) const noexcept {
		return operator()(seed, x, y, z, _distanceFn, _returnType, _jitter);
	}

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec2f coord) const noexcept {
		return operator()(seed, coord.x, coord.y, _distanceFn, _returnType, _jitter);
	}

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec3f coord) const noexcept {
		return operator()(seed, coord.x, coord.y, coord.z, _distanceFn, _returnType, _jitter);
	}

	[[nodiscard]] static float operator()(Seed seed, float x, float y,
			CellularDistanceFunction distanceFn,
			CellularReturnType returnType,
			float jitter) noexcept;

	[[nodiscard]] static float operator()(Seed seed, float x, float y, float z,
			CellularDistanceFunction distanceFn,
			CellularReturnType returnType,
			float jitter) noexcept;

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec2f coord,
				CellularDistanceFunction distanceFn,
				CellularReturnType returnType,
				float jitter) const noexcept {
		return operator()(seed, coord.x, coord.y, distanceFn, returnType, jitter);
	}

	[[nodiscard]] LIBV_FORCE_INLINE float operator()(Seed seed, libv::vec3f coord,
				CellularDistanceFunction distanceFn,
				CellularReturnType returnType,
				float jitter) const noexcept {
		return operator()(seed, coord.x, coord.y, coord.z, distanceFn, returnType, jitter);
	}
} cellular;

// -------------------------------------------------------------------------------------------------

// struct SimplexGradFn {
// 	[[nodiscard]] static float operator()(Seed seed, float x, float y) noexcept;
// 	[[nodiscard]] static float operator()(Seed seed, float x, float y, float z) noexcept;
//
// 	[[nodiscard]] LIBV_FORCE_INLINE static float operator()(Seed seed, libv::vec2f coord) noexcept {
// 		return operator()(seed, coord.x, coord.y);
// 	}
//
// 	[[nodiscard]] LIBV_FORCE_INLINE static float operator()(Seed seed, libv::vec3f coord) noexcept {
// 		return operator()(seed, coord.x, coord.y, coord.z);
// 	}
// } simplex_grad;

// -------------------------------------------------------------------------------------------------

/// FBM (Fractional Brownian Motion)
template <typename NoiseFunc>
[[nodiscard]] float fractal(
		Seed seed,
		float x, float y,
		NoiseFunc noiseFunc,
		int octaves,
		float amplitude = 1.0f,
		float frequency = 1.0f,
		float lacunarity = 2.0f,
		float persistence = 0.5f) {
	float sum = 0;

	for (int32_t i = 0; i < octaves; ++i) {
		float noise = noiseFunc(seed++, x * frequency, y * frequency);
		sum += noise * amplitude;

		frequency *= lacunarity;
		amplitude *= persistence;
	}

	return sum;
}

/// FBM (Fractional Brownian Motion)
template <typename NoiseFunc>
[[nodiscard]] float fractal(
		Seed seed,
		float x, float y, float z,
		NoiseFunc noiseFunc,
		int octaves,
		float amplitude = 1.0f,
		float frequency = 1.0f,
		float lacunarity = 2.0f,
		float persistence = 0.5f) {
	float sum = 0;

	for (int32_t i = 0; i < octaves; ++i) {
		float noise = noiseFunc(seed++, x * frequency, y * frequency, z * frequency);
		sum += noise * amplitude;

		frequency *= lacunarity;
		amplitude *= persistence;
	}

	return sum;
}

// --- Warp ----------------------------------------------------------------------------------------

// enum class WarpType {
// 	simplex2,
// 	simplex2_reduced,
// 	basic_grid,
// };
//
// enum class WarpFractalType {
// //	none,
// //	progressive,
// 	independent,
// };
//
// libv::vec2f singleWarpSimplexGradient(Seed seed, float x, float y, float xr, float yr, float warpAmp, float frequency, bool outGradOnly);
//
// /// Domain Warp
// libv::vec2f doSingleWarp(Seed seed, float x, float y, float xr, float yr, WarpType type, float amp, float freq);
//
// /// Domain Warp Fractal Independent
// libv::vec2f warpFractalIndependent(
// 		Seed seed,
// 		float x, float y,
// 		int octaves = 5,
// 		float amplitude = 1.0f,
// 		float frequency = 0.01f,
// 		float lacunarity = 2.0f,
// 		float persistence = 0.5f,
// 		WarpType type = WarpType::simplex2);
//
//
// /// 2D warps the input position
// libv::vec2f warp(
// 		Seed seed,
// 		float x, float y,
// 		WarpFractalType type,
// 		int octaves = 5,
// 		float amplitude = 1.0f,
// 		float frequency = 0.01f,
// 		float lacunarity = 2.0f,
// 		float persistence = 0.5f);

// -------------------------------------------------------------------------------------------------

} // namespace noise
} // namespace libv
