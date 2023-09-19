#version 330 core

#include <shader/block300/model300.glsl>
#include <shader/block300/pass300.glsl>

layout(location = VERTEX_ATTR_POSITION) in vec3 vertexPosition;

invariant gl_Position; // invariant - Enforce consitent depth value calculations

// -------------------------------------------------------------------------------------------------

void main() {
	gl_Position = matP * matMV * vec4(vertexPosition, 1.0);
}
