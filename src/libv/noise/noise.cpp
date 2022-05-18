// Created by dbobula on 2/13/2022.

// hpp
#include <libv/noise/noise.hpp>
#include <libv/noise/noise_util.hpp>
// libv
#include <libv/math/vec.hpp>
//
#include <cassert>
#include <cmath>


namespace libv {

// -------------------------------------------------------------------------------------------------

float noise_value(Seed seed, float x, float y) noexcept {
	int x0 = static_cast<int>(std::floor(x));
	int y0 = static_cast<int>(std::floor(y));
//	int x0 = fastFloor(x);
//	int y0 = fastFloor(y);

	float xs = hermiteInterp(x - static_cast<float>(x0));
	float ys = hermiteInterp(y - static_cast<float>(y0));

	x0 *= PrimeX;
	y0 *= PrimeY;
	int x1 = x0 + PrimeX;
	int y1 = y0 + PrimeY;

	float xf0 = lerp(ValCoord(seed, x0, y0), ValCoord(seed, x1, y0), xs);
	float xf1 = lerp(ValCoord(seed, x0, y1), ValCoord(seed, x1, y1), xs);

	return lerp(xf0, xf1, ys);
}

float noise_perlin(Seed seed, float x, float y) noexcept {
	int x0 = static_cast<int>(std::floor(x));
	int y0 = static_cast<int>(std::floor(y));

	auto xd0 = x - static_cast<float>(x0);
	auto yd0 = y - static_cast<float>(y0);
	float xd1 = xd0 - 1;
	float yd1 = yd0 - 1;

	float xs = quinticInterp(xd0);
	float ys = quinticInterp(yd0);

	x0 *= PrimeX;
	y0 *= PrimeY;
	int x1 = x0 + PrimeX;
	int y1 = y0 + PrimeY;

	float xf0 = lerp(gradCoord(seed, x0, y0, xd0, yd0), gradCoord(seed, x1, y0, xd1, yd0), xs);
	float xf1 = lerp(gradCoord(seed, x0, y1, xd0, yd1), gradCoord(seed, x1, y1, xd1, yd1), xs);

	return lerp(xf0, xf1, ys) * 1.4247691104677813f;
}

float noise_simplex(Seed seed, float x, float y) noexcept {
	// 2D OpenSimplex2 case uses the same algorithm as ordinary Simplex.

	const float SQRT3 = 1.7320508075688772935274463415059f;
	const float G2 = (3 - SQRT3) / 6;

	// Skew
	const float F2 = 0.5f * (SQRT3 - 1);
	float s = (x + y) * F2;
	x += s;
	y += s;

	int i = static_cast<int>(std::floor(x));
	int j = static_cast<int>(std::floor(y));
	float xi = x - static_cast<float>(i);
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

	float c = static_cast<float>(2 * (1 - 2 * G2) * (1 / G2 - 2)) * t + (static_cast<float>(-2 * (1 - 2 * G2) * (1 - 2 * G2)) + a);
	if (c <= 0) n2 = 0;
	else {
		float x2 = x0 + (2 * static_cast<float>(G2) - 1);
		float y2 = y0 + (2 * static_cast<float>(G2) - 1);
		n2 = (c * c) * (c * c) * gradCoord(seed, i + PrimeX, j + PrimeY, x2, y2);
	}

	if (y0 > x0) {
		float x1 = x0 + static_cast<float>(G2);
		float y1 = y0 + (static_cast<float>(G2) - 1);
		float b = 0.5f - x1 * x1 - y1 * y1;
		if (b <= 0) n1 = 0;
		else {
			n1 = (b * b) * (b * b) * gradCoord(seed, i, j + PrimeY, x1, y1);
		}
	} else {
		float x1 = x0 + (static_cast<float>(G2) - 1);
		float y1 = y0 + static_cast<float>(G2);
		float b = 0.5f - x1 * x1 - y1 * y1;
		if (b <= 0) n1 = 0;
		else {
			n1 = (b * b) * (b * b) * gradCoord(seed, i + PrimeX, j, x1, y1);
		}
	}

	return (n0 + n1 + n2) * 99.83685446303647f;
}

// -------------------------------------------------------------------------------------------------

// Only baseline for glsl, remove it
float noise_simplex_g(float x, float y) noexcept {
	//  https://github.com/BrianSharpe/Wombat/blob/master/SimplexPerlin2D.glsl

	using vec2 = libv::vec2f;
	using vec3 = libv::vec3f;
	using vec4 = libv::vec4f;

	vec2 P = {x, y};

	//  simplex math constants
	const float SKEWFACTOR = 0.36602540378443864676372317075294f;            // 0.5*(sqrt(3.0)-1.0)
	const float UNSKEWFACTOR = 0.21132486540518711774542560974902f;          // (3.0-sqrt(3.0))/6.0
	const float SIMPLEX_TRI_HEIGHT = 0.70710678118654752440084436210485f;    // sqrt( 0.5 )	height of simplex triangle
	const vec3 SIMPLEX_POINTS = vec3(1.0f - UNSKEWFACTOR, -UNSKEWFACTOR, 1.0f - 2.0f * UNSKEWFACTOR);  //  simplex triangle geo

	//  establish our grid cell.
	P *= SIMPLEX_TRI_HEIGHT;    // scale space so we can have an approx feature size of 1.0
	vec2 Pi = floor(P + dot(P, vec2(SKEWFACTOR)));

	// calculate the hash
	vec4 Pt = vec4(xy(Pi), xy(Pi) + 1.0f);
	Pt = Pt - floor(Pt * (1.0f / 71.0f)) * 71.0f;
	Pt += xyxy(vec2(26.0f, 161.0f));
	Pt *= Pt;
	Pt = xzxz(Pt) * yyww(Pt);
	vec4 hash_x = fract(Pt * (1.0f / 951.135664f));
	vec4 hash_y = fract(Pt * (1.0f / 642.949883f));

	//  establish vectors to the 3 corners of our simplex triangle
	vec2 v0 = Pi - dot(Pi, vec2(UNSKEWFACTOR)) - P;
	vec4 v1pos_v1hash = (v0.x < v0.y) ? vec4(xy(SIMPLEX_POINTS), hash_x.y, hash_y.y) : vec4(yx(SIMPLEX_POINTS), hash_x.z, hash_y.z);
	vec4 v12 = vec4(xy(v1pos_v1hash), zz(SIMPLEX_POINTS)) + xyxy(v0);

	//  calculate the dotproduct of our 3 corner vectors with 3 random normalized vectors
	vec3 grad_x = vec3(hash_x.x, v1pos_v1hash.z, hash_x.w) - 0.49999f;
	vec3 grad_y = vec3(hash_y.x, v1pos_v1hash.w, hash_y.w) - 0.49999f;
	vec3 grad_results = inversesqrt(grad_x * grad_x + grad_y * grad_y) * (grad_x * vec3(v0.x, xz(v12)) + grad_y * vec3(v0.y, yw(v12)));

	//	Normalization factor to scale the final result to a strict 1.0->-1.0 range
	//	http://briansharpe.wordpress.com/2012/01/13/simplex-noise/#comment-36
	const float FINAL_NORMALIZATION = 99.204334582718712976990005025589f;

	//	evaluate and return
	vec3 m = vec3(v0.x, xz(v12)) * vec3(v0.x, xz(v12)) + vec3(v0.y, yw(v12)) * vec3(v0.y, yw(v12));
	m = max(0.5f - m, 0.0f);
	m = m * m;
	return dot(m * m, grad_results) * FINAL_NORMALIZATION;
}

float noise_simplex_glsl(float x, float y) noexcept {
	//  https://github.com/BrianSharpe/Wombat/blob/master/SimplexPerlin2D.glsl

	using vec2 = libv::vec2f;
	using vec3 = libv::vec3f;
	using vec4 = libv::vec4f;

	//  simplex math constants
	constexpr const float SKEWFACTOR = 0.36602540378443864676372317075294f;            // 0.5*(sqrt(3.0)-1.0)
	constexpr const float UNSKEWFACTOR = 0.21132486540518711774542560974902f;          // (3.0-sqrt(3.0))/6.0
	constexpr const float SIMPLEX_TRI_HEIGHT = 0.70710678118654752440084436210485f;    // sqrt( 0.5 )	height of simplex triangle
	constexpr const vec3 SIMPLEX_POINTS = vec3(1.0f - UNSKEWFACTOR, -UNSKEWFACTOR, 1.0f - 2.0f * UNSKEWFACTOR);  //  simplex triangle geo

	//  establish our grid cell.
	vec2 P = {x * SIMPLEX_TRI_HEIGHT, y * SIMPLEX_TRI_HEIGHT}; // scale space so we can have an approx feature size of 1.0
	vec2 Pi = floor(P + dot(P, vec2(SKEWFACTOR)));

	// calculate the hash
	vec4 Pt = vec4(Pi.x, Pi.y, Pi.x + 1.0f, Pi.y + 1.0f);
	Pt = Pt - floor(Pt * (1.0f / 71.0f)) * 71.0f;
	Pt += vec4(26.0f, 161.0f, 26.0f, 161.0f);
	Pt *= Pt;
	Pt = vec4(Pt.x * Pt.y, Pt.z * Pt.y, Pt.x * Pt.w, Pt.z * Pt.w);
	vec4 hash_x = fract(Pt * (1.0f / 951.135664f));
	vec4 hash_y = fract(Pt * (1.0f / 642.949883f));

	//  establish vectors to the 3 corners of our simplex triangle
	vec2 v0 = Pi - dot(Pi, vec2(UNSKEWFACTOR)) - P;
	vec4 v1pos_v1hash = (v0.x < v0.y) ?
			vec4(SIMPLEX_POINTS.x, SIMPLEX_POINTS.y, hash_x.y, hash_y.y) :
			vec4(SIMPLEX_POINTS.y, SIMPLEX_POINTS.x, hash_x.z, hash_y.z);
	vec4 v12 = vec4(v1pos_v1hash.x + v0.x, v1pos_v1hash.y + v0.y, SIMPLEX_POINTS.z + v0.x, SIMPLEX_POINTS.z + v0.y);

	//  calculate the dotproduct of our 3 corner vectors with 3 random normalized vectors
	vec3 grad_x = vec3(hash_x.x, v1pos_v1hash.z, hash_x.w) - 0.49999f;
	vec3 grad_y = vec3(hash_y.x, v1pos_v1hash.w, hash_y.w) - 0.49999f;
	vec3 grad_results = inversesqrt(grad_x * grad_x + grad_y * grad_y) * (grad_x * vec3(v0.x, v12.x, v12.z) + grad_y * vec3(v0.y, v12.y, v12.w));

	//	Normalization factor to scale the final result to a strict 1.0->-1.0 range
	//	http://briansharpe.wordpress.com/2012/01/13/simplex-noise/#comment-36
	const float FINAL_NORMALIZATION = 99.204334582718712976990005025589f;

	//	evaluate and return
	vec3 m = vec3(v0.x, v12.x, v12.z) * vec3(v0.x, v12.x, v12.z) + vec3(v0.y, v12.y, v12.w) * vec3(v0.y, v12.y, v12.w);
	m = max(0.5f - m, 0.0f);
	m = m * m;
	return dot(m * m, grad_results) * FINAL_NORMALIZATION;
}

// -------------------------------------------------------------------------------------------------
namespace detail { // ------------------------------------------------------------------------------

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
			T vecX = static_cast<T>(xi) - x + Lookup<T>::RandVecs2D[idx] * cellularJitter;
			T vecY = static_cast<T>(yi) - y + Lookup<T>::RandVecs2D[idx | 1] * cellularJitter;
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
		return static_cast<float>(closestHash) * (1.f / 2147483648.0f);
	case CellularReturnType::distance:
		return distance0 - 1.f;
	case CellularReturnType::distance2:
		return distance1 - 1.f;
	case CellularReturnType::distance2Add:
		return (distance1 + distance0) * 0.5f - 1.f;
	case CellularReturnType::distance2Sub:
		return distance1 - distance0 - 1.f;
	case CellularReturnType::distance2Mul:
		return distance1 * distance0 * 0.5f - 1.f;
	case CellularReturnType::distance2Div:
		return distance0 / distance1 - 1.f;
	default:
		return 0.f;
	}
}

} // namespace detail ------------------------------------------------------------------------------

