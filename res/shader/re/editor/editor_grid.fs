#version 330 core

#include <shader/block300/model300.glsl>
#include <shader/block300/pass300.glsl>
#include <shader/lib/constants.glsl>
#include <shader/lib/mask_aa.glsl>
#include <shader/lib/remap.glsl>

in vec3 fragmentPositionM;
in float lod_ramp;
in float lod_level_index;
in float lod_transition;
in float lod_level;

out vec4 result;


// -------------------------------------------------------------------------------------------------

const vec4 color_axes_x = vec4(0.93, 0.16, 0.16, 1);
const vec4 color_axes_y = vec4(0.16, 0.93, 0.16, 1);
const vec4 color_axes_z = vec4(0.16, 0.16, 0.93, 1);

// -------------------------------------------------------------------------------------------------

float grid(vec3 positionW, float cell_size, float line_width) {
	float scale = 1 / cell_size;

	vec2 coord = positionW.xy * scale;
	// NOTE: fwidth split into two, as 2D call caused artifact on one axis
	vec2 derivative = vec2(fwidth(coord.x), fwidth(coord.y)) * line_width;
	vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;

	float line = min(grid.x, grid.y);
	float minimumx = min(derivative.x, 1);
	float minimumy = min(derivative.y, 1);

	return max(1.0 - line, 0.0);
}

vec2 calc_axis(vec3 positionW, float line_width) {
	vec2 coord = positionW.xy;
	// NOTE: fwidth split into two, as 2D call caused artifact on one axis
	vec2 derivative = vec2(fwidth(coord.x), fwidth(coord.y)) * line_width;
	vec2 lines = abs(coord) / derivative;
	return max(vec2(0, 0), 1 - lines).yx;
}

vec4 weight_mix(vec4 x, vec4 y, float wx, float wy) {
	// NOTE: Based on a Weighted Blended Order-Independent Transparency paper but
	// But implemented incorrectly to help this shader out and save me a couple of hour
	vec3 sum_rgb = x.rgb * x.a * wx + y.rgb * y.a * wy;
	float sum_a = x.a * wx + y.a * wy;

	return vec4(sum_rgb, sum_a);
	// The correct implemenation would end like this
	//	float prod_a = (1.0 - x.a) * (1.0 - y.a);
	//	return vec4(sum_rgb / clamp(sum_a, 1e-4, 5e4), 1 - prod_a);
}

// -------------------------------------------------------------------------------------------------

void main() {
	const float camera_far = 1000;
	const float plane_size_far_rate = 0.25; // was 0.8
	const float grid_alpha = 0.75; // was 1

	const float plane_size_far = camera_far * plane_size_far_rate;
	float plane_size = 100 + abs(eyeW.z) * 100;

	float eye_distance = length(eyeW - fragmentPositionM);

	float fading_0 = max(0, 1 - inv_lerp(0, lod_level * 100 *    .1, eye_distance));
	float fading_1 = max(0, 1 - inv_lerp(0, lod_level * 100 *   1.0, eye_distance));
	float fading_2 = max(0, 1 - inv_lerp(0, lod_level * 100 *  10.0, eye_distance));
	float fading_3 = max(0, 1 - inv_lerp(0, lod_level * 100 * 100.0, eye_distance));
	fading_3 *= lod_transition;

	// Linear fade out to the edge of the whole grid quad
	float fading_grid_global = max(0, 1 - inv_lerp(0, plane_size_far, eye_distance));

	// Linear fade out if incident angle is too shallow (< 2°)
	float incident_threshold = 1 / (180 / pi);
	vec3 E = normalize(eyeW - fragmentPositionM);
	vec3 L = vec3(0, 0, 1);
	float incident_angle = abs(dot(E, L));
	float fading_grid_angle = min(1, incident_angle / incident_threshold);

	const float grid_line_width_base = 0.9;
	const float grid_line_width_per_level = 0.1;
	const float grid_line_width_axis = 1.5;

	vec2 axis = calc_axis(fragmentPositionM, grid_line_width_axis);
	axis = 1 - pow(1 - axis, vec2(2)); // More sharper line body
	axis *= (fading_grid_global < 0.01 ? 0 : 1);
	axis *= fading_grid_angle;
	// TODO P4: The end of axes on the far cutof looking slanted

	float line_width = grid_line_width_base + grid_line_width_per_level * (1.0 - lod_transition);

	float grid_0 = grid(fragmentPositionM, lod_level *    .1, line_width + 0 * grid_line_width_per_level);
	float grid_1 = grid(fragmentPositionM, lod_level *   1.0, line_width + 1 * grid_line_width_per_level);
	float grid_2 = grid(fragmentPositionM, lod_level *  10.0, line_width + 2 * grid_line_width_per_level);
	float grid_3 = grid(fragmentPositionM, lod_level * 100.0, line_width + 2 * grid_line_width_per_level); // Matches the previous line witdh to allow seamless pop in

	const float level_power_0 = 0.2;
	const float level_power_1 = 0.35;
	const float level_power_2 = 0.50;
	const float level_power_3 = level_power_2;

	float grid = 0;
	grid = max(grid, grid_0 * fading_0 * mix(level_power_0, level_power_1, 1 - lod_transition));
	grid = max(grid, grid_1 * fading_1 * mix(level_power_1, level_power_2, 1 - lod_transition));
	grid = max(grid, grid_2 * fading_2 * mix(level_power_2, level_power_3, 1 - lod_transition));
	grid = max(grid, grid_3 * fading_3 * level_power_3);
	grid *= min(fading_grid_global, fading_grid_angle);

	// Fake lighting: Little extra kick for lighting effect on the grid based on the angle
	grid += min(1, grid * pow(incident_angle, 32));

	result = vec4(vec3(0.6), grid * grid_alpha);

	float grid_exclusion = 1 - max(axis.x, axis.y);
	result *= grid_exclusion;
	result += weight_mix(color_axes_y, color_axes_x, axis.y, axis.x);
}
