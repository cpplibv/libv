#version 330 core

layout(location = 0) in vec3 vertexPosition;

out vec2 fragmentTexture0;

void main() {
	gl_Position = vec4(vertexPosition, 1);
	gl_Position.z = 1.0; // Send the vertex to the far plane
	fragmentTexture0 = vertexPosition.xy * 0.5 + 0.5;
}
