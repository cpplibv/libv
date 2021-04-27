#version 330 core

in vec4 fragmentColor;

out vec4 result;

#include <block/sphere.glsl>

void main() {
	result = fragmentColor;
}
