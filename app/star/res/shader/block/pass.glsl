#pragma once

layout(std140) uniform Pass {
    float time_sim;
    float time_real;
    float time_real_hcap;
    vec2 render_resolution;
    vec2 pixel_size; // 1.0 / render_resolution

	int test_mode0;

    vec4 frame_random_vec4; // [0..1] random numbers changing each frame
    vec4 frame_random_ivec4; // [int_min..int_max] random numbers changing each frame
};
