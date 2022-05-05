#version 330 core

#include <builtin/sprite.glsl>
#include <lib/fog.glsl>
#include <rev_sandbox/block/matrices.glsl>


layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
//layout(location = 11) in vec3 vertexHSVColorShift;
//layout(location = 12) in vec2 vertexRotationHeight;
layout(location = 13) in int vertexType;

out GeometryData vs_out;


uniform bool fogEnabled = true;
uniform float fogIntensity = 0.05;


// -------------------------------------------------------------------------------------------------

void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
	vec3 positionW = (matM * vec4(vertexPosition, 1)).xyz;
	vs_out.eyeDir = normalize(eye - vertexPosition);
	vs_out.positionW = (matM * vec4(vertexPosition, 1)).xyz; // Used?
//	vs_out.positionW = vertexPosition; // Used?
	vs_out.type = vertexType;

	vs_out.normalW = normalize((matM * vec4(0, 0, 1, 1)).xyz);

	// --- Fog ---

	if (fogEnabled) {
		float dist = length(eye - positionW);
//		float dist = gl_FragCoord.z / gl_FragCoord.w;

//		float fogAmount = fogFactorLinear(dist, 1, 10);
//		float fogAmount = fogFactorExp(dist, fogIntensity);
		vs_out.fogFactor = fogFactorExp2(dist, fogIntensity);

	} else {
		vs_out.fogFactor = 0;
	}
}
