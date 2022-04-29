#pragma once

struct SpriteDefinition {
	ivec2 tile_size;
	int tile_num_x;
	int tile_num_y;
	vec3 model_offset;
	float model_scale;
	float angle_x_min;
	float angle_x_max;
	float angle_y_min;
	float angle_y_max;
};

layout(std140) uniform SpriteDefinitions {
	SpriteDefinition spriteDefinitions[64];
};
