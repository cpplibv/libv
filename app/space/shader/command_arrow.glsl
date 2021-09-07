#pragma once


const float body_width = 7;
const float head_width = 16;
const float head_height = 28;
//const float body_width = 15;
//const float head_width = 50;
//const float head_height = 50;


struct GeometryData {
	vec4 color;
	float segmentPosition;
	float segmentSize;
	float chainPosition;
	float chainSize;
	float animationOffset;
};

struct FragmentData {
	vec4 color;
	vec2 uv;
	vec2 part_uv;
	float head;
	float segmentPosition;
	float segmentSize;
	float chainPosition;
	float chainSize;
	float animationOffset;
};
