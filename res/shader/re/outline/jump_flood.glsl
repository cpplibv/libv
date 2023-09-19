#pragma once

#include <shader/block300/pass300.glsl>

// -------------------------------------------------------------------------------------------------

#define FLOOD_NULL (-1)
#define FLOOD_NULL2 vec2(FLOOD_NULL, FLOOD_NULL)

const float inf = 1.0 / 0.0;

vec2 uvToPosition(vec2 uv) {
	// Remap to -1..1 range to allow 16_SNORMs to have 16x16 subpixel positions at 4k
	// and deal with aspect ratios by have a uniform "position space" to have simpler steps later
	//
	// "FLOOD_NULL" and -1 will not interfere with a meaningful manner as only single pixel wide objects
	// at the exact edge of the canvas would lose their outline. This does not make the outline disapear in the
	// edge pixels.
	vec2 ratios = canvasSize.x > canvasSize.y ?
			vec2(1, canvasSize.y / canvasSize.x) :
			vec2(canvasSize.x / canvasSize.y, 1);
	return uv * 2.0 * ratios - 1.0;
}

vec2 uvToPositionWithPackLoss(vec2 uv) {
	// Conversion to SNORM 16 is lossy and causes differences in different stages
	// if the calculated float UVs are not round tripped too.
	// This pack can behave differently from native texture write conversion.
	return unpackSnorm2x16(packSnorm2x16(uvToPosition(uv)));
}

float positionToPixelDistance(vec2 position) {
	return length(position * 0.5) * max(canvasSize.x, canvasSize.y);
}

vec2 positionToUV(vec2 position) {
	vec2 ratios = canvasSize.x > canvasSize.y ?
			vec2(1, canvasSize.y / canvasSize.x) :
			vec2(canvasSize.x / canvasSize.y, 1);
	return (position * 0.5 + 0.5) / ratios;
}

bool isNullPosition(vec2 position) {
	return position.x == FLOOD_NULL;
}
