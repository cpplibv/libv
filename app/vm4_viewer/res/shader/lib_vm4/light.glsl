#pragma once

struct Light {
	int type;

	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

//	float range;
	float innerCosAngle;
	float outerCosAngle;
};
