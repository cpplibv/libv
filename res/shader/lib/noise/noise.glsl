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

vec2 simplex_gradient(uint seed, float x, float y) {
	// Skewing is done here instead of TransformNoiseCoordinate bc we are direct calling _fnlSingleOpenSimplex2S2D
	const float SQRT3 = float(1.7320508075688772935274463415059);
	const float F2 = 0.5 * (SQRT3 - 1);
	float s = (x + y) * F2;
	x += s;
	y += s;

	float xr = 0;
	float yr = 0;
	_fnlSingleDomainWarpSimplexGradientB(int(seed), x, y, xr, yr, false);
	return vec2(xr, yr);
}

vec2 simplex_gradient(uint seed, vec2 coord) {
	return simplex_gradient(seed, coord.x, coord.y);
}

vec3 simplex_gradient(uint seed, float x, float y, float z) {
	// Rotating is done here instead of TransformNoiseCoordinate bc we are direct calling _fnlSingleOpenSimplex2S3D
	const float R3 = 2.f / 3.f;
	float r = (x + y + z) * R3;// Rotation, not skew
	x = r - x;
	y = r - y;
	z = r - z;

	float xr = 0;
	float yr = 0;
	float zr = 0;
	_fnlSingleDomainWarpOpenSimplex2GradientB(int(seed), x, y, z, xr, yr, zr, false);
	return vec3(xr, yr, zr);
}

vec3 simplex_gradient(uint seed, vec3 coord) {
	return simplex_gradient(seed, coord.x, coord.y, coord.z);
}

// --- Cellular ------------------------------------------------------------------------------------

#define libv_cellular_distance_func int
#define libv_cellular_return_type int

float cellular(uint seed, float x, float y,
		libv_cellular_distance_func distanceFn,
		libv_cellular_return_type returnType,
		float jitter) {
	fnl_state state = fnlCreateState(int(seed));
	state.cellular_distance_func = distanceFn;
	state.cellular_return_type = returnType;
	state.cellular_jitter_mod = jitter;
	return _fnlSingleCellular2D(state, int(seed), x, y);
}

float cellular(uint seed, float x, float y, float z,
		libv_cellular_distance_func distanceFn,
		libv_cellular_return_type returnType,
		float jitter) {
	fnl_state state = fnlCreateState(int(seed));
	state.cellular_distance_func = distanceFn;
	state.cellular_return_type = returnType;
	state.cellular_jitter_mod = jitter;
	return _fnlSingleCellular3D(state, int(seed), x, y, z);
}

float cellular(uint seed, vec2 coord, libv_cellular_distance_func distanceFn, libv_cellular_return_type returnType, float jitter) {
	return cellular(seed, coord.x, coord.y, distanceFn, returnType, jitter);
}

float cellular(uint seed, vec3 coord, libv_cellular_distance_func distanceFn, libv_cellular_return_type returnType, float jitter) {
	return cellular(seed, coord.x, coord.y, coord.z, distanceFn, returnType, jitter);
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
_GENERATE_FRACTAL_INDEPENDENT(float, float, fractal_cellular, cellular, COMMA libv_cellular_distance_func distanceFn COMMA libv_cellular_return_type returnType COMMA float jitter, COMMA distanceFn COMMA returnType COMMA jitter)
_GENERATE_FRACTAL_INDEPENDENT(vec2, vec3, fractal_simplex_gradient_independent, simplex_gradient, , )
_GENERATE_FRACTAL_PROGRESSIVE(vec2, vec3, fractal_simplex_gradient_progressive, simplex_gradient, , )

#undef _GENERATE_FRACTAL_PROGRESSIVE
#undef _GENERATE_FRACTAL_INDEPENDENT
#undef COMMA
