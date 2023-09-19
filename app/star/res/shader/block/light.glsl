#pragma once

layout(std140) uniform Light {
	// Members are ordered in a way that floats are packed in vec3 paddings

	vec3 positionW;
	float cosAngleInner;

	vec3 directionW;
	float cosAngleOuter;

	vec3 colorDiffuse;
	float intensity;

	vec3 colorSpecular; // Not sure if diffuse and specular color split will be useful with PBR
	uint type; // Point = 0, Spot = 1, Directional = 2

	// = 64 byte

	// float range;
	// or
	// vec3 attenuation; constant, linear, exponent

	// bool selected; // Is the light selected in the editor
};
