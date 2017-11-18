#version 330 core

layout(location = 0) in vec3 vertexPos;

uniform mat4 MVPmat;

out vec4 fragmentColor;

void main() {
	gl_Position = MVPmat * vec4(vertexPos, 1);
}
