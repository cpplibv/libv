#pragma once

struct GeometryData {
	vec3 position;
//	vec3 normal;
//	vec2 texture0;
	float segmentPosition;
	float segmentSize;
	float totalPosition;
	float totalSize;
};

struct FragmentData {
	vec4 color;
//	vec3 normal;
//	vec2 texture0;
	float segmentPosition;
	float segmentSize;
	float totalPosition;
	float totalSize;
};
