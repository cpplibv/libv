// Created by dbobula on 2/12/2022.

#pragma once

// libv
#include <libv/math/vec.hpp>
//#include <libv/math/constants.hpp>
// std
#include <cassert>
#include <cmath>
// pro
#include <libv/noise/noise_util.hpp>
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

template <typename T>
[[nodiscard]] T noise_value(Seed seed, T x, T y) noexcept;

//[[nodiscard]] float noise_value(Seed seed, float x, float y, float z) noexcept;

template <typename T>
[[nodiscard]] float noise_perlin(Seed seed, T x, T y) noexcept;

template <typename T>
[[nodiscard]] T noise_simplex(Seed seed, T x, T y) noexcept;

//template <typename T>
//[[nodiscard]] float noise_simplex2(Seed seed, T x, T y) noexcept;

template <typename T>
[[nodiscard]] float noise_simplex2S(Seed seed, T x, T y);

template <typename T>
[[nodiscard]] constexpr inline T noise_simplex_g(T x, T y) noexcept;

template <typename T>
[[nodiscard]] constexpr inline T noise_simplex_glsl(T x, T y) noexcept;

template <typename T>
[[nodiscard]] T noise_cellular(
		Seed seed,
		T x, T y,
		CellularDistanceFunction distanceFn = CellularDistanceFunction::euclidean,
		CellularReturnType returnType = CellularReturnType::cellValue,
		float jitter = 1.0f);

/// FBM (Fractional Brownian Motion)
template <typename T, typename NoiseFn>
[[nodiscard]] float noise_fractal(
		Seed seed,
		T x, T y,
		NoiseFn noiseFn,
		int octaves,
		float amplitude = 1.0,
		float frequency = 1.0f,
		float lacunarity = 2.0f,
		float persistence = 0.5f);

// --- Warp ----------------------------------------------------------------------------------------

constexpr inline libv::vec2f singleWarpSimplexGradient(Seed seed, float x, float y, float xr, float yr, float warpAmp, float frequency, bool outGradOnly);

/// Domain Warp
constexpr inline libv::vec2f doSingleWarp(Seed seed, float x, float y, float xr, float yr, WarpType type, float amp, float freq);

/// Domain Warp Fractal Independent
constexpr inline libv::vec2f warpFractalIndependent(
		Seed seed,
		float x, float y,
		int octaves = 5,
		float amplitude = 1.0,
		float frequency = 0.01f,
		float lacunarity = 2.0f,
		float persistence = 0.5f,
		WarpType type = WarpType::simplex2);


/// 2D warps the input position
constexpr inline libv::vec2f warp(
		Seed seed,
		float x, float y,
		WarpFractalType type,
		int octaves = 5,
		float amplitude = 1.0,
		float frequency = 0.01f,
		float lacunarity = 2.0f,
		float persistence = 0.5f);

// -------------------------------------------------------------------------------------------------

} // namespace libv

#include <libv/noise/noise.inl>
