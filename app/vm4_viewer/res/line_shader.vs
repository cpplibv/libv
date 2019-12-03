#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 matMVP;

void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
}
