#version 330 core

layout(location = 0) in vec3 vertexPos;

uniform mat4 MVPmat;
uniform mat4 Mmat;

out vec3 fragmentPosW;

void main() {
	gl_Position = MVPmat * vec4(vertexPos, 1);
	fragmentPosW = vec3(Mmat * vec4(vertexPos, 1 ));
}