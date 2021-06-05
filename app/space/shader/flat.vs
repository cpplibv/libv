#version 330 core

#include <block/matrices.glsl>

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 8) in vec2 vertexTexture0;

out vec3 fragmentPositionW;
out vec3 fragmentNormal;
out vec2 fragmentTexture0;


void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
	fragmentPositionW = (matM * vec4(vertexPosition, 1)).xyz;
	fragmentNormal = vertexNormal;
	fragmentTexture0 = vertexTexture0;
}
