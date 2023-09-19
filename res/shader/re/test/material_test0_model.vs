#version 450 core

#include <shader/block300/model300.glsl>
#define BUILTIN_BLOCK_IDENTIFIER_PASS pass
#include <shader/block300/pass300.glsl>

layout(location = VERTEX_ATTR_POSITION) in vec3 vertexPosition;
layout(location = VERTEX_ATTR_UV) in vec2 vertexUV;
layout(location = VERTEX_ATTR_NORMAL) in vec3 vertexNormal;
layout(location = VERTEX_ATTR_TANGENT) in vec3 vertexTangent;
layout(location = VERTEX_ATTR_DIFFUSE) in vec4 vertexColor;
layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW0) in vec4 vertexInstanceMatRow0;
layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW1) in vec4 vertexInstanceMatRow1;
layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW2) in vec4 vertexInstanceMatRow2;

//layout(binding = TEXTURE_UNIT_DIFFUSE) uniform sampler2D texture0;

uniform vec4 uniform_color;

layout(std140) uniform Shared {
	vec4 shared_block_color;
};

layout(std140) uniform Custom {
	vec4 custom_block_color;
};

invariant gl_Position; // invariant - Enforce consitent depth value calculations

out vec4 fragmentColor;
out vec3 fragmentPositionW;
out vec2 fragmentUV;
out vec3 fragmentNormalUnW;
out vec3 fragmentTangentUnW;
out vec3 fragmentBitangentUnW;

// -------------------------------------------------------------------------------------------------

void main() {

	mat4 matSubM = mat4(
		vec4(vertexInstanceMatRow0.x, vertexInstanceMatRow1.x, vertexInstanceMatRow2.x, 0.0),
		vec4(vertexInstanceMatRow0.y, vertexInstanceMatRow1.y, vertexInstanceMatRow2.y, 0.0),
		vec4(vertexInstanceMatRow0.z, vertexInstanceMatRow1.z, vertexInstanceMatRow2.z, 0.0),
		vec4(vertexInstanceMatRow0.w, vertexInstanceMatRow1.w, vertexInstanceMatRow2.w, 1.0)
	);

	fragmentColor = vertexColor;
	fragmentPositionW = vec3(matM * matSubM * vec4(vertexPosition, 1));
	fragmentUV = vertexUV;
	fragmentNormalUnW = mat3(matM) * mat3(matSubM) * vertexNormal;
	fragmentTangentUnW = mat3(matM) * mat3(matSubM) * vertexTangent;
	fragmentBitangentUnW = cross(fragmentNormalUnW, fragmentTangentUnW);

	gl_Position = pass.matP * matMV * matSubM * vec4(vertexPosition, 1);
}
