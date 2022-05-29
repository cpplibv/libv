#version 330 core

#include <block/matrices.glsl>

layout(location = 0) in vec3 vertexPosition;

out vec3 fragmentPositionM;
out float lod_ramp;
out float lod_level_index;
out float lod_transition;
out float lod_level;



float log10(float v) {
	const float inv_log_10 = 1.0 / log(10.0);
	return log(v) * inv_log_10;
}

void main() {
	const float grid_size_min = 0.001;
	float plane_size = 100 + abs(eye.z) * 100;

	vec3 posM = vertexPosition * plane_size + vec3(eye.xy, 0);

	gl_Position = matMVP * vec4(posM, 1);
	fragmentPositionM = posM;
	lod_ramp = log10(abs(eye.z));
	lod_level_index = floor(lod_ramp);
	lod_transition = fract(lod_ramp);
	lod_level = max(grid_size_min, pow(10.0, lod_level_index));
}
