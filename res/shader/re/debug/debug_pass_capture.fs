#version 450 core

#include <shader/block300/pass300.glsl>
#include <shader/re/outline/jump_flood.glsl>

in vec2 fragmentUV;

layout(binding = TEXTURE_UNIT_DIFFUSE) uniform sampler2D textureDiffuse;
layout(binding = TEXTURE_UNIT_STENCIL) uniform usampler2D textureStencil;
layout(binding = TEXTURE_UNIT_SPECULAR) uniform sampler2DArray textureDiffuseArray;
//layout(binding = TEXTURE_UNIT_SKY) uniform samplerCube textureSky;

uniform int captureMode = DEBUG_MODE_NORMAL;
uniform int levelIndex = -1;
uniform int layerIndex = -1;

out vec4 outputColor;


// -------------------------------------------------------------------------------------------------

// TODO P1: Place these linearize depth functions in the glsl lib, and make them a proper and full set
//				range:     0..1, -1..1
//				direction: normalZ, reverseZ
//				infOrFar:  far, inf

float linearize_ReverseZinf_depth_01(float depthValue01, float near) {
	return near / depthValue01;
}

//float linearize_depth_01(float depthValue01, float near, float far) {
//	return near * far / (far + depthValue01 * (near - far));
//}
//
//float linearize_depth_m1p1(in float zoverw, in float near, in float far) {
//	return (2.0 * near) / (far + near - zoverw * (far - near)) * (far - near);
//}
//
//float normalize_depth(in float d, in float near, in float far) {
//	return (d * (far + near) + 2.0 * near * (near - far)) / (d * (far - near));
//}

void main() {
	vec4 sampleColor;
	if (layerIndex == -1) {
		if (levelIndex == -1)
			sampleColor = texture(textureDiffuse, fragmentUV).rgba;
		else
			sampleColor = textureLod(textureDiffuse, fragmentUV, levelIndex).rgba;
	} else {
		if (levelIndex == -1)
			sampleColor = texture(textureDiffuseArray, vec3(fragmentUV, layerIndex)).rgba;
		else
			sampleColor = textureLod(textureDiffuseArray, vec3(fragmentUV, layerIndex), levelIndex).rgba;
	}

	if (captureMode == DEBUG_MODE_NORMAL) {
		outputColor = sampleColor;
	} else if (captureMode == DEBUG_MODE_DEPTH) {
		outputColor = vec4(sampleColor.rrr, 1);
	} else if (captureMode == DEBUG_MODE_DEPTH_LINEAR) {
		outputColor = vec4(1.0 - vec3(fract(linearize_ReverseZinf_depth_01(sampleColor.r, cameraNear) / (cameraFar - cameraNear))), 1);
	} else if (captureMode == DEBUG_MODE_DEPTH_DISTANCE) {
		float depth = sampleColor.r;
		vec4 ndc = vec4(fragmentUV * 2 - 1, depth, 1.0);
		vec4 world = matVPinv * ndc;
		world /= world.w;
		float dist = length(eyeW - world.xyz);
		outputColor = vec4(vec3(fract(dist / (cameraFar - cameraNear))), 1);
	} else if (captureMode == DEBUG_MODE_STENCIL) {
		outputColor = vec4(texture(textureStencil, fragmentUV).rrr / 255.0, 1.0);
	} else if (captureMode == DEBUG_MODE_JFA) {
		outputColor = vec4(sampleColor.xy * 0.5 + 0.5, 0, 1);
	} else if (captureMode == DEBUG_MODE_MISSING) {
		outputColor = vec4(1, 0, 1, 1);
	} else {
		outputColor = vec4(1, 0, 1, 1);
	}
}
