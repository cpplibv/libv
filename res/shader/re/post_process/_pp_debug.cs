#version 450 core

#include <shader/block300/pass300.glsl>
#include <shader/lib/resample.glsl>
#include <shader/re/post_process/pp_block.glsl>


layout(binding = TEXTURE_UNIT_PP_SCENE) uniform sampler2D textureScene;
layout(rgba16f, binding = TEXTURE_UNIT_PP_BLOOM_PYRAMID_OUTPUT) writeonly uniform image2D textureSceneOutput;


// -------------------------------------------------------------------------------------------------

//vec4 DEBUGtexture(sampler2D tex, vec2 uv) {
//	float outside = (uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1) ? 0.15 : 0.0;
////	float outside = 0;
//	uv = abs(floor(uv * 0.5 + 0.5) * 2.0 - uv); // Mirrored repeat UV remapping
//
////	vec2 px = 1.0 / textureSize(tex, 0);
////
////	float t0 = 1.0 * (sin(timeSimLooping * 0.25) * 0.5 + 0.5);
////	float t1 = 1.1 * (sin((timeSimLooping + 8.0) * 0.25) * 0.5 + 0.5);
////
////	float s0 = 10; // Must be even or it flickers
////
////	if (t0 + px.y * s0 * -0.5 <= uv.y && uv.y <= t0 + px.y * s0 * +0.5)
////		return vec4(vec3(15.0 * uv.x), 1);
////
////	if (t1 + px.y * s0 * -0.5 <= uv.y && uv.y <= t1 + px.y * s0 * +0.5)
////		return vec4(vec3(15.0 * uv.x), 1);
////
//////	return texture(tex, uv);
////	return vec4(outside, outside, outside, 1);
//
////	vec2 px = 1.0 / textureSize(tex, 0);
////	float t0 = 1.0 * (fract(timeSimLooping * 0.10));
////	float t1 = 1.1 * (fract((timeSimLooping * 0.221 + 8.0) * 0.25));
////	float sizeY = textureSize(tex, 0).y;
////	t0 = (round(t0 * sizeY)) / sizeY;
////	t1 = (round(t1 * sizeY)) / sizeY;
////
////	float s0 = px.y * 2; // Must be even or it flickers
////
////	if (t0 - 0.00001 < uv.y && t0 + s0 + 0.00001 > uv.y)
////		return vec4(vec3(15.0 * uv.x), 1);
////
////	if (t1 - 0.00001 < uv.y && t1 + s0 + 0.00001 > uv.y)
////		return vec4(vec3(15.0 * uv.x), 1);
////
//////	return texture(tex, uv);
////	return vec4(outside, outside, outside, 1);
//
//	float sizeY = bloomBoundingSize.y;
////	float sizeY = textureSize(tex, 0).y;
//	float t0 = 1.0 * (fract(timeSimLooping * 0.10));
//	float t1 = 1.1 * (fract((timeSimLooping * 0.221 + 8.0) * 0.25));
//	t0 = (round(t0 * sizeY)) / sizeY;
//	t1 = (round(t1 * sizeY)) / sizeY;
//
//	float s0 = 2.0 / sizeY;
//
////	if (t0 < uv.y && uv.y < t0 + s0)
////		return vec4(vec3(15.0 * uv.x), 1);
////
////	if (t1 < uv.y && t1 + s0 > uv.y)
////		return vec4(vec3(15.0 * uv.x), 1);
//
//	if (fract(timeSimLooping * 0.0 + uv.y * bloomBoundingSize.y / 10.0) < 0.2)
//		return vec4(vec3(15.0 * uv.x), 1);
//
////	return texture(tex, uv);
//	return vec4(outside, outside, outside, 1);
//}

layout(local_size_x = 8, local_size_y = 8) in;
void main() {
	ivec2 inputSize = textureSize(textureScene, 0).xy;
	ivec2 outputSize = imageSize(textureSceneOutput).xy;
	ivec2 outputIndex = ivec2(gl_GlobalInvocationID.xy);

	vec2 uv = (vec2(outputIndex) + 0.5) / outputSize;

//	imageStore(textureSceneOutput, outputIndex, vec4(uv * 2, 0, 1));

	vec3 result = vec3(uv * 2, 0);

//	if (mod(round(timeSimLooping * 10) + outputIndex.y, 200) < 1)
	if (mod(round(timeSimLooping * 10) + outputIndex.y, 200) < 3)
		result = vec3(15.0 * uv.x);
	else
//		result = vec3(0);
		return;

	imageStore(textureSceneOutput, outputIndex, vec4(result, 1));
}
