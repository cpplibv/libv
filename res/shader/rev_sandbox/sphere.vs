#version 330 core

#include <rev_sandbox/block/matrices.glsl>

layout(location = 0) in vec3 vertexPosition;
layout(location = 8) in vec2 vertexTexture0;

out vec2 fragmentTexture0;

void main() {
	fragmentTexture0 = vertexTexture0;
	gl_Position = matMVP * vec4(vertexPosition, 1);
}
