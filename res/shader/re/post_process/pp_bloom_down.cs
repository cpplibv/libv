#version 450 core

#include <shader/lib/resample.glsl>
#include <shader/re/post_process/pp_block.glsl>


layout(binding = TEXTURE_UNIT_PP_BLOOM_PYRAMID) uniform sampler2D textureBloomPyramid;
layout(rgba16f, binding = TEXTURE_UNIT_PP_BLOOM_PYRAMID_OUTPUT) writeonly uniform image2D textureBloomPyramidOutput;

uniform int currentLevel;


// -------------------------------------------------------------------------------------------------

layout(local_size_x = 8, local_size_y = 8) in;
void main() {
	int sourceLevel = currentLevel - 1;
	int targetLevel = currentLevel;
	ivec2 inputSize = textureSize(textureBloomPyramid, sourceLevel).xy;
	ivec2 outputSize = imageSize(textureBloomPyramidOutput).xy;
	ivec2 outputIndex = ivec2(gl_GlobalInvocationID.xy);

	vec2 sourceUV = vec2(outputIndex * 2 + 1) / inputSize;
	vec2 pixelSize = 2.0 / vec2(inputSize);
	// NOTE: downsampleBox13 expects uv and pixelSize in target space
	vec3 sample0 = downsampleBox13BalancedLod(textureBloomPyramid, sourceLevel, sourceUV, pixelSize).rgb;
//	vec3 sample0 = downsampleBox13Lod(textureBloomPyramid, sourceLevel, sourceUV, pixelSize).rgb;
	sample0 *= bloomDecay;

	// Output
	imageStore(textureBloomPyramidOutput, outputIndex, vec4(sample0, 1)); // output texture is bound to the targetLevel
}
