#pragma once


struct GeometryData {
	vec3 positionW;
//	vec3 normalW;
	vec3 eyeDir;

	vec3 normalUp;
	vec3 surfaceNormal;
	float rotation;
	float scale;

	vec4 hsvColorShift;

	int type;
	float fogFactor;
};

struct FragmentData {
	vec3 positionW;
	vec4 rotationQuat;

	vec3 surfaceNormal;

	vec2 uv;
	vec2 tile_index;
	vec2 dither; // [ -0.5 .. 0.5 ]

	vec4 hsvColorShift;

	float type;
	float fogFactor;
};

float rand(vec2 co) {
	return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
