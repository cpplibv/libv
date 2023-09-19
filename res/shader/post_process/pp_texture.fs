#version 330 core

in vec2 fragmentTexture0;

out vec4 outputColor;

uniform sampler2D texture0;

// -------------------------------------------------------------------------------------------------

void main() {
	outputColor = texture(texture0, fragmentTexture0).rgba;
}
