#version 450 core

#include <shader/lib/resample.glsl>

layout(binding = TEXTURE_UNIT_PP_BLOOM_PYRAMID) uniform sampler2D textureBloomPyramid;
layout(rgba16f, binding = TEXTURE_UNIT_PP_BLOOM_PYRAMID_OUTPUT) writeonly uniform image2D textureBloomPyramidOutput;

uniform int currentLevel;


// -------------------------------------------------------------------------------------------------

layout(local_size_x = 8, local_size_y = 8) in;
void main() {
	int previousLevel = currentLevel + 1;
	int targetLevel = currentLevel;

	ivec2 inputSize = textureSize(textureBloomPyramid, previousLevel).xy;
	ivec2 outputSize = imageSize(textureBloomPyramidOutput).xy;
	ivec2 outputIndex = ivec2(gl_GlobalInvocationID.xy);

	vec2 fragmentUV = (vec2(outputIndex) + 0.5) / vec2(outputSize);

	vec3 result = textureLod(textureBloomPyramid, fragmentUV, currentLevel).rgb;
	vec2 pixelSize = 1.0 / textureSize(textureBloomPyramid, previousLevel).xy;
	result += upsampleTent9Lod(textureBloomPyramid, previousLevel, fragmentUV, pixelSize, 1.0).rgb;

	// Output
	imageStore(textureBloomPyramidOutput, outputIndex, vec4(result, 1)); // output texture is bound to the targetLevel
}
