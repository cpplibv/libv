#pragma once


float fresnel(float power, vec3 N, vec3 V) {
	float fresnel = pow(1.0 - dot(N, V), power);
	return fresnel;
}

vec3 fresnel(float power, vec3 color, vec3 N, vec3 V) {
	float fresnel = pow(1.0 - dot(N, V), power);
	return color * fresnel;
}

vec4 fresnel(float power, vec4 color, vec3 N, vec3 V) {
	float fresnel = pow(1.0 - dot(N, V), power);
	return color * vec4(fresnel, fresnel, fresnel, 1.0);
}
