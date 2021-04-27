#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor;

out vec4 fragmentColor;

#include <block/sphere.glsl>

void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
	fragmentColor = vertexColor;
}
