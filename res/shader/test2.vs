#version 330 core

layout(location = 0) in vec3 vertexPos;

uniform mat4 matMVP;
uniform mat4 matM;

out vec3 fragmentPosW;

void main() {
	gl_Position = matMVP * vec4(vertexPos, 1);
	fragmentPosW = vec3(matM * vec4(vertexPos, 1 ));
}
