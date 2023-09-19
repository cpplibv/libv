#version 330 core

#include <shader/block300/model300.glsl>
#include <shader/block300/pass300.glsl>

layout(location = VERTEX_ATTR_POSITION) in vec3 vertexPosition;
layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW0) in vec4 vertexInstanceMatRow0;
layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW1) in vec4 vertexInstanceMatRow1;
layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW2) in vec4 vertexInstanceMatRow2;

invariant gl_Position; // invariant - Enforce consitent depth value calculations

// -------------------------------------------------------------------------------------------------

void main() {
	mat4 matSubM = mat4(
		vec4(vertexInstanceMatRow0.x, vertexInstanceMatRow1.x, vertexInstanceMatRow2.x, 0.0),
		vec4(vertexInstanceMatRow0.y, vertexInstanceMatRow1.y, vertexInstanceMatRow2.y, 0.0),
		vec4(vertexInstanceMatRow0.z, vertexInstanceMatRow1.z, vertexInstanceMatRow2.z, 0.0),
		vec4(vertexInstanceMatRow0.w, vertexInstanceMatRow1.w, vertexInstanceMatRow2.w, 1.0)
	);

	gl_Position = matP * matMV * matSubM * vec4(vertexPosition, 1);
}
