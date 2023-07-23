#pragma once

#include <lib/noise/__fastnoiselite.glsl>


// --- Value ---------------------------------------------------------------------------------------

float value(uint seed, float x, float y) {
	return _fnlSingleValue2D(int(seed), x, y);
}

float value(uint seed, vec2 coord) {
	return _fnlSingleValue2D(int(seed), coord.x, coord.y);
}

float value(uint seed, float x, float y, float z) {
	return _fnlSingleValue3D(int(seed), x, y, z);
}

float value(uint seed, vec3 coord) {
	return _fnlSingleValue3D(int(seed), coord.x, coord.y, coord.z);
}

// --- Perlin --------------------------------------------------------------------------------------

float perlin(uint seed, float x, float y) {
	return _fnlSinglePerlin2D(int(seed), x, y);
}

float perlin(uint seed, vec2 coord) {
	return _fnlSinglePerlin2D(int(seed), coord.x, coord.y);
}

float perlin(uint seed, float x, float y, float z) {
	return _fnlSinglePerlin3D(int(seed), x, y, z);
}

float perlin(uint seed, vec3 coord) {
	return _fnlSinglePerlin3D(int(seed), coord.x, coord.y, coord.z);
}

// --- Simplex -------------------------------------------------------------------------------------

float simplex(uint seed, float x, float y) {
	// Skewing is done here instead of TransformNoiseCoordinate bc we are direct calling _fnlSingleOpenSimplex2S2D
	const float SQRT3 = float(1.7320508075688772935274463415059);
	const float F2 = 0.5 * (SQRT3 - 1);
	float s = (x + y) * F2;
	x += s;
	y += s;
	// SingleOpenSimplex2S yield a different result in their GLSL and C++ implementation
	// It seams like a bug in the GLSL as the output has artifacts
	// return _fnlSingleOpenSimplex2S2D(int(seed), x, y);
	return _fnlSingleSimplex2D(int(seed), x, y);
}

float simplex(uint seed, vec2 coord) {
	return simplex(seed, coord.x, coord.y);
}

float simplex(uint seed, float x, float y, float z) {
	// Rotating is done here instead of TransformNoiseCoordinate bc we are direct calling _fnlSingleOpenSimplex2S3D
	const float R3 = 2.f / 3.f;
	float r = (x + y + z) * R3; // Rotation, not skew
	x = r - x;
	y = r - y;
	z = r - z;

	// Not using SingleOpenSimplex2S3D to keep 2D and 3D aligned
	// return _fnlSingleOpenSimplex2S3D(int(seed), x, y, z);
	return _fnlSingleOpenSimplex23D(int(seed), x, y, z);
}

float simplex(uint seed, vec3 coord) {
	return simplex(seed, coord.x, coord.y, coord.z);
}

// --- Simplex Gradient ----------------------------------------------------------------------------

vec2 simplex_grad(uint seed, float x, float y) {
	// Skewing is done here instead of TransformNoiseCoordinate bc we are direct calling _fnlSingleOpenSimplex2S2D
	const float SQRT3 = float(1.7320508075688772935274463415059);
	const float F2 = 0.5 * (SQRT3 - 1);
	float s = (x + y) * F2;
	x += s;
	y += s;

	float xr = 0;
	float yr = 0;
	_fnlSingleDomainWarpSimplexGradient(int(seed), x, y, xr, yr, false);
	return vec2(xr, yr);
}

vec2 simplex_grad(uint seed, vec2 coord) {
	return simplex_grad(seed, coord.x, coord.y);
}

vec3 simplex_grad(uint seed, float x, float y, float z) {
	// Rotating is done here instead of TransformNoiseCoordinate bc we are direct calling _fnlSingleOpenSimplex2S3D
	const float R3 = 2.f / 3.f;
	float r = (x + y + z) * R3;// Rotation, not skew
	x = r - x;
	y = r - y;
	z = r - z;

	float xr = 0;
	float yr = 0;
	float zr = 0;
	__fnlSingleDomainWarpOpenSimplex2Gradient(int(seed), x, y, z, xr, yr, zr, false);
	return vec3(xr, yr, zr);
}

