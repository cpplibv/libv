#version 330 core

#include <block/matrices.glsl>
#include <lib/fresnel.glsl>

in vec3 fragmentPositionW;
in vec3 fragmentNormal;
in vec2 fragmentTexture0;

out vec4 result;

uniform vec4 base_color;
uniform bool selected;


void main() {
	vec3 N = normalize(fragmentNormal); // Normal vector
	vec3 V = normalize(eye - fragmentPositionW); // View vector
	vec3 L = vec3(0, 0, 1); // Light vector

	vec3 R = reflect(-L, N); // Reflection vector

	float strength_diffuse = max(dot(N, L), 0.0);
	float strength_specular = pow(max(dot(V, R), 0.0), 16);

	float attenuation =
			strength_diffuse +
			strength_specular * 0.2;
	result = vec4(base_color.rgb * attenuation, base_color.a);

	if (selected) {
//		vec3 fresnel_color = fresnel(2, vec3(0.95, 0.55, 0.0), N, V);
		vec3 fresnel_color = fresnel(2, vec3(2.0, 1.0, 0.2), N, V);
		result.rgb += fresnel_color;
	}
}
