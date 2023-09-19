#version 450 core

out vec2 outputColor;

uniform float colorID;

// -------------------------------------------------------------------------------------------------

void main() {
	outputColor = vec2(1.0, colorID);
}
