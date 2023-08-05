#version 330 core

#include <lib/color_spaces.glsl>
#include <lib/gamma.glsl>
#include <lib/resample.glsl>
#include <lib/tonemap.glsl>

// TODO: Drive these defines from C++
// TODO: Runtime change in C++ driven define and shader reload
#define ENABLE_BLOOM
#define ENABLE_VIGNETTE
//#define ENABLE_EXPOSURE
#define ENABLE_TONEMAPPING
//#define ENABLE_TONEMAPPING_WITH_WHITE
//#define ENABLE_GAMMA_CORRECTION
//#define TONEMAP_FN tonemapAcesFilmic

#if defined(ENABLE_TONEMAPPING) && defined(ENABLE_TONEMAPPING_WITH_WHITE)
#error "ENABLE_TONEMAPPING and ENABLE_TONEMAPPING_WITH_WHITE must not be defined at the same time"
#endif

#ifndef TONEMAP_FN
#define TONEMAP_FN tonemapAcesFilmic
// Possible values are:
//	tonemapAcesFilmic
//	tonemapNeutral
//	tonemapAces
//	tonemapReinhard
//	tonemapReinhard2
//	tonemapLottes
//	tonemapFilmic
//	tonemapUchimura
//	tonemapUncharted2
//	tonemapUncharted2Tonemap
//	tonemapUnreal
#endif

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D textureMain;

#ifdef ENABLE_BLOOM
uniform sampler2D textureBloom;
uniform float bloomIntensity;
#endif

#ifdef ENABLE_VIGNETTE
uniform float vignetteIntensity;
uniform float vignetteSmoothness;
uniform float vignetteRoundness;
uniform vec3 vignetteColor;
#endif

#ifdef ENABLE_TONEMAPPING_WITH_WHITE
uniform vec3 whitePoint = vec3(4, 4, 4); // [0 .. inf] White point in the input (what should yield white output after tonemapping)
uniform vec3 whiteCurve = vec3(5, 5, 5); // [0 .. inf] White on the tonemap curve (how much should be used from the tonemap curve)
#endif

// -------------------------------------------------------------------------------------------------

vec3 vignette(vec3 color, vec2 uv, float intensity, float smoothness, float roundness, vec3 vignetteColor) {
	vec2 center = vec2(0.5, 0.5);
	vec2 diff = abs(uv - center) * intensity;
	diff.x *= roundness;
	float vfactor = pow(clamp(1.0 - dot(diff, diff), 0, 1), smoothness);
	return color * mix(vignetteColor, vec3(1.0), vfactor);
}

// -------------------------------------------------------------------------------------------------

void main() {
	vec2 uv = fragmentTexture0;
	vec2 pixelSize = textureSize(textureMain, 0);
	vec3 color = texture(textureMain, uv).rgb;

	// --- Fog ---
	// TODO

	#ifdef ENABLE_BLOOM // --- Bloom ---
	{
		vec3 bloom = upsampleTent9(textureBloom, uv, pixelSize, 2.0).rgb;

		bloom *= bloomIntensity;

		color += bloom;
	}
	#endif

	// --- Bloom Dirt ---
	// TODO

	#ifdef ENABLE_VIGNETTE // --- Vignette ---
	{
		float roundness = mix(pixelSize.x / pixelSize.y, 1.0, vignetteRoundness);
		color = vignette(color, uv, vignetteIntensity, vignetteSmoothness, vignetteRoundness, vignetteColor);
	}
	#endif

	#ifdef ENABLE_EXPOSURE // --- Exposure ---
	{
		// TODO
		// color *= uniforms.exposure;
//		 color *= 2.5;
	}
	#endif

	#ifdef ENABLE_TONEMAPPING // --- Tone Mapping ---
	{
		color = TONEMAP_FN(color);
	}
	#endif

	#ifdef ENABLE_TONEMAPPING_WITH_WHITE // --- Tone Mapping with white point correction ---
	// This is an experimental homebrew idea to have fine grained control over tonemapping whitepoints
	// Math: https://www.desmos.com/calculator/a3nsvtkpxn
	{
		color = TONEMAP_FN(color * whiteCurve / whitePoint) / TONEMAP_FN(whiteCurve);
		color = min(color, vec3(1)); // White balancing pushes curves above 1
	}
	#endif

	#ifdef ENABLE_GAMMA_CORRECTION // --- Gamma correction ---
	{
//		color = rgb_to_srgb(color);
//		color = gamma_correction(color, 2.2);
	}
	#endif

	// --- Result ---
	result = vec4(color, 1);
}
