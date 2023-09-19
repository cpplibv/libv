#version 330

uniform mat4 matMVP;

layout(location = 0) in vec3 vertexPos;

void main() {
	gl_Position = matMVP * vec4(vertexPos,1);
}
