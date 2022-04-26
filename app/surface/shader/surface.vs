#version 330 core

#include <block/matrices.glsl>


layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec4 vertexColor;

out vec3 fragmentPositionW;
out vec3 fragmentNormal;
out vec4 fragmentColor;


void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
	fragmentPositionW = (matM * vec4(vertexPosition, 1)).xyz;
	fragmentNormal = mat3(matM) * vertexNormal;
	fragmentColor = vertexColor;
}
