#version 330 core

#include <block/matrices.glsl>
#include <lib/fog.glsl>


layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec4 vertexColor;

out vec3 fragmentPositionW;
out vec3 fragmentNormal;
out vec4 fragmentColor;
out float fragmentFogFactor;


uniform bool fogEnabled = true;
uniform float fogIntensity = 0.05;


void main() {
	fragmentPositionW = (matM * vec4(vertexPosition, 1)).xyz;

	float dist = length(eye - fragmentPositionW);
	float dist_curve = length(eye.xy - fragmentPositionW.xy);
	float fake_sphere = -pow(dist_curve * 0.02, 2);

	gl_Position = matMVP * vec4(vertexPosition + vec3(0, 0, fake_sphere), 1);
	fragmentNormal = mat3(matM) * vertexNormal;
	fragmentColor = vertexColor;

	// --- Fog ---

	if (fogEnabled) {
//		float dist = gl_FragCoord.z / gl_FragCoord.w;

//		float fogAmount = fogFactorLinear(dist, 1, 10);
//		float fogAmount = fogFactorExp(dist, fogIntensity);
		fragmentFogFactor = fogFactorExp2(dist, fogIntensity);

	} else {
		fragmentFogFactor = 0;
	}
}
