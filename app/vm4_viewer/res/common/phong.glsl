#pragma once

#include <app/vm4_viewer/res/common/light.glsl>
#include <app/vm4_viewer/res/common/material.glsl>



vec4 phong_diffuse(vec3 eyeW, vec3 fragmentPositionW, vec3 fragmentNormalW, vec3 material, Light light);
vec4 phong_specular(vec3 eyeW, vec3 fragmentPositionW, vec3 fragmentNormalW, vec3 material, Light light);
vec4 phong_emission(vec3 eyeW, vec3 fragmentPositionW, vec3 fragmentNormalW, vec3 material, Light light);


vec4 phong(vec3 eyeW, vec3 fragmentPositionW, vec3 normal, Material material, Light light) {
	vec4 result = vec4(0, 0, 0, 1);

	vec3 N = normal; // Normal vector
	vec3 V = normalize(eyeW - fragmentPositionW); // View vector
	vec3 L; // Light vector

	// TODO P2: Use attenuation
	float attenuation;

	// Point light
	if (light.type == 1) {
		L = normalize(light.position - fragmentPositionW);
		float dist = dot(L, L);
		attenuation = 1 / (dist * dist);

	// Directional Light
	} else if (light.type == 2) {
		L = -light.direction;
		attenuation = 1;

	// Spot light
	} else if (light.type == 3) {
		L = normalize(light.position - fragmentPositionW);
		float dist = dot(L, L);
		float ioDiff = 1 / (light.innerCosAngle - light.outerCosAngle);

		attenuation = max(dot(-light.direction, L), 0.0); // SpotDir Dot L
		attenuation = clamp(attenuation * ioDiff - light.outerCosAngle * ioDiff, 0.0, 1.0); // Spot Cone
		attenuation = attenuation * 1 / (dist * dist); // Distance
	}

	vec3 R = reflect(-L, N); // Reflection vector

	float strength_diffuse = max(dot(N, L), 0.0);
	float strength_specular = pow(max(dot(V, R), 0.0), material.shininess);

	vec3 ambient = material.ambient * light.ambient;
	vec3 diffuse = material.diffuse * light.diffuse * strength_diffuse * attenuation;
	vec3 specular = material.specular * light.specular * strength_specular * attenuation;
	vec3 emission = material.emission;

	result.rgb = ambient + diffuse + specular + emission;

	return result;
}
