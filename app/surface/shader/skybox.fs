#version 330 core

#include <block/matrices.glsl>


uniform samplerCube textureSkybox;

in vec3 fragmentDirectionW;

out vec4 result;


uniform bool fogEnabled = true;
//uniform float fogIntensity = 0.05;
uniform vec4 fogColor = vec4(0.7, 0.8, 0.9, 1.0);


void main() {
	// Skybox but the horizon is pushed down a little to better match the fake spherical surface
	result = texture(textureSkybox, normalize(normalize(fragmentDirectionW.xzy) + vec3(0, 0.05, 0))).rgba;
	// Normal skybox
//	result = texture(textureSkybox, fragmentDirectionW.xzy).rgba;

	// --- Fog ---

	if (fogEnabled)
//		result = mix(result, fogColor, smoothstep(0.15, -0.05, normalize(fragmentDirectionW).z));
		result = mix(result, fogColor, smoothstep(0.05, -0.10, normalize(fragmentDirectionW).z));
//		result = mix(result, fogColor, smoothstep(0.05, -0.15, normalize(fragmentDirectionW).z));
}
