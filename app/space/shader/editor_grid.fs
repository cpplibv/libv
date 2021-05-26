#version 330 core

in vec4 fragmentColor;

out vec4 result;

#include <block/matrices.glsl>

void main() {
	result = fragmentColor;
}
