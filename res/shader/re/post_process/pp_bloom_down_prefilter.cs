#version 450 core

#include <shader/block300/pass300.glsl>
#include <shader/lib/resample.glsl>
#include <shader/re/post_process/pp_block.glsl>


layout(binding = TEXTURE_UNIT_PP_SCENE) uniform sampler2D textureScene;
// Its stupid but glsl does not support rgb16f as image unit have to use rgba16f (there could be a workaround with with 3*width r16f)
layout(rgba16f, binding = TEXTURE_UNIT_PP_BLOOM_PYRAMID_OUTPUT) writeonly uniform image2D textureBloomPyramidOutput;


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

layout(local_size_x = 8, local_size_y = 8) in;
void main() {
	ivec2 inputSize = textureSize(textureScene, 0).xy;
	ivec2 outputSize = imageSize(textureBloomPyramidOutput).xy;
	ivec2 outputIndex = ivec2(gl_GlobalInvocationID.xy);

	vec2 requestIndex = vec2(outputIndex) - bloomBoundingPosition;
	vec2 sourceUV = requestIndex / bloomBoundingSize;

	vec2 pixelSize = 2.0 / vec2(inputSize);
	// NOTE: downsampleBox13 expects uv and pixelSize in target space
	vec3 sample0 = downsampleBox13Balanced(textureScene, sourceUV, pixelSize).rgb;
//	vec3 sample0 = downsampleBox13(textureScene, sourceUV, pixelSize).rgb;
	sample0 = prefilter(sample0, bloomThreshold, bloomKnee);

	// Absolute 0 values would break the math and spread blacking out the whole image
	sample0 = max(sample0, 0.0001);

	// Output
	imageStore(textureBloomPyramidOutput, outputIndex, vec4(sample0, 1));
}

//	float outside = (sourceUV.x < 0 || sourceUV.x > 1 || sourceUV.y < 0 || sourceUV.y > 1) ? 0.15 : 0.0;
//	sample0 -= outside;
