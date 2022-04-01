#version 330 core

#include <rev_sandbox/block/matrices.glsl>


layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
//layout(location = 2) in vec4 vertexColor;
//layout(location = 6) in vec3 vertexTangent;
//layout(location = 7) in vec3 vertexBitangent;
layout(location = 8) in vec2 vertexTexture0;

out vec3 fragmentPositionW;
//out vec4 fragmentColor;
out vec3 fragmentNormalW;
//out vec3 fragmentTangentW;
//out vec3 fragmentBitangentW;
out vec2 fragmentTexture0;

void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
	fragmentPositionW = (matM * vec4(vertexPosition, 1)).xyz;
//	fragmentColor = vertexColor;
	fragmentNormalW = mat3(matM) * vertexNormal;
//	fragmentTangentW = mat3(matM) * vertexTangent;
//	fragmentBitangentW = mat3(matM) * vertexBitangent;
	fragmentTexture0 = vertexTexture0;
}
