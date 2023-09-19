//

#pragma once

#ifndef BUILTIN_BLOCK_IDENTIFIER_LIGHTS
#define BUILTIN_BLOCK_IDENTIFIER_LIGHTS
#endif

struct LightDirectional300 {
	vec3 directionW;
	//< 4 byte padding

	vec3 radiance;
	//< 4 byte padding

	// = 32 byte
	// bool castsShadow
};

struct LightPoint300 {
	vec3 positionW;
	float range;

	vec3 radiance;
	float falloff;

	uint attenuationFormula;
	//< 12 byte padding

	// = 48 byte
	// bool castsShadow
};

struct LightSpot300 {
	vec3 positionW;
	float range;

	vec3 directionW;
	float cosAngleInner;

	vec3 radiance;
	float cosAngleOuter;

	float falloff;
	uint attenuationFormula;
	//< 8 byte padding

	// = 64 byte
	// bool castsShadow
};

layout(std140) uniform Lights300 {
	uint numLightDirectionals;
	uint numLightPoints;
	uint numLightSpots;
	//< 4 byte padding
	LightDirectional300 lightDirectionals[4]; // 4 Will be an all time hard limit for directional lights
	LightPoint300 lightPoints[8];
	LightSpot300 lightSpots[8];

	// = 1040 byte

	// int numShadowStuffs
	// Shadow shadowStuffs[4]
} BUILTIN_BLOCK_IDENTIFIER_LIGHTS;
