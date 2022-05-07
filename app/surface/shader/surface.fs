#version 330 core

#include <block/matrices.glsl>
//#include <lib/color.glsl>


in vec3 fragmentPositionW;
in vec4 fragmentColor;
in vec3 fragmentNormal;
in float fragmentFogFactor;

out vec4 result;


uniform sampler2D textureBase;

uniform vec4 fogColor = vec4(0.7, 0.8, 0.9, 1.0);


void main() {
	vec3 N = normalize(fragmentNormal); // Normal vector
	vec3 V = normalize(eye - fragmentPositionW); // View vector
//	vec3 L = vec3(0, 0, 1); // Light vector
	vec3 L = normalize(vec3(0.6, 0.8, 1)); // Light vector

	vec3 R = reflect(-L, N); // Reflection vector

	float strength_diffuse = max(dot(N, L), 0.0);
	float strength_specular = pow(max(dot(V, R), 0.0), 16);

	float attenuation =
			strength_diffuse +
			strength_specular * 0.05;

	// --- Tex --

//	+ texture(textureBase, vec3(fract(fragmentPositionW.xy * 0.2), textureIndex.x).rgb * textureWeight.x;
//	+ texture(textureBase, vec3(fract(fragmentPositionW.xy * 0.2), textureIndex.y).rgb * textureWeight.y;
//	+ texture(textureBase, vec3(fract(fragmentPositionW.xy * 0.2), textureIndex.z).rgb * textureWeight.z;
//	+ texture(textureBase, vec3(fract(fragmentPositionW.xy * 0.2), textureIndex.w).rgb * textureWeight.w;

	vec3 texBase = texture(textureBase, fract(fragmentPositionW.xy * 0.2)).rgb;
//	texBase = hsv_to_rgb(rgb_to_hsv(texBase) * vec3(1, 0, 1));

	result = vec4(texBase * fragmentColor.rgb * attenuation, fragmentColor.a);

	// --- Fog ---

	result.rgb = mix(result.rgb, fogColor.rgb, fragmentFogFactor);

	// --- Debug ---

//	result = vec4(abs(N), 1);
//	result = vec4(N, 1);

//	result = fragmentColor;
//
//	result = vec4(vec3(fragmentPositionW.z), fragmentColor.a);
}
