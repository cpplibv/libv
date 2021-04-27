#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec4 vertexColor;
layout(location = 6) in vec3 vertexTangent;
layout(location = 7) in vec3 vertexBitangent;
layout(location = 8) in vec2 vertexUV;

uniform mat4 matMVP;
uniform mat4 matM;

out vec4 fragmentColor;
out vec3 fragmentNormalW;
out vec3 fragmentTangentW;
out vec3 fragmentBitangentW;
out vec2 fragmentUV;

void main() {
	gl_Position = matMVP * vec4(vertexPos, 1);
	fragmentColor = vertexColor;
	fragmentNormalW = mat3(matM) * vertexNormal;
	fragmentUV = vertexUV;
}
