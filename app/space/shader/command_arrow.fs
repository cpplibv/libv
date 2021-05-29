#version 330 core

#include <command_arrow.glsl>
//#include <lib/linearize_depth.glsl>

in FragmentData fs_in;

out vec4 result;

//uniform vec4 base_color; // base_color is most likely an attribute
uniform float time;


void main() {
	// Debug inputs
	//	result.x = fs_in.segmentPosition / fs_in.segmentSize;
	//	result.x = fs_in.totalPosition / fs_in.totalSize;
	//	result.x = fs_in.head;
	//	result.xy = fs_in.uv;
	//	result.xy = fs_in.part_uv;
	//	result.r = fract(linearize_depth(gl_FragCoord.z, 0.01, 1000) / 2 + time / 5);

	// --- START: Test only base color selection
	const vec4 base_color_c = vec4(0.48, 0.65, 0.70, 0.5);
	const vec4 base_color_r = vec4(0.80, 0.30, 0.30, 0.5);

//	vec4 base_color = fract(fs_in.totalPosition / 15 - time / 50) > 0.5 ?
	vec4 base_color = fract((fs_in.totalPosition - fs_in.uv.y * fs_in.segmentSize) / 15 - time / 50) > 0.5 ?
			base_color_c :
			base_color_r;
	// --- NORMAL: Test only base color selection
	//	vec4 base_color = fs_in.color;
	// --- END: Test only base color selection

	// Base color
	result = base_color;

	// Edge/Border highlight
	const float border_pow = 2;

	// TODO P1: Fix head border
	result += pow(abs(fs_in.part_uv.x - 0.5), border_pow);

	// Running blink effect on body
	const float blink_distance  = 5.00; // [blink_length..inf] Unit distance between blink effect
	const float blink_length    = 0.25; // [0..blink_distance] Unit lenght of a blink effect
	const float blink_strength  = 0.85; // [0..1] Blink effect opacity
	const float blink_run_speed = 0.50; // Unit / sec, Blink running speed

	float blink_time_offset = -time * blink_run_speed;
	float blink_t = fract((fs_in.totalPosition + blink_time_offset) / blink_distance);
	float blink_lin_v = max(0, blink_t / blink_length * blink_distance - blink_distance / blink_length + 1);
	float blink_v = pow(blink_lin_v, 4);

	result += (1 - fs_in.head) * (1 - base_color) * blink_v * blink_strength;

	// Running blink effect lingering head
	const float blink_head_linger = 6;
	const float blink_head_linger_fase_pow = 2;

	float blink_head_lin_v = max(0, blink_t / blink_head_linger / blink_length * blink_distance - blink_distance / blink_length / blink_head_linger + 1);
	float blink_head_v = pow(blink_head_lin_v, blink_head_linger_fase_pow);

	result += (fs_in.head) * (1 - base_color) * blink_head_v * blink_strength;

	// Gradient in segment
	// TODO P4: Segment gradient
}




//	result.x = mask_aa(abs(fs_in.part_uv.x - 0.5) * body_width - 1.4);
