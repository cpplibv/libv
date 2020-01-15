#version 330 core

out vec4 output;

uniform vec4 color;

void main() {
	output = color;
	//output = vec4(1, 0, 0, 1);
}
