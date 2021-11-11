#version 330 core

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture0;

// -------------------------------------------------------------------------------------------------

void main() {
	result = texture(texture0, fragmentTexture0).rgba;
}