float noise_cellular(Seed seed, float x, float y,
		CellularDistanceFunction distanceFn,
		CellularReturnType returnType,
		float jitter) noexcept {

	switch (distanceFn) {
	case CellularDistanceFunction::euclidean:
	case CellularDistanceFunction::euclideanSq:
		return detail::aux_noise_cellular(seed, x, y, [](const auto& x, const auto& y) {
			return x * x + y * y;
		}, distanceFn, returnType, jitter);
	case CellularDistanceFunction::manhattan:
		return detail::aux_noise_cellular(seed, x, y, [](const auto& x, const auto& y) {
			return std::abs(x) + std::abs(y);
		}, distanceFn, returnType, jitter);

	case CellularDistanceFunction::hybrid:
		return detail::aux_noise_cellular(seed, x, y, [](const auto& x, const auto& y) {
			return std::abs(x) + std::abs(y) + x * x + y * y;
		}, distanceFn, returnType, jitter);
	}

	assert(false && "Invalid CellularDistanceFunction enum value");
	return 0;
}

// --- Warp ----------------------------------------------------------------------------------------

libv::vec2f singleWarpSimplexGradient(Seed seed, float x, float y, float warpAmp, float frequency, bool outGradOnly) {
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
			gradCoordOut(seed, i, j, xo, yo);
		else
			gradCoordDual(seed, i, j, x0, y0, xo, yo);
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
			gradCoordOut(seed, i + PrimeX, j + PrimeY, xo, yo);
		else
			gradCoordDual(seed, i + PrimeX, j + PrimeY, x2, y2, xo, yo);
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
				gradCoordOut(seed, i, j + PrimeY, xo, yo);
			else
				gradCoordDual(seed, i, j + PrimeY, x1, y1, xo, yo);
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
				gradCoordOut(seed, i + PrimeX, j, xo, yo);
			else
				gradCoordDual(seed, i + PrimeX, j, x1, y1, xo, yo);
			vx += bbbb * xo;
			vy += bbbb * yo;
		}
	}

	return {vx * warpAmp, vy * warpAmp};
}

