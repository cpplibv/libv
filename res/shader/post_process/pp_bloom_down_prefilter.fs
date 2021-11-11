#version 330 core

#include <lib/sample.glsl>

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture0;
uniform float bloomThreshold;
uniform float bloomKnee;

// -------------------------------------------------------------------------------------------------

// Quadratic color thresholding
vec3 prefilter(vec3 color, float threshold, float knee) {
	float brightness = max(color.r, max(color.g, color.b));

	// Under-threshold part: quadratic curve
	float soft = brightness - threshold + knee;
	soft = clamp(soft, 0, 2 * knee);
	soft = soft * soft / (4 * knee + 0.0001);

	// Combine and apply the brightness response curve.
	return color * max(soft, brightness - threshold) / max(brightness, 0.0001);
}

// -------------------------------------------------------------------------------------------------

void main() {
	vec2 pixelSize = 2.0 / textureSize(texture0, 0); // NOTE: 2.0 comes from: We are downsampling so the bound texture is twice as big as we are
	vec3 sample0 = downsampleBox13Balanced(texture0, fragmentTexture0, pixelSize).rgb;
//	vec3 sample0 = downsampleBox13(texture0, fragmentTexture0, pixelSize).rgb;
//	vec3 sample0 = downsampleBox4(texture0, fragmentTexture0, pixelSize).rgb;

	//	const float knee = threshold * threshold_soft; ?

	sample0 = prefilter(sample0, bloomThreshold, bloomKnee);

	result = vec4(sample0, 1);
}
