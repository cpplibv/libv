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
// --- Cellular ------------------------------------------------------------------------------------

// Structure containing entire noise system state.
// @note Must only be created using fnlCreateState(optional: seed). To ensure defaults are set.

//struct fnl_state
//{
//    // Seed used for all noise types.
//    // @remark Default: 1337
//    int seed;
//
//    // The frequency for all noise types.
//    // @remark Default: 0.01
//    float frequency;
//
//    // The noise algorithm to be used by GetNoise(...).
//    // @remark Default: FNL_NOISE_OPENSIMPLEX2
//    fnl_noise_type noise_type;
//
//    // Sets noise rotation type for 3D.
//    // @remark Default: FNL_ROTATION_NONE
//    fnl_rotation_type_3d rotation_type_3d;
//
//    // The method used for combining octaves for all fractal noise types.
//    // @remark Default: None
//    // @remark FNL_FRACTAL_DOMAIN_WARP_... only effects fnlDomainWarp...
//    fnl_fractal_type fractal_type;
//
//    // The octave count for all fractal noise types.
//    // @remark Default: 3
//    int octaves;
//
//    // The octave lacunarity for all fractal noise types.
//    // @remark Default: 2.0
//    float lacunarity;
//
//    // The octave gain for all fractal noise types.
//    // @remark Default: 0.5
//    float gain;
//
//    // The octave weighting for all none Domaain Warp fractal types.
//    // @remark Default: 0.0
//    // @remark
//    float weighted_strength;
//
//    // The strength of the fractal ping pong effect.
//    // @remark Default: 2.0
//    float ping_pong_strength;
//
//    // The distance function used in cellular noise calculations.
//    // @remark Default: FNL_CELLULAR_FUNC_DISTANCE
//    fnl_cellular_distance_func cellular_distance_func;
//
//    // The cellular return type from cellular noise calculations.
//    // @remark Default: FNL_CELLULAR_RETURN_TYPE_EUCLIEANSQ
//    fnl_cellular_return_type cellular_return_type;
//
//    // The maximum distance a cellular point can move from it's grid position.
//    // @remark Default: 1.0
//    // @note Setting this higher than 1 will cause artifacts.
//    float cellular_jitter_mod;
//
//    // The warp algorithm when using fnlDomainWarp...
//    // @remark Default: OpenSimplex2
//    fnl_domain_warp_type domain_warp_type;
//
//    // The maximum warp distance from original position when using fnlDomainWarp...
//    // @remark Default: 1.0
//    float domain_warp_amp;
//};

// -------------------------------------------------------------------------------------------------

#define libv_cellular_distance_func int
#define libv_cellular_return_type int
//#define fnl_state struct

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

// -------------------------------------------------------------------------------------------------

// TODO: libv.glsl: fractal, warp,