libv::vec2f doSingleWarp(Seed seed, float x, float y, WarpType type, float amp, float freq) {
	switch (type) {
	case WarpType::simplex2:
		return singleWarpSimplexGradient(seed, x, y, amp * 38.283687591552734375f, freq, false);

	case WarpType::simplex2_reduced:
//		singleWarpSimplexGradient(seed, amp * 16.0f, freq, x, y, xr, yr, true);
		break;

	case WarpType::basic_grid:
//		singleDomainWarpBasicGrid(seed, amp, freq, x, y, xr, yr);
		break;
	}

	assert(false && "Invalid WarpType enum value");
	return {0, 0};
}

/// Domain Warp Coordinate Transforms
libv::vec2f transformWarpCoordinate(float x, float y, WarpType type) {
	if (type == WarpType::simplex2 || type == WarpType::simplex2_reduced) {
		const float SQRT3 = 1.7320508075688772935274463415059f;
		const float F2 = 0.5f * (SQRT3 - 1);
		float t = (x + y) * F2;
		x += t;
		y += t;
	}

	return libv::vec2f{x, y};
}

libv::vec2f warpFractalIndependent(Seed seed, float x, float y,
		int octaves, float amplitude, float frequency, float lacunarity, float persistence, WarpType type) {

	const auto [xs, ys] = transformWarpCoordinate(x, y, type);

//	Seed seed = mSeed;
//	auto domainWarpAmp = 1.0f;
//	auto fractalBounding = 1.f / 1.75f;
//	float amp = domainWarpAmp * fractalBounding;
//	float freq = mFrequency;

	libv::vec2f result(x, y);

	for (int i = 0; i < octaves; i++) {
		result += doSingleWarp(seed, xs, ys, type, amplitude, frequency);

		seed++;
		amplitude *= persistence;
		frequency *= lacunarity;
	}

	return result;
}

libv::vec2f warp(Seed seed, float x, float y, WarpFractalType type,
		int octaves, float amplitude, float frequency, float lacunarity, float persistence) {
	switch (type) {
//	case WarpFractalType::none:
//		DomainWarpSingle(x, y);
//		break;

//	case WarpFractalType::progressive:
//		DomainWarpFractalProgressive(x, y);
//		break;

	case WarpFractalType::independent:
		return warpFractalIndependent(seed, x, y, octaves, amplitude, frequency, lacunarity, persistence);
	}

	assert(false && "Invalid WarpFractalType enum value");
	return {0, 0};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