vec3 simplex_grad(uint seed, vec3 coord) {
	return simplex_grad(seed, coord.x, coord.y, coord.z);
}

// --- Cellular ------------------------------------------------------------------------------------


#define type_cellular_distance int
#define cellular_distance_euclidean FNL_CELLULAR_DISTANCE_EUCLIDEAN
#define cellular_distance_euclideansq FNL_CELLULAR_DISTANCE_EUCLIDEANSQ
#define cellular_distance_manhattan FNL_CELLULAR_DISTANCE_MANHATTAN
#define cellular_distance_hybrid FNL_CELLULAR_DISTANCE_HYBRID
#define default_cellular_distance cellular_distance_euclidean

#define type_cellular_return int
#define cellular_return_cellvalue FNL_CELLULAR_RETURN_TYPE_CELLVALUE
#define cellular_return_distance FNL_CELLULAR_RETURN_TYPE_DISTANCE
#define cellular_return_distance2 FNL_CELLULAR_RETURN_TYPE_DISTANCE2
#define cellular_return_distance2add FNL_CELLULAR_RETURN_TYPE_DISTANCE2ADD
#define cellular_return_distance2sub FNL_CELLULAR_RETURN_TYPE_DISTANCE2SUB
#define cellular_return_distance2mul FNL_CELLULAR_RETURN_TYPE_DISTANCE2MUL
#define cellular_return_distance2div FNL_CELLULAR_RETURN_TYPE_DISTANCE2DIV
#define default_cellular_return cellular_return_cellvalue

#define default_cellular_jitter 1.0f

float cellular(uint seed, float x, float y,
		type_cellular_distance distanceFn,
		type_cellular_return returnType,
		float jitter) {
	return _fnlSingleCellular2D(int(seed), x, y, distanceFn, returnType, jitter);
}

float cellular(uint seed, float x, float y, float z,
		type_cellular_distance distanceFn,
		type_cellular_return returnType,
		float jitter) {
	return _fnlSingleCellular3D(int(seed), x, y, z, distanceFn, returnType, jitter);
}

float cellular(uint seed, vec2 coord, type_cellular_distance distanceFn, type_cellular_return returnType, float jitter) {
	return cellular(seed, coord.x, coord.y, distanceFn, returnType, jitter);
}

float cellular(uint seed, vec3 coord, type_cellular_distance distanceFn, type_cellular_return returnType, float jitter) {
	return cellular(seed, coord.x, coord.y, coord.z, distanceFn, returnType, jitter);
}

float cellular(uint seed, float x, float y) {
	return cellular(seed, x, y, default_cellular_distance, default_cellular_return, default_cellular_jitter);
}

float cellular(uint seed, float x, float y, float z) {
	return cellular(seed, x, y, z, default_cellular_distance, default_cellular_return, default_cellular_jitter);
}

float cellular(uint seed, vec2 coord) {
	return cellular(seed, coord.x, coord.y, default_cellular_distance, default_cellular_return, default_cellular_jitter);
}

float cellular(uint seed, vec3 coord) {
	return cellular(seed, coord.x, coord.y, coord.z, default_cellular_distance, default_cellular_return, default_cellular_jitter);
}

// --- Fractal -------------------------------------------------------------------------------------

