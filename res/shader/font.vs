#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 8) in vec2 vertexUV;

uniform mat4 MVPmat;

out vec2 fragmentUV;

void main() {
	gl_Position = MVPmat * vec4(vertexPos, 1);
	fragmentUV = vertexUV;
}