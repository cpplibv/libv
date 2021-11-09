#version 330 core

#include <bloom/color_spaces.glsl>
#include <bloom/sample.glsl>
#include <bloom/tone_mapping.glsl>

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D textureMain;
uniform sampler2D textureBloom;

uniform float bloomIntensity;

uniform float vignetteIntensity;
uniform float vignetteSmoothness;
uniform float vignetteRoundness;
uniform vec3 vignetteColor;

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

	// --- Bloom ---
	{
		vec3 bloom = upsampleTent9(textureBloom, uv, pixelSize, 2.0).rgb;

		bloom *= bloomIntensity;

		color += bloom;
	}

	// --- Vignette ---
	{
		float roundness = mix(pixelSize.x / pixelSize.y, 1.0, vignetteRoundness);
		color = vignette(color, uv, vignetteIntensity, vignetteSmoothness, vignetteRoundness, vignetteColor);
	}

	//	color *= postExposure;

	// --- Tone Mapping ---
	{
		color = tonemapNeutral(color);
//		color = tonemapAces(color);
//		color = tonemapReinhard(color);
//		color = tonemapReinhard2(color);
//		color = tonemapLottes(color);
//		color = tonemapFilmic(color);
//		color = tonemapUchimura(color);
//		color = tonemapUncharted2(color);
//		color = tonemapUncharted2Tonemap(color);
//		color = tonemapUnreal(color);
	}

	// --- Gamma correction ---
	{
//		color = rgb_to_srgb(color);
//		color = gamma_correction(color, 2.2);
	}

	// --- Result ---
	result = vec4(color, 1);
}
