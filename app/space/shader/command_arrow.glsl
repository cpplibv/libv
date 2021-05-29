#pragma once

struct GeometryData {
	float segmentPosition;
	float segmentSize;
	float totalPosition;
	float totalSize;
};

struct FragmentData {
	vec2 uv;
	vec2 part_uv;
	float head;
	float segmentPosition;
	float segmentSize;
	float totalPosition;
	float totalSize;
};
