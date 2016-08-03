#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 2) in vec4 vertexColor;

uniform mat4 MVPmat;

out vec4 fragmentColor;

void main() {
	gl_Position = MVPmat * vec4(vertexPos, 1);
	fragmentColor = vertexColor;
}