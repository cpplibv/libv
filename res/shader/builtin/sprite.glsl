#pragma once


struct GeometryData {
	vec3 positionW;
//	vec3 normalW;
	vec3 eyeDir;

	vec3 normalUp;
	float rotation;
	float scale;

	vec4 hsvColorShift;

	int type;
	float fogFactor;
};

struct FragmentData {
	vec3 positionW;
	vec4 rotationQuat;
	vec2 uv;
	vec2 tile_index;
//	float dither0;
//	float dither1;

	vec4 hsvColorShift;

	float type;
	float fogFactor;
};

float rand(vec2 co) {
	return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
