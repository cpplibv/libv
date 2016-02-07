#version 330 core

uniform sampler2D textureSampler;

in vec2 fragmentUV;

out vec4 color;

void main() {
	color = vec4(1, 1, 1, texture2D(textureSampler, fragmentUV).r);
}