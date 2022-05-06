#version 330 core

#include <block/matrices.glsl>


layout(location = 0) in vec3 vertexPosition;
//layout(location = 1) in vec3 vertexNormal;

//out vec3 fragmentNormal;

void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
//	fragmentNormal = mat3(matM) * vertexNormal;
}
