
local uniform_block = plugin("uniform_block")

namespace("libv::rev")

uniform_block.generate(2, "Pass200", "res/shader/block/pass200.glsl", {
	{"vec2", "render_resolution"},
	{"vec2", "pixel_size"}, -- 1.0 / render_resolution
	{"float", "time_sim"}, -- sim time with mod 3600
	{"float", "time_sim_uncapped"},
	{"float", "time_real"}, -- real time with mod 3600
	{"float", "time_real_uncapped"},
	--{"vec4", "frame_random_float"}, -- [0..1] random numbers changing each frame
	--{"vec4i", "frame_random_int"}, -- [int_min..int_max] random numbers changing each frame
})
