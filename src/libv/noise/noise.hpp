// Created by dbobula on 2/12/2022.

#include <cmath>
#include <libv/noise/noise_util.hpp>


#pragma once


namespace libv {

//float, double, libv::vec4f, libv::vec4d
template <typename T>
[[nodiscard]] T noise_value(int seed, T x, T y) noexcept {
	int x0 = static_cast<int>(std::floor(x));
	int y0 = static_cast<int>(std::floor(y));
//	int x0 = fastFloor(x);
//	int y0 = fastFloor(y);

	T xs = hermiteInterp(static_cast<T>(x - x0));
	T ys = hermiteInterp(static_cast<T>(y - y0));

	x0 *= PrimeX;
	y0 *= PrimeY;
	int x1 = x0 + PrimeX;
	int y1 = y0 + PrimeY;

	T xf0 = lerp(ValCoord(seed, x0, y0), ValCoord(seed, x1, y0), xs);
	T xf1 = lerp(ValCoord(seed, x0, y1), ValCoord(seed, x1, y1), xs);

	return lerp(xf0, xf1, ys);
}

[[nodiscard]] float noise_value(int seed, float x, float y, float z) noexcept;

//float, double, libv::vec4f, libv::vec4d
template <typename T>
[[nodiscard]] float noise_perlin(int seed, T x, T y) noexcept {
	int x0 = static_cast<int>(std::floor(x));
	int y0 = static_cast<int>(std::floor(y));

	auto xd0 = static_cast<T>(x - x0);
	auto yd0 = static_cast<T>(y - y0);
	T xd1 = xd0 - 1;
	T yd1 = yd0 - 1;

	T xs = quinticInterp(xd0);
	T ys = quinticInterp(yd0);

	x0 *= PrimeX;
	y0 *= PrimeY;
	int x1 = x0 + PrimeX;
	int y1 = y0 + PrimeY;

	T xf0 = lerp(gradCoord(seed, x0, y0, xd0, yd0), gradCoord(seed, x1, y0, xd1, yd0), xs);
	T xf1 = lerp(gradCoord(seed, x0, y1, xd0, yd1), gradCoord(seed, x1, y1, xd1, yd1), xs);

	return lerp(xf0, xf1, ys) * 1.4247691104677813f;
}

template <typename T>
[[nodiscard]] float noise_simplex(int seed, T x, T y) noexcept {
	// 2D OpenSimplex2 case uses the same algorithm as ordinary Simplex.

	const float SQRT3 = 1.7320508075688772935274463415059f;
	const float G2 = (3 - SQRT3) / 6;

	/*
	 * --- Skew moved to TransformNoiseCoordinate method ---
	 * const FNfloat F2 = 0.5f * (SQRT3 - 1);
	 * FNfloat s = (x + y) * F2;
	 * x += s; y += s;
	*/

	int i = static_cast<int>(std::floor(x));
	int j = static_cast<int>(std::floor(y));
	float xi = static_cast<float> (x - i);
	float yi = static_cast<float> (y - j);

	float t = (xi + yi) * G2;
	float x0 = static_cast<float> (xi - t);
	float y0 = static_cast<float> (yi - t);

	i *= PrimeX;
	j *= PrimeY;

	float n0, n1, n2;

	float a = 0.5f - x0 * x0 - y0 * y0;
	if (a <= 0) n0 = 0;
	else {
		n0 = (a * a) * (a * a) * gradCoord(seed, i, j, x0, y0);
	}

	float c = static_cast<float> (2 * (1 - 2 * G2) * (1 / G2 - 2)) * t + (static_cast<float> (-2 * (1 - 2 * G2) * (1 - 2 * G2)) + a);
	if (c <= 0) n2 = 0;
	else {
		float x2 = x0 + (2 * static_cast<float> (G2) - 1);
		float y2 = y0 + (2 * static_cast<float> (G2) - 1);
		n2 = (c * c) * (c * c) * gradCoord(seed, i + PrimeX, j + PrimeY, x2, y2);
	}

	if (y0 > x0) {
		float x1 = x0 + static_cast<float> (G2);
		float y1 = y0 + (static_cast<float> (G2) - 1);
		float b = 0.5f - x1 * x1 - y1 * y1;
		if (b <= 0) n1 = 0;
		else {
			n1 = (b * b) * (b * b) * gradCoord(seed, i, j + PrimeY, x1, y1);
		}
	} else {
		float x1 = x0 + (static_cast<float> (G2) - 1);
		float y1 = y0 + static_cast<float> (G2);
		float b = 0.5f - x1 * x1 - y1 * y1;
		if (b <= 0) n1 = 0;
		else {
			n1 = (b * b) * (b * b) * gradCoord(seed, i + PrimeX, j, x1, y1);
		}
	}

	return (n0 + n1 + n2) * 99.83685446303647f;
}

template <typename T>
float noise_simplex2S(int seed, T x, T y) {
	// 2D OpenSimplex2S case is a modified 2D simplex noise.

	const T SQRT3 = (T) 1.7320508075688772935274463415059;
	const T G2 = (3 - SQRT3) / 6;

	/*
	 * --- Skew moved to TransformNoiseCoordinate method ---
	 * const FNfloat F2 = 0.5f * (SQRT3 - 1);
	 * FNfloat s = (x + y) * F2;
	 * x += s; y += s;
	*/

	int i = static_cast<int>(std::floor(x));
	int j = static_cast<int>(std::floor(y));
	float xi = static_cast<float> (x - i);
	float yi = static_cast<float> (y - j);

	i *= PrimeX;
	j *= PrimeY;
	int i1 = i + PrimeX;
	int j1 = j + PrimeY;

	float t = (xi + yi) * static_cast<float> (G2);
	float x0 = xi - t;
	float y0 = yi - t;

	float a0 = (2.0f / 3.0f) - x0 * x0 - y0 * y0;
	float value = (a0 * a0) * (a0 * a0) * gradCoord(seed, i, j, x0, y0);

	float a1 = static_cast<float> (2 * (1 - 2 * G2) * (1 / G2 - 2)) * t + (static_cast<float> (-2 * (1 - 2 * G2) * (1 - 2 * G2)) + a0);
	float x1 = x0 - static_cast<float> (1 - 2 * G2);
	float y1 = y0 - static_cast<float> (1 - 2 * G2);
	value += (a1 * a1) * (a1 * a1) * gradCoord(seed, i1, j1, x1, y1);

	// Nested conditionals were faster than compact bit logic/arithmetic.
	float xmyi = xi - yi;
	if (t > G2) {
		if (xi + xmyi > 1) {
			float x2 = x0 + static_cast<float> (3 * G2 - 2);
			float y2 = y0 + static_cast<float> (3 * G2 - 1);
			float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
			if (a2 > 0) {
				value += (a2 * a2) * (a2 * a2) * gradCoord(seed, i + (PrimeX << 1), j + PrimeY, x2, y2);
			}
		} else {
			float x2 = x0 + static_cast<float> (G2);
			float y2 = y0 + static_cast<float> (G2 - 1);
			float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
			if (a2 > 0) {
				value += (a2 * a2) * (a2 * a2) * gradCoord(seed, i, j + PrimeY, x2, y2);
			}
		}

		if (yi - xmyi > 1) {
			float x3 = x0 + static_cast<float> (3 * G2 - 1);
			float y3 = y0 + static_cast<float> (3 * G2 - 2);
			float a3 = (2.0f / 3.0f) - x3 * x3 - y3 * y3;
			if (a3 > 0) {
				value += (a3 * a3) * (a3 * a3) * gradCoord(seed, i + PrimeX, j + (PrimeY << 1), x3, y3);
			}
		} else {
			float x3 = x0 + static_cast<float> (G2 - 1);
			float y3 = y0 + static_cast<float> (G2);
			float a3 = (2.0f / 3.0f) - x3 * x3 - y3 * y3;
			if (a3 > 0) {
				value += (a3 * a3) * (a3 * a3) * gradCoord(seed, i + PrimeX, j, x3, y3);
			}
		}
	} else {
		if (xi + xmyi < 0) {
			float x2 = x0 + static_cast<float> (1 - G2);
			float y2 = y0 - static_cast<float> (G2);
			float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
			if (a2 > 0) {
				value += (a2 * a2) * (a2 * a2) * gradCoord(seed, i - PrimeX, j, x2, y2);
			}
		} else {
			float x2 = x0 + static_cast<float> (G2 - 1);
			float y2 = y0 + static_cast<float> (G2);
			float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
			if (a2 > 0) {
				value += (a2 * a2) * (a2 * a2) * gradCoord(seed, i + PrimeX, j, x2, y2);
			}
		}

		if (yi < xmyi) {
			float x2 = x0 - static_cast<float> (G2);
			float y2 = y0 - static_cast<float> (G2 - 1);
			float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
			if (a2 > 0) {
				value += (a2 * a2) * (a2 * a2) * gradCoord(seed, i, j - PrimeY, x2, y2);
			}
		} else {
			float x2 = x0 + static_cast<float> (G2);
			float y2 = y0 + static_cast<float> (G2 - 1);
			float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
			if (a2 > 0) {
				value += (a2 * a2) * (a2 * a2) * gradCoord(seed, i, j + PrimeY, x2, y2);
			}
		}
	}

	return value * 18.24196194486065f;
}

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

namespace details {

//float, double, libv::vec4f, libv::vec4d
template <typename T, typename DistFn>
[[nodiscard]] T aux_noise_cellular(int seed, T x, T y,
		DistFn&& distFn,
		CellularDistanceFunction distFnType,
		CellularReturnType returnType,
		float jitter) noexcept {

	int xr = static_cast<int>(std::round(x));
	int yr = static_cast<int>(std::round(y));
	float distance0 = 1e10f;
	float distance1 = 1e10f;
	int closestHash = 0;
	float cellularJitter = 0.43701595f * jitter;
	int xPrimed = (xr - 1) * PrimeX;
	int yPrimedBase = (yr - 1) * PrimeY;

	for (int xi = xr - 1; xi <= xr + 1; xi++) {
		int yPrimed = yPrimedBase;
		for (int yi = yr - 1; yi <= yr + 1; yi++) {
			int hashVal = hash(seed, xPrimed, yPrimed);
			int idx = hashVal & (255 << 1);
			T vecX = static_cast<T>(xi - x) + Lookup<T>::RandVecs2D[idx] * cellularJitter;
			T vecY = static_cast<T>(yi - y) + Lookup<T>::RandVecs2D[idx | 1] * cellularJitter;
			T newDistance = distFn(vecX, vecY);
			distance1 = std::max(std::min(distance1, newDistance), distance0);
			if (newDistance < distance0) {
				distance0 = newDistance;
				closestHash = hashVal;
			}
			yPrimed += PrimeY;
		}
		xPrimed += PrimeX;
	}

	if (distFnType == CellularDistanceFunction::euclidean && returnType != CellularReturnType::cellValue) {
		distance0 = sqrtf(distance0);
		if (returnType != CellularReturnType::distance) {
			distance1 = sqrtf(distance1);
		}
	}

	switch (returnType) {
	case CellularReturnType::cellValue:
		return closestHash * (1 / 2147483648.0f);
	case CellularReturnType::distance:
		return distance0 - 1;
	case CellularReturnType::distance2:
		return distance1 - 1;
	case CellularReturnType::distance2Add:
		return (distance1 + distance0) * 0.5f - 1;
	case CellularReturnType::distance2Sub:
		return distance1 - distance0 - 1;
	case CellularReturnType::distance2Mul:
		return distance1 * distance0 * 0.5f - 1;
	case CellularReturnType::distance2Div:
		return distance0 / distance1 - 1;
	default:
		return 0;
	}

}

} // namespace details


template <typename T>
T noise_cellular(int seed, T x, T y,
		CellularDistanceFunction distanceFn = CellularDistanceFunction::euclidean,
		CellularReturnType returnType = CellularReturnType::cellValue,
		float jitter = 1.0f) {

	switch (distanceFn) {
	case CellularDistanceFunction::euclidean:
	case CellularDistanceFunction::euclideanSq:
		return details::aux_noise_cellular(seed, x, y, [](const auto& x, const auto& y) {
			return x * x + y * y;
		}, distanceFn, returnType, jitter);
	case CellularDistanceFunction::manhattan:
		return details::aux_noise_cellular(seed, x, y, [](const auto& x, const auto& y) {
			return std::abs(x) + std::abs(y);
		}, distanceFn, returnType, jitter);

	case CellularDistanceFunction::hybrid:
		return details::aux_noise_cellular(seed, x, y, [](const auto& x, const auto& y) {
			return std::abs(x) + std::abs(y) + x * x + y * y;
		}, distanceFn, returnType, jitter);
	}
}


template <typename T, typename NoiseFn>
float noise_fractal(int seed, T x, T y, NoiseFn noiseFn, int octaves,
		float amplitude = 1.0, float frequency = 1.0f, float lacunarity = 2.0f, float persistence = 0.5f) {
//	int seed = mSeed;
	float sum = 0;

	for (int i = 0; i < octaves; i++) {
		float noise = noiseFn(seed++, x * frequency, y * frequency);
		sum += noise * amplitude;

		auto mWeightedStrength = 0.f;
		amplitude *= lerp(1.0f, std::min(noise + 1, 2.f) * 0.5f, mWeightedStrength);

//		x *= lacunarity;
//		y *= lacunarity;
		frequency *= lacunarity;
		//mGain == persistence ?
		amplitude *= persistence;
	}

	return sum;
}

} // namespace libv
