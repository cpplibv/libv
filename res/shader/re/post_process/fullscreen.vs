#version 330 core

layout(location = VERTEX_ATTR_POSITION) in vec3 vertexPosition;
layout(location = VERTEX_ATTR_UV) in vec2 vertexUV;

out vec2 fragmentUV;

// -------------------------------------------------------------------------------------------------

void main() {
    gl_Position = vec4(vertexPosition, 1.0); // Set depth to 1.0 with xyww
	fragmentUV = vertexUV;
}
