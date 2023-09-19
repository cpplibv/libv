//

#pragma once

#ifndef BLOCK_IDENTIFIER_PP_BLOCK
#define BLOCK_IDENTIFIER_PP_BLOCK
#endif


layout(std140) uniform PostProcess300 {
	float bloomThreshold;
	float bloomKnee;
	float bloomDecay;
	float bloomFinalMultiplier;

	float bloomCombineAdd;
	float bloomCombineMix;
	vec2 bloomBoundingPosition;

	vec2 bloomBoundingSize;
	float vignetteIntensity;
	float vignetteSmoothness;

	float vignetteRoundness;

	vec3 vignetteColor;

	vec3 whitePoint;

	vec3 whiteCurve;
} BLOCK_IDENTIFIER_PP_BLOCK;
