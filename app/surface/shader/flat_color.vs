#version 330 core

#include <block/matrices.glsl>
#include <lib/fog.glsl>


layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor;

out vec4 fragmentColor;
out float fragmentFogFactor;


uniform bool fogEnabled = true;
uniform float fogIntensity = 0.05;


void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
	vec3 fragmentPositionW = (matM * vec4(vertexPosition, 1)).xyz;
	fragmentColor = vertexColor;

	// --- Fog ---

	if (fogEnabled) {
		float dist = length(eye - fragmentPositionW);
//		float dist = gl_FragCoord.z / gl_FragCoord.w;

//		float fogAmount = fogFactorLinear(dist, 1, 10);
//		float fogAmount = fogFactorExp(dist, fogIntensity);
		fragmentFogFactor = fogFactorExp2(dist, fogIntensity);

	} else {
		fragmentFogFactor = 0;
	}
}
