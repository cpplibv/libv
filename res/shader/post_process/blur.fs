#version 330 core

#include <post_process/blur_kernel.glsl>

in vec2 fragment_kernel_uvs[kernel_size];

uniform sampler2D input_texture;

out vec4 outputColor;

void main() {
	outputColor = vec4(0.0);
	for (int i = 0; i < kernel_size; i++)
		outputColor += texture(input_texture, fragment_kernel_uvs[i]) * kernel_weight[i];
}
