#version 330 core

#include <block/matrices.glsl>
#include <builtin/sprite.glsl>
#include <lib/fog.glsl>
#include <lib/constants.glsl> // !!!


layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec4 vertexHSVColorShift;
layout(location = 6) in vec3 vertexSurfaceNormal;
layout(location = 13) in int vertexType;
layout(location = 15) in vec2 vertexRotationScale;

out GeometryData vs_out;


uniform bool fogEnabled = true;
uniform float fogIntensity = 0.05;


// -------------------------------------------------------------------------------------------------

void main() {
	vec3 positionW = (matM * vec4(vertexPosition, 1)).xyz;

	float dist_curve = length(eye.xy - positionW.xy);
	float fake_sphere = -pow(dist_curve * 0.02, 2);

//	gl_Position = matMVP * vec4(vertexPosition + vec3(0, 0, fake_sphere), 1);
//	gl_Position = matMVP * vec4(vertexPosition, 1);
	vs_out.eyeDir = normalize(eye - vertexPosition);
	vs_out.positionW = (matM * vec4(vertexPosition + vec3(0, 0, fake_sphere), 1)).xyz;
//	vs_out.positionW = (matM * vec4(vertexPosition, 1)).xyz; // Used?
//	vs_out.positionW = vertexPosition; // Used?
	vs_out.type = vertexType;

	vs_out.normalUp = vertexNormal;
	vs_out.surfaceNormal = vertexSurfaceNormal;
	vs_out.rotation = vertexRotationScale.x;
//	vs_out.normalUp = vec3(0, 0, 1); // !!!
//	vs_out.rotation = pi *0; // !!!
	vs_out.scale = vertexRotationScale.y;
	vs_out.hsvColorShift = vertexHSVColorShift;

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
