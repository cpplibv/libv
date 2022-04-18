#version 330 core

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture_noise;

void main() {
//	vec4 noise_sample = texture(texture_noise, fragmentTexture0, 0).rgba;
	vec4 noise_sample = texture(texture_noise, fragmentTexture0, 0).rgba;
	result = noise_sample;
//	result = vec4(fragmentTexture0, 0, 1);
}
