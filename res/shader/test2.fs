#version 330 core

uniform samplerCube textureSkySampler;

uniform vec3 eyePosW;

in vec3 fragmentPosW;

out vec4 color;

void main() {
	color = texture(textureSkySampler, normalize(fragmentPosW - eyePosW)).rgba;
}