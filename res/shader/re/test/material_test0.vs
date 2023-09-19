#version 450 core

#include <shader/block300/model300.glsl>
#define BUILTIN_BLOCK_IDENTIFIER_PASS pass
#include <shader/block300/pass300.glsl>

layout(location = VERTEX_ATTR_POSITION) in vec3 vertexPosition;
layout(location = VERTEX_ATTR_UV) in vec2 vertexUV;
layout(location = VERTEX_ATTR_NORMAL) in vec3 vertexNormal;
layout(location = VERTEX_ATTR_TANGENT) in vec3 vertexTangent;
layout(location = VERTEX_ATTR_DIFFUSE) in vec4 vertexColor;

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
//	gl_Position = vec4(vertexPosition, 1);
//	gl_Position = vec4(vertexPosition * (sin(pass.timeSimLooping * 0.2) * 0.2 + 0.6), 1);

//	fragmentColor = vertexColor;
//	fragmentUV = vertexUV;
////	fragmentColor = vec4(0, 1, 0, 1);
////	fragmentNormal = vertexNormal;
//	fragmentNormalW = mat3(matM) * vertexNormal;
//	fragmentPositionW = (matM * vec4(vertexPosition, 1)).xyz;
//
//	gl_Position = pass.matP * matMV * vec4(vertexPosition, 1);

	fragmentColor = vertexColor;
	fragmentPositionW = vec3(matM * vec4(vertexPosition, 1));
	fragmentUV = vertexUV;
	fragmentNormalUnW = mat3(matM) * vertexNormal;
	fragmentTangentUnW = mat3(matM) * vertexTangent;
	fragmentBitangentUnW = cross(fragmentNormalUnW, fragmentTangentUnW);

	gl_Position = pass.matP * matMV * vec4(vertexPosition, 1);
}
