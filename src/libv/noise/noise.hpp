// Project: libv.noise, File: src/libv/noise/noise.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// pro
#include <libv/noise/seed.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

enum class CellularDistanceFunction {
	euclidean,
	euclideanSq,
	manhattan,
	hybrid
};

enum class CellularReturnType {
	cellValue,
	distance,
	distance2,
	distance2Add,
	distance2Sub,
	distance2Mul,
	distance2Div
};

//enum class FractalType {
//	none,
//	fbm,
//	ridged,
//	pingpong
//};

enum class WarpType {
	simplex2,
	simplex2_reduced,
	basic_grid,
};

enum class WarpFractalType {
//	none,
//	progressive,
	independent,
};

// -------------------------------------------------------------------------------------------------

[[nodiscard]] float noise_value(Seed seed, float x, float y) noexcept;
[[nodiscard]] float noise_perlin(Seed seed, float x, float y) noexcept;
[[nodiscard]] float noise_simplex(Seed seed, float x, float y) noexcept;

[[nodiscard]] float noise_simplex_g(float x, float y) noexcept; // Only baseline for glsl, remove it

/// Slower simplex noise implementation, but matches the glsl variant
/// The frequency is around twice of the \c noise_simplex variant
/// Based on https://github.com/BrianSharpe/Wombat
///		The implementation is optimized for glsl and therefore around 2x slower than the \c noise_simplex variant
[[nodiscard]] float noise_simplex_glsl(float x, float y) noexcept;

// -------------------------------------------------------------------------------------------------

[[nodiscard]] float noise_cellular(Seed seed, float x, float y,
		CellularDistanceFunction distanceFn = CellularDistanceFunction::euclidean,
		CellularReturnType returnType = CellularReturnType::cellValue,
		float jitter = 1.0f) noexcept;

/// FBM (Fractional Brownian Motion)
template <typename T, typename NoiseFn>
[[nodiscard]] float noise_fractal(
		Seed seed,
		T x, T y,
		NoiseFn noiseFn,
		int octaves,
		float amplitude = 1.0f,
		float frequency = 1.0f,
		float lacunarity = 2.0f,
		float persistence = 0.5f);

// --- Warp ----------------------------------------------------------------------------------------

libv::vec2f singleWarpSimplexGradient(Seed seed, float x, float y, float xr, float yr, float warpAmp, float frequency, bool outGradOnly);

/// Domain Warp
libv::vec2f doSingleWarp(Seed seed, float x, float y, float xr, float yr, WarpType type, float amp, float freq);

/// Domain Warp Fractal Independent
libv::vec2f warpFractalIndependent(
		Seed seed,
		float x, float y,
		int octaves = 5,
		float amplitude = 1.0f,
		float frequency = 0.01f,
		float lacunarity = 2.0f,
		float persistence = 0.5f,
		WarpType type = WarpType::simplex2);


/// 2D warps the input position
libv::vec2f warp(
		Seed seed,
		float x, float y,
		WarpFractalType type,
		int octaves = 5,
		float amplitude = 1.0f,
		float frequency = 0.01f,
		float lacunarity = 2.0f,
		float persistence = 0.5f);

// -------------------------------------------------------------------------------------------------

} // namespace libv

#include <libv/noise/noise.inl>
