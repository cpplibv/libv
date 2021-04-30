#version 330 core

#include <blur_kernel.glsl>

in vec2 fragment_kernel_uvs[kernel_size];

uniform sampler2D input_texture;

out vec4 result;

void main() {
	result = vec4(0.0);
	for (int i = 0; i < kernel_size; i++)
		result += texture(input_texture, fragment_kernel_uvs[i]) * kernel_weight[i];
}
