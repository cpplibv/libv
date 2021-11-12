#version 330 core

#include <command_arrow.glsl>
//#include <lib/linearize_depth.glsl>

in FragmentData fs_in;

out vec4 result;

uniform float time;


void main() {
	// Debug inputs
//	result = vec4(0, 0, 0, 1);
//	result.x = fs_in.segmentPosition / fs_in.segmentSize;
//	result.x = fs_in.chainPosition / fs_in.chainSize;
//	result.x = fs_in.chainPosition / fs_in.chainSize;
//	result.x = fs_in.chainPosition / fs_in.chainSize;
//	result.x = fract(fs_in.chainPosition);
//	result.a = 1;
	//	result.x = fs_in.head;
	//	result.xy = fs_in.uv;
	//	result.xy = fs_in.part_uv;
	//	result.r = fract(linearize_depth(gl_FragCoord.z, 0.01, 1000) / 2 + time / 5);

	vec4 base_color = fs_in.color;

	// Base color
	result = base_color;

	// Edge highlight
	const float border_pow = 2;
	const float border_alpha_buff = 0.5; // [0..1] How much should the border effect drag the alpha to 1

	float border_dist_body = abs(fs_in.part_uv.x - 0.5); // Border is [0..0.5] range
	float border_dist_head = abs(fs_in.part_uv.x - 0.5) + fs_in.part_uv.y * 0.5;

	float border_body = (1 - fs_in.head) * pow(border_dist_body, border_pow);
	float border_head = (fs_in.head) * pow(border_dist_head, border_pow);
	float border = border_head + border_body;

	result.rgb += border;
	result.a += abs(border - result.a) * border_alpha_buff;

	// Running spark effect on body
	const float spark_distance  = 5.00; // [spark_length..inf] Unit distance between spark effect
	const float spark_length    = 0.25; // [0..spark_distance] Unit lenght of a spark effect
	const float spark_strength  = 0.65; // [0..1] Blink effect opacity
	const float spark_run_speed = 0.50; // Unit / sec, Blink running speed
	const float spark_fade_pow  = 4;
		// Another variant, big and fast with less strength
		//	const float spark_distance  = 25.00; // [spark_length..inf] Unit distance between spark effect
		//	const float spark_length    = 1.25; // [0..spark_distance] Unit lenght of a spark effect
		//	const float spark_strength  = 0.30; // [0..1] Blink effect opacity
		//	const float spark_run_speed = 5.50; // Unit / sec, Blink running speed
		//	const float spark_fade_pow  = 4;

	float spark_time_offset = -time * spark_run_speed + fs_in.animationOffset;
	float spark_shape_delay = abs(fs_in.uv.x - 0.5) * 0.5;
	float spark_t = fract((fs_in.chainPosition + spark_shape_delay + spark_time_offset ) / spark_distance);
	float spark_lin_v = max(0, spark_t / spark_length * spark_distance - spark_distance / spark_length + 1);
	float spark_v = pow(spark_lin_v, spark_fade_pow);

	result += (1 - fs_in.head) * (1 - base_color) * spark_v * spark_strength;

	// Running spark effect lingering head
	const float spark_head_linger = 6;
	const float spark_head_linger_fade_pow = 2;

	float spark_head_lin_v = max(0, spark_t / spark_head_linger / spark_length * spark_distance - spark_distance / spark_length / spark_head_linger + 1);
	float spark_head_v = pow(spark_head_lin_v, spark_head_linger_fade_pow);

	result += (fs_in.head) * (1 - base_color) * spark_head_v * spark_strength;

	// Gradient in segment
	// TODO P4: Segment gradient

	result = clamp(result, 0, 1);
}
