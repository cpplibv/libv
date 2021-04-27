#version 330 core

layout(location = 0) in vec3 vertexPosition;

out vec2 fragmentTexture0;

void main() {
	gl_Position = vec4(vertexPosition, 1);
	fragmentTexture0 = vertexPosition.xy * 0.5 + 0.5;
}
