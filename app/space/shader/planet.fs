#version 330 core

#include <block/matrices.glsl>
#include <lib/fresnel.glsl>

in vec3 fragmentPositionW;
in vec3 fragmentNormal;
in vec2 fragmentTexture0;

out vec4 result;

uniform vec4 base_color0 = vec4(1, 1, 1, 1);
uniform vec4 base_color1 = vec4(1, 1, 1, 1);
//uniform int selected = 0;


void main() {
	vec3 N = normalize(fragmentNormal); // Normal vector
	vec3 V = normalize(eye - fragmentPositionW); // View vector
	vec3 L = vec3(0, 0, 1); // Light vector

	vec3 R = reflect(-L, N); // Reflection vector

	float strength_diffuse = max(dot(N, L), 0.0);
	float strength_specular = pow(max(dot(V, R), 0.0), 16);

	result = vec4(
			(strength_diffuse * 0.9 + 0.1 ) * base_color0.rgb +
			strength_specular * 0.5 +
			fresnel(2, base_color1.rgb * 2, N, V) * strength_diffuse
			, base_color0.a
	);

//	result = vec4(base_color0.rgb * attenuation + strength_specular_atmo * base_color1.rgb, base_color0.a);


//	if (selected == 0) // NotSelected
//		;
//
//	else if (selected == 1) // hoverSingle
//		result.rgb += fresnel(4, vec3(2.0, 1.0, 0.2) * 2, N, V);
//
//	else if (selected == 2) // hoverBox
//		result.rgb += fresnel(4, vec3(2.0, 1.0, 0.2) * 2, N, V);
//
//	else if (selected == 3) // Selected
//		result.rgb += fresnel(2, vec3(2.0, 1.0, 0.2) * 4, N, V);
//
//	else if (selected == 4) // selectedHoverSingle
//		result.rgb += fresnel(2, vec3(2.0, 1.0, 0.2) * 6, N, V);
//
//	else if (selected == 5) // selectedHoverBox
//		result.rgb += fresnel(2, vec3(2.0, 1.0, 0.2) * 6, N, V);

//	result.rgb = N;
}
