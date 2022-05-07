#version 330 core

#include <block/matrices.glsl>


uniform samplerCube textureSkybox;

in vec3 fragmentDirectionW;

out vec4 result;


uniform bool fogEnabled = true;
//uniform float fogIntensity = 0.05;
uniform vec4 fogColor = vec4(0.7, 0.8, 0.9, 1.0);


void main() {
	result = texture(textureSkybox, fragmentDirectionW.xzy).rgba;
//	result = vec4(1, 0, 0, 1);

	// --- Fog ---

	if (fogEnabled)
		result = mix(result, fogColor, smoothstep(0.15, -0.05, normalize(fragmentDirectionW).z));
}
