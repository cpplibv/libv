#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor;

uniform mat4 matMVP;

out vec4 fragmentColor;

void main() {
	fragmentColor = vertexColor;
	gl_Position = matMVP * vec4(vertexPosition, 1);
}
