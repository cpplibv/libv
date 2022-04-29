#version 330 core

#include <builtin/sprite.glsl>
#include <rev_sandbox/block/matrices.glsl>


layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
//layout(location = 11) in vec3 vertexHSVColorShift;
//layout(location = 12) in vec2 vertexRotationHeight;
layout(location = 13) in int vertexType;

out GeometryData vs_out;


// -------------------------------------------------------------------------------------------------

void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
//	fragmentPositionW = (matM * vec4(vertexPosition, 1)).xyz;
	vs_out.eyeDir = normalize(eye - vertexPosition);
	vs_out.positionW = (matM * vec4(vertexPosition, 1)).xyz; // Used?
//	vs_out.positionW = vertexPosition; // Used?
	vs_out.type = vertexType;

	vs_out.normalW = normalize((matM * vec4(0, 0, 1, 1)).xyz);
//	vs_out.up = mat3(matV) * vec3(0, 0, 1);
//	vs_out.right = mat3(matV) * vec3(0, -1, 0);
//	vs_out.right = matMVP * vec4(1, 0, 0, 1);
}
