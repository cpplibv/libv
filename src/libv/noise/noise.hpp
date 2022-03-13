// Created by dbobula on 2/12/2022.

#include <cmath>
#include <libv/noise/noise_util.hpp>

#include <stdexcept>


#pragma once


namespace libv {
using Seed = uint32_t;

//float, double, libv::vec4f, libv::vec4d
template <typename T>
[[nodiscard]] T noise_value(Seed seed, T x, T y) noexcept {
	int x0 = static_cast<int>(std::floor(x));
	int y0 = static_cast<int>(std::floor(y));
//	int x0 = fastFloor(x);
//	int y0 = fastFloor(y);

	T xs = hermiteInterp(x - static_cast<T>(x0));
	T ys = hermiteInterp(y - static_cast<T>(y0));

	x0 *= PrimeX;
	y0 *= PrimeY;
	int x1 = x0 + PrimeX;
	int y1 = y0 + PrimeY;

	T xf0 = lerp(ValCoord(seed, x0, y0), ValCoord(seed, x1, y0), xs);
	T xf1 = lerp(ValCoord(seed, x0, y1), ValCoord(seed, x1, y1), xs);

	return lerp(xf0, xf1, ys);
}

[[nodiscard]] float noise_value(Seed seed, float x, float y, float z) noexcept;

//float, double, libv::vec4f, libv::vec4d
template <typename T>
[[nodiscard]] float noise_perlin(Seed seed, T x, T y) noexcept {
	int x0 = static_cast<int>(std::floor(x));
	int y0 = static_cast<int>(std::floor(y));

	auto xd0 = x - static_cast<T>(x0);
	auto yd0 = y - static_cast<T>(y0);
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
[[nodiscard]] float noise_simplex(Seed seed, T x, T y) noexcept {
	// 2D OpenSimplex2 case uses the same algorithm as ordinary Simplex.

	const float SQRT3 = 1.7320508075688772935274463415059f;
	const float G2 = (3 - SQRT3) / 6;

	/*
	 * --- Skew moved to TransformNoiseCoordinate method ---
	 * const FNfloat F2 = 0.5f * (SQRT3 - 1);
	 * FNfloat s = (x + y) * F2;
	 * x += s; y += s;
	*/

	//x = x-y;
	x = x + y * 0.5f;//!!!
//	y = y + x*0.5f;
	//y = y+x;

	int i = static_cast<int>(std::floor(x));
	int j = static_cast<int>(std::floor(y));
	float xi =  x - static_cast<float>(i);
	float yi = y - static_cast<float>(j);

	float t = (xi + yi) * G2;
	float x0 = xi - static_cast<float>(t);
	float y0 = yi - static_cast<float>(t);

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

//template <typename T>
//[[nodiscard]] float noise_simplex2(Seed seed, T x, T y) noexcept {
//	const float SQRT3 = 1.7320508075688772935274463415059f;
//	const float F2 = 0.5f * (SQRT3 - 1.0f);
//	const float G2 = (3.0f - SQRT3) / 6.0f;
//
//	float f = float( F2 ) * (x + y);
//	float x0 = FS_Floor_f32( x + f );
//	float y0 = FS_Floor_f32( y + f );
//
//	int i = FS_Convertf32_i32( x0 ) * int( FnPrimes::X );
//	int j = FS_Convertf32_i32( y0 ) * int( FnPrimes::Y );
//
//	float g = float( G2 ) * (x0 + y0);
//	x0 = x - (x0 - g);
//	y0 = y - (y0 - g);
//
//	mask32v i1 = x0 > y0;
//	//mask32v j1 = ~i1; //NMasked funcs
//
//	float x1 = FS_MaskedSub_f32( x0, float( 1.f ), i1 ) + float( G2 );
//	float y1 = FS_NMaskedSub_f32( y0, float( 1.f ), i1 ) + float( G2 );
//
//	float x2 = x0 + float( G2 * 2 - 1 );
//	float y2 = y0 + float( G2 * 2 - 1 );
//
//	float t0 = FS_FNMulAdd_f32( x0, x0, FS_FNMulAdd_f32( y0, y0, float( 0.5f ) ) );
//	float t1 = FS_FNMulAdd_f32( x1, x1, FS_FNMulAdd_f32( y1, y1, float( 0.5f ) ) );
//	float t2 = FS_FNMulAdd_f32( x2, x2, FS_FNMulAdd_f32( y2, y2, float( 0.5f ) ) );
//
//	t0 = FS_Max_f32( t0, float( 0 ) );
//	t1 = FS_Max_f32( t1, float( 0 ) );
//	t2 = FS_Max_f32( t2, float( 0 ) );
//
//	t0 *= t0; t0 *= t0;
//	t1 *= t1; t1 *= t1;
//	t2 *= t2; t2 *= t2;
//
//	float n0 = FnUtils::GetGradientDot( FnUtils::HashPrimes( seed, i, j ), x0, y0 );
//	float n1 = FnUtils::GetGradientDot( FnUtils::HashPrimes( seed, FS_MaskedAdd_i32( i, int( FnPrimes::X ), i1 ), FS_NMaskedAdd_i32( j, int( FnPrimes::Y ), i1 ) ), x1, y1 );
//	float n2 = FnUtils::GetGradientDot( FnUtils::HashPrimes( seed, i + int( FnPrimes::X ), j + int( FnPrimes::Y ) ), x2, y2 );
//
//	return float( 38.283687591552734375f ) * FS_FMulAdd_f32( n0, t0, FS_FMulAdd_f32( n1, t1, n2 * t2 ) );
//}

template <typename T>
[[nodiscard]] float noise_simplex2S(Seed seed, T x, T y) {
	// 2D OpenSimplex2S case is a modified 2D simplex noise.

	const T SQRT3 = static_cast<T> (1.7320508075688772935274463415059f);
	const T G2 = (T{3} - SQRT3) / T{6};

	/*
	 * --- Skew moved to TransformNoiseCoordinate method ---
	 * const FNfloat F2 = 0.5f * (SQRT3 - 1);
	 * FNfloat s = (x + y) * F2;
	 * x += s; y += s;
	*/

	int i = static_cast<int>(std::floor(x));
	int j = static_cast<int>(std::floor(y));
	float xi = x - static_cast<float>(i);
	float yi = y - static_cast<float>(j);

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
[[nodiscard]] T aux_noise_cellular(Seed seed, T x, T y,
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
[[nodiscard]] T noise_cellular(Seed seed, T x, T y,
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
	throw std::runtime_error("Distance function unknown: {}" ); //TODO error msg;
}


enum class FractalType {
	none,
	fbm,
	ridged,
	pingpong
};

//fmb(Fractional Brownian motion)
template <typename T, typename NoiseFn>
[[nodiscard]] float noise_fractal(Seed seed, T x, T y, NoiseFn noiseFn, int octaves,
		float amplitude = 1.0, float frequency = 1.0f, float lacunarity = 2.0f, float persistence = 0.5f) {
//	Seed seed = mSeed;
	float sum = 0;

	for (int i = 0; i < octaves; i++) {
		float noise = noiseFn(seed++, x * frequency, y * frequency);
		sum += noise * amplitude;

		auto weightedStrength = 0.f;
		amplitude *= lerp(1.0f, std::min(noise + 1, 2.f) * 0.5f, weightedStrength);

//		x *= lacunarity;
//		y *= lacunarity;
		frequency *= lacunarity;
		//mGain == persistence ?
		amplitude *= persistence;
	}

	return sum;
}

// -------------------------------------------------------------------------------------------------
// warp

constexpr inline void singleWarpSimplexGradient(Seed seed, float x, float y, float& xr, float& yr, float warpAmp, float frequency, bool outGradOnly) {
	const float SQRT3 = 1.7320508075688772935274463415059f;
	const float G2 = (3 - SQRT3) / 6;

	x *= frequency;
	y *= frequency;

	/*
	 * --- Skew moved to TransformNoiseCoordinate method ---
	 * const float F2 = 0.5f * (SQRT3 - 1);
	 * float s = (x + y) * F2;
	 * x += s; y += s;
	*/

	int i = static_cast<int>(std::floor(x));
	int j = static_cast<int>(std::floor(y));

	float xi = x - static_cast<float>(i);
	float yi = y - static_cast<float>(j);

	float t = (xi + yi) * G2;
	float x0 = xi - static_cast<float>(t);
	float y0 = yi - static_cast<float>(t);

	i *= PrimeX;
	j *= PrimeY;

	float vx, vy;
	vx = vy = 0;

	float a = 0.5f - x0 * x0 - y0 * y0;
	if (a > 0) {
		float aaaa = (a * a) * (a * a);
		float xo, yo;
		if (outGradOnly)
			GradCoordOut(seed, i, j, xo, yo);
		else
			GradCoordDual(seed, i, j, x0, y0, xo, yo);
		vx += aaaa * xo;
		vy += aaaa * yo;
	}

	float c = static_cast<float> (2 * (1 - 2 * G2) * (1 / G2 - 2)) * t + (static_cast<float> (-2 * (1 - 2 * G2) * (1 - 2 * G2)) + a);
	if (c > 0) {
		float x2 = x0 + (2 * static_cast<float> (G2) - 1);
		float y2 = y0 + (2 * static_cast<float> (G2) - 1);
		float cccc = (c * c) * (c * c);
		float xo, yo;
		if (outGradOnly)
			GradCoordOut(seed, i + PrimeX, j + PrimeY, xo, yo);
		else
			GradCoordDual(seed, i + PrimeX, j + PrimeY, x2, y2, xo, yo);
		vx += cccc * xo;
		vy += cccc * yo;
	}

	if (y0 > x0) {
		float x1 = x0 + static_cast<float> (G2);
		float y1 = y0 + (static_cast<float> (G2) - 1);
		float b = 0.5f - x1 * x1 - y1 * y1;
		if (b > 0) {
			float bbbb = (b * b) * (b * b);
			float xo, yo;
			if (outGradOnly)
				GradCoordOut(seed, i, j + PrimeY, xo, yo);
			else
				GradCoordDual(seed, i, j + PrimeY, x1, y1, xo, yo);
			vx += bbbb * xo;
			vy += bbbb * yo;
		}
	} else {
		float x1 = x0 + (static_cast<float> (G2) - 1);
		float y1 = y0 + static_cast<float> (G2);
		float b = 0.5f - x1 * x1 - y1 * y1;
		if (b > 0) {
			float bbbb = (b * b) * (b * b);
			float xo, yo;
			if (outGradOnly)
				GradCoordOut(seed, i + PrimeX, j, xo, yo);
			else
				GradCoordDual(seed, i + PrimeX, j, x1, y1, xo, yo);
			vx += bbbb * xo;
			vy += bbbb * yo;
		}
	}

	xr += vx * warpAmp;
	yr += vy * warpAmp;
}

enum class WarpType {
	simplex2,
	simplex2_reduced,
	basic_grid
};

// Domain Warp
constexpr inline void doSingleWarp(Seed seed, float x, float y, float& xr, float& yr, WarpType type, float amp, float freq) {
	switch (type) {
	case WarpType::simplex2:
		singleWarpSimplexGradient(seed, x, y, xr, yr, amp * 38.283687591552734375f, freq, false);
		break;
	case WarpType::simplex2_reduced:
		//TODO
//		singleWarpSimplexGradient(seed, amp * 16.0f, freq, x, y, xr, yr, true);
		break;
	case WarpType::basic_grid:
		//TODO
//		SingleDomainWarpBasicGrid(seed, amp, freq, x, y, xr, yr);
		break;
	}
}

// Domain Warp Coordinate Transforms
constexpr inline void transformWarpCoordinate(float& x, float& y, WarpType type) {
	if (type == WarpType::simplex2 || type == WarpType::simplex2_reduced) {
		const float SQRT3 = 1.7320508075688772935274463415059f;
		const float F2 = 0.5f * (SQRT3 - 1);
		float t = (x + y) * F2;
		x += t;
		y += t;
	}
}

// Domain Warp Fractal Independent
constexpr inline void warpFractalIndependent(Seed seed, float& x, float& y,
		float amplitude = 1.0, float frequency = 0.01f,
		int octaves = 5, float lacunarity = 2.0f, float persistence = 0.5f, WarpType type = WarpType::simplex2) {
	float xs = x;
	float ys = y;
	transformWarpCoordinate(xs, ys, type);

//	Seed seed = mSeed;
//	auto domainWarpAmp = 1.0f;
//	auto fractalBounding = 1.f / 1.75f;
//	float amp = domainWarpAmp * fractalBounding;
//	float freq = mFrequency;

	for (int i = 0; i < octaves; i++) {
		doSingleWarp(seed, xs, ys, x, y, type, amplitude, frequency);

		seed++;
		amplitude *= persistence;
		frequency *= lacunarity;
	}
}


enum class WarpFractalType {
	none,
	progressive,
	independent
};


/// <summary>
/// 2D warps the input position using current domain warp settings
/// </summary>
/// <example>
/// Example usage with GetNoise
/// <code>DomainWarp(x, y)
/// noise = GetNoise(x, y)</code>
/// </example>
constexpr inline void warp(Seed seed, float& x, float& y, WarpFractalType type,
		float amplitude = 1.0, float frequency = 0.01f,
		int octaves = 5, float lacunarity = 2.0f, float persistence = 0.5f) {
	switch (type) {
	case WarpFractalType::none:
		//TODO
//		DomainWarpSingle(x, y);
		break;
	case WarpFractalType::progressive:
		//TODO
//		DomainWarpFractalProgressive(x, y);
		break;
	case WarpFractalType::independent:
		warpFractalIndependent(seed, x, y, amplitude, frequency, octaves, lacunarity, persistence);
		break;
	}
}

constexpr inline void warpWithIndependentFractal(Seed seed, float& x, float& y,
		float amplitude = 1.0, float frequency = 0.01f,
		int octaves = 5, float lacunarity = 2.0f, float persistence = 0.5f) {
	warp(seed, x, y, WarpFractalType::independent,
			amplitude, frequency, octaves, lacunarity, persistence);
}


//template <typename T>
//float noise_warp(Seed seed, T x, T y) {
//
//	return 0;
//}

} // namespace libv
