#version 330 core

#include <builtin/sprite.glsl>
#include <rev_sandbox/block/matrices.glsl>


layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out GeometryData vs_out;


// -------------------------------------------------------------------------------------------------

void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
//	fragmentPositionW = (matM * vec4(vertexPosition, 1)).xyz;
	vs_out.eyeDir = normalize(eye - vertexPosition);
	vs_out.positionW = (matM * vec4(vertexPosition, 1)).xyz; // Used?
}