#define _GENERATE_FRACTAL_INDEPENDENT(ReturnType2D, ReturnType3D, NoiseFnName, NoiseFn, AdditionalParam, AdditionalCall) \
		ReturnType2D NoiseFnName (uint seed, float x, float y, int octaves, float amplitude, float frequency, float lacunarity, float persistence AdditionalParam) { \
			ReturnType2D result = ReturnType2D(0); \
            for (int i = 0; i < octaves; ++i) { \
                result += NoiseFn (seed++, x * frequency, y * frequency AdditionalCall) * amplitude; \
                frequency *= lacunarity; \
                amplitude *= persistence; \
            } \
            return result; \
        } \
		ReturnType3D NoiseFnName (uint seed, float x, float y, float z, int octaves, float amplitude, float frequency, float lacunarity, float persistence AdditionalParam) { \
			ReturnType3D result = ReturnType3D(0); \
            for (int i = 0; i < octaves; ++i) { \
                result += NoiseFn (seed++, x * frequency, y * frequency, z * frequency AdditionalCall) * amplitude; \
                frequency *= lacunarity; \
                amplitude *= persistence; \
            } \
            return result; \
        } \
		ReturnType2D NoiseFnName (uint seed, vec2 coord, int octaves, float amplitude, float frequency, float lacunarity, float persistence AdditionalParam) { \
			return NoiseFnName (seed, coord.x, coord.y, octaves, amplitude, frequency, lacunarity, persistence AdditionalCall); \
        } \
		ReturnType3D NoiseFnName (uint seed, vec3 coord, int octaves, float amplitude, float frequency, float lacunarity, float persistence AdditionalParam) { \
			return NoiseFnName (seed, coord.x, coord.y, coord.z, octaves, amplitude, frequency, lacunarity, persistence AdditionalCall); \
        }

#define _GENERATE_FRACTAL_PROGRESSIVE(ReturnType2D, ReturnType3D, NoiseFnName, NoiseFn, AdditionalParam, AdditionalCall) \
		ReturnType2D NoiseFnName (uint seed, float x, float y, int octaves, float amplitude, float frequency, float lacunarity, float persistence AdditionalParam) { \
			ReturnType2D result = ReturnType2D(0); \
            for (int i = 0; i < octaves; ++i) { \
                result += NoiseFn (seed, x * frequency + result.x, y * frequency + result.y AdditionalCall) * amplitude; \
                frequency *= lacunarity; \
                amplitude *= persistence; \
            } \
            return result; \
        } \
		ReturnType3D NoiseFnName (uint seed, float x, float y, float z, int octaves, float amplitude, float frequency, float lacunarity, float persistence AdditionalParam) { \
			ReturnType3D result = ReturnType3D(0); \
            for (int i = 0; i < octaves; ++i) { \
                result += NoiseFn (seed, x * frequency + result.x, y * frequency + result.y, z * frequency + result.z AdditionalCall) * amplitude; \
                frequency *= lacunarity; \
                amplitude *= persistence; \
            } \
            return result; \
        } \
		ReturnType2D NoiseFnName (uint seed, vec2 coord, int octaves, float amplitude, float frequency, float lacunarity, float persistence AdditionalParam) { \
			return NoiseFnName (seed, coord.x, coord.y, octaves, amplitude, frequency, lacunarity, persistence AdditionalCall); \
		} \
		ReturnType3D NoiseFnName (uint seed, vec3 coord, int octaves, float amplitude, float frequency, float lacunarity, float persistence AdditionalParam) { \
			return NoiseFnName (seed, coord.x, coord.y, coord.z, octaves, amplitude, frequency, lacunarity, persistence AdditionalCall); \
		}

#define COMMA ,

_GENERATE_FRACTAL_INDEPENDENT(float, float, fractal_value, value, , )
_GENERATE_FRACTAL_INDEPENDENT(float, float, fractal_perlin, perlin, , )
_GENERATE_FRACTAL_INDEPENDENT(float, float, fractal_simplex, simplex, , )
_GENERATE_FRACTAL_INDEPENDENT(float, float, fractal_cellular, cellular, COMMA type_cellular_distance distanceFn COMMA type_cellular_return returnType COMMA float jitter, COMMA distanceFn COMMA returnType COMMA jitter)
_GENERATE_FRACTAL_INDEPENDENT(vec2, vec3, fractal_simplex_grad_independent, simplex_grad, , )
_GENERATE_FRACTAL_PROGRESSIVE(vec2, vec3, fractal_simplex_grad_progressive, simplex_grad, , )

#undef _GENERATE_FRACTAL_PROGRESSIVE
#undef _GENERATE_FRACTAL_INDEPENDENT
#undef COMMA
