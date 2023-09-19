#version 420 core

#include <shader/lib/color_spaces.glsl>
#include <shader/lib/gamma.glsl>
#include <shader/lib/resample.glsl>
#include <shader/lib/tonemap.glsl>
#include <shader/re/post_process/pp_block.glsl>


#if defined(ENABLE_BLOOM_DIRT) && !defined(ENABLE_BLOOM)
#error "ENABLE_BLOOM_DIRT requires ENABLE_BLOOM"
#endif

#if defined(ENABLE_VIGNETTE_TEXTURE) && !defined(ENABLE_VIGNETTE)
#error "ENABLE_VIGNETTE_TEXTURE requires ENABLE_VIGNETTE"
#endif

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

in vec2 fragmentUV;

out vec4 outputColor;

layout(binding = TEXTURE_UNIT_PP_SCENE) uniform sampler2D textureScene;

#ifdef ENABLE_BLOOM
layout(binding = TEXTURE_UNIT_PP_BLOOM_PYRAMID) uniform sampler2D textureBloomPyramid;
#endif

#ifdef ENABLE_BLOOM_DIRT
layout(binding = TEXTURE_UNIT_PP_BLOOM_DIRT) uniform sampler2D textureBloomDirt;
#endif

#ifdef ENABLE_VIGNETTE_TEXTURE
layout(binding = TEXTURE_UNIT_PP_VIGNETTE) uniform sampler2D textureVignette;
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
	vec2 uv = fragmentUV;
	vec2 pixelSize = textureSize(textureScene, 0);
	vec3 color = texture(textureScene, uv).rgb;

	#ifdef ENABLE_BLOOM // --- Bloom ---
	{
		vec2 bloomPixelSize = 1.0 / textureSize(textureBloomPyramid, 0).xy;
		vec2 bloomUV = (bloomBoundingPosition + fragmentUV * bloomBoundingSize + 0.5) * bloomPixelSize;
		// I have no clue why the +0.5 bloom pixel is needed for the UV, but it is actually required
		vec3 bloomSample = upsampleTent9Lod(textureBloomPyramid, 0, bloomUV, bloomPixelSize, 1.0).rgb;
		vec3 bloom = bloomFinalMultiplier * bloomSample;
		color.xyz = mix(color.xyz, bloom, bloomCombineMix) + bloom * bloomCombineAdd;
	}
	#endif

	#ifdef ENABLE_BLOOM_DIRT // --- Bloom Dirt ---
	{
		// TODO
	}
	#endif

	#ifdef ENABLE_VIGNETTE // --- Vignette ---
	{
		float roundness = mix(pixelSize.x / pixelSize.y, 1.0, vignetteRoundness);
		color = vignette(color, uv, vignetteIntensity, vignetteSmoothness, vignetteRoundness, vignetteColor);
	}
	#endif

	#ifdef ENABLE_VIGNETTE_TEXTURE // --- Vignette Texture ---
	{
		// TODO
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
		const float gamma = 2.2;
		color = gamma_correction(color, gamma);
	}
	#endif

	// --- Result ---
	outputColor = vec4(color, 1);
}
