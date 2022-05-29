#version 330 core

#include <block/matrices.glsl>


in vec4 fragmentColor;
in float fragmentFogFactor;

out vec4 result;


uniform vec4 fogColor = vec4(0.7, 0.8, 0.9, 1.0);


void main() {
	result = fragmentColor;

	// --- Fog ---

	result.rgb = mix(result.rgb, fogColor.rgb, fragmentFogFactor);
}
