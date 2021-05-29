#version 330 core

#include <block/matrices.glsl>
#include <command_arrow.glsl>
#include <lib/linearize_depth.glsl>
#include <lib/mask_aa.glsl>
//#include <lib/sd_triangle.glsl>

in FragmentData fs_in;

out vec4 result;

uniform vec4 color;
uniform float time;


void main() {
	float depth = fract(linearize_depth(gl_FragCoord.z, 0.01, 1000) / 2 + time / 5);
//	float depth = fract(linearize_depth(gl_FragCoord.z, 0.01, 1000) / 2);
//	float depth = (linearize_depth(gl_FragCoord.z, 0.01, 1000));
//	result = color * fs_in.color;
//	result.a = depth;
//	result = color;
	result = vec4(0, 0, 0, 1);

	result.x = fract((fs_in.totalPosition) / fs_in.totalSize * 10 - time / 3);
//	result.x = fract(fs_in.totalPosition);

	// Input debugs
	//	result.x = fs_in.segmentPosition / fs_in.segmentSize;
	//	result.x = fs_in.totalPosition / fs_in.totalSize;
	//	result.x = fs_in.head;
	//	result.xy = fs_in.uv;
	//	result.xy = fs_in.part_uv;
	//	result.r = depth;


	// TODO P5: Visual improvments of command arrow
	// 			- Pulse pattern in whole arrow, with spark and/or (small arrow) pattern
	// 			- Body borders
	// 			- Gradient in segments
	// 			? Highlight Arrow end with slower fade out when pulse goes through it

//	result.a *= 0.5;
//	result.x = fract(linearize_depth(gl_FragDepth, 0.01, 1000) * 500000);
//	result.xyz = vec3(1) * ;
//	result.xyz = vec3(1) * linearize_depth(gl_FragDepth, 0.01, 1000);
//	result.xyz = vec3(1) * linearize_depth(gl_FragCoord.z, 0.01, 1000);

//	result.x = fs_in.segmentPosition;

//	const vec4 color_body = vec4(0.60, 0.60, 0.60, 1);
//	const vec4 color_edge = vec4(0.85, 0.85, 0.85, 1);
//	const vec4 color_head = vec4(0.50, 0.70, 0.70, 1);
//	const float body_size = 0.400;
//	const float edge_size = 0.075;
//	const float head_size = 0.125;
//
//	float distance_from_middle = abs(fs_in.texture0.x - 0.5);
//
//	result = vec4(color_body.rgb, 0);
//
//	float body_sdf = distance_from_middle - body_size * 0.5;
//	result = mix(result, color_body, mask_aa(body_sdf));
//
//	float edge_sdf = distance_from_middle - body_size * 0.5 + edge_size;
//	result = mix(result, color_edge, mask_aa(edge_sdf));
//
//	float head_x = fs_in.texture0.x;
//	float head_y = fs_in.segmentSize - fs_in.segmentPosition - head_size;
//	float head_sdf = sd_triangle(vec2(head_x, head_y), vec2(0.5, -head_size), vec2(0, 0), vec2(1, 0));
//	result = mix(result, color_head, mask_aa(head_sdf));
}
