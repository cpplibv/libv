#version 330 core

#include <post_process/blur_kernel.glsl>

layout(location = 0) in vec3 vertexPosition;

uniform vec2 render_resolution; // Program block

out vec2 fragment_kernel_uvs[kernel_size];

void main() {
	gl_Position = vec4(vertexPosition, 1);

	vec2 uv = vertexPosition.xy * 0.5 + 0.5;
	vec2 pixel_size = 1.0 / render_resolution; // TODO P4: pixel_size could be a uniform

	for (int i = 0; i < kernel_size; i++)
		fragment_kernel_uvs[i] = uv + (i - kernel_half_size) * pixel_size.y;
}
