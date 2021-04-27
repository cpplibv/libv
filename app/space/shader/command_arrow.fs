#version 330 core

#include <command_arrow.glsl>
#include <block/sphere.glsl>
#include <lib/mask_aa.glsl>
#include <lib/sd_triangle.glsl>

in FragmentData fragment;

out vec4 result;

void main() {
	const vec4 color_body = vec4(0.60, 0.60, 0.60, 1);
	const vec4 color_edge = vec4(0.85, 0.85, 0.85, 1);
	const vec4 color_head = vec4(0.50, 0.70, 0.70, 1);
	const float body_size = 0.400;
	const float edge_size = 0.075;
	const float head_size = 0.125;

	float distance_from_middle = abs(fragment.texture0.x - 0.5);

	result = vec4(color_body.rgb, 0);

	float body_sdf = distance_from_middle - body_size * 0.5;
	result = mix(result, color_body, mask_aa(body_sdf));

	float edge_sdf = distance_from_middle - body_size * 0.5 + edge_size;
	result = mix(result, color_edge, mask_aa(edge_sdf));

	float head_x = fragment.texture0.x;
	float head_y = fragment.segmentSize - fragment.segmentPosition - head_size;
	float head_sdf = sd_triangle(vec2(head_x, head_y), vec2(0.5, -head_size), vec2(0, 0), vec2(1, 0));
	result = mix(result, color_head, mask_aa(head_sdf));
}
