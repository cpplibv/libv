#version 330 core

uniform sampler2D textureDiffuseSampler;

in vec2 fragmentUV;

out vec4 color;

void main() {
	color = texture(textureDiffuseSampler, fragmentUV, 0).rgba;
}
