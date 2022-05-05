#pragma once


struct GeometryData {
	vec3 positionW;
	vec3 normalW;
	vec3 eyeDir;

	vec3 right;
	vec3 up;

	int type;

//	vec4 color;
//	float segmentPosition;
//	float segmentSize;
//	float chainPosition;
//	float chainSize;
//	float animationOffset;
};

struct FragmentData {
	vec3 positionW;
	vec4 color;
	vec2 uv;
	vec2 tile_index;
	float dither0;
	float dither1;

	float type;

//	vec3 eyeDir;
//	vec2 part_uv;
//	float head;
//	float segmentPosition;
//	float segmentSize;
//	float chainPosition;
//	float chainSize;
//	float animationOffset;
};

float rand(vec2 co) {
	return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
