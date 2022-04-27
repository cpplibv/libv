#version 330 core

#include <block/matrices.glsl>


layout(location = 0) in vec3 vertexPosition;

out vec3 fragmentDirectionW;


void main() {
    fragmentDirectionW = vertexPosition;
	vec4 pos = matMVP * vec4(vertexPosition, 1.0);
    gl_Position = pos.xyww; // Set depth to 1.0
}
