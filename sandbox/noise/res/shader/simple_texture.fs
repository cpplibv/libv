#version 450 core

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture0Sampler;

void main() {
	result = texture(texture0Sampler, fragmentTexture0).rgba;
}
