#pragma once

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;

	float shininess;
};
