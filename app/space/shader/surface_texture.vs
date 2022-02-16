#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 8) in vec2 vertexTexture0;

out vec2 fragmentTexture0;

#include <block/matrices.glsl>

void main() {
	gl_Position = vec4(vertexPosition, 1);
//	gl_Position = matMVP * vec4(vertexPosition, 1);
	fragmentTexture0 = vertexTexture0;
}
