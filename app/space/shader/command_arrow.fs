#version 330 core

in vec3 fragmentNormal;
in vec2 fragmentTexture0;
in float fragmentSegmentPosition;
in float fragmentSegmentSize;
in float fragmentTotalPosition;
in float fragmentTotalSize;

out vec4 result;

#include <block/sphere.glsl>

#include <lib/mask_aa.glsl>

float sdTriangle(in vec2 p, in vec2 p0, in vec2 p1, in vec2 p2) {
	vec2 e0 = p1 - p0;
	vec2 e1 = p2 - p1;
	vec2 e2 = p0 - p2;

	vec2 v0 = p - p0;
	vec2 v1 = p - p1;
	vec2 v2 = p - p2;

	vec2 pq0 = v0 - e0 * clamp(dot(v0, e0) / dot(e0, e0), 0.0, 1.0);
	vec2 pq1 = v1 - e1 * clamp(dot(v1, e1) / dot(e1, e1), 0.0, 1.0);
	vec2 pq2 = v2 - e2 * clamp(dot(v2, e2) / dot(e2, e2), 0.0, 1.0);

	float s = e0.x * e2.y - e0.y * e2.x;
	vec2 d = min(min(vec2(dot(pq0, pq0), s * (v0.x * e0.y - v0.y * e0.x)),
	vec2(dot(pq1, pq1), s * (v1.x * e1.y - v1.y * e1.x))),
	vec2(dot(pq2, pq2), s * (v2.x * e2.y - v2.y * e2.x)));

	return -sqrt(d.x) * sign(d.y);
}

void main() {
	const vec4 color_body = vec4(0.60, 0.60, 0.60, 1);
	const vec4 color_edge = vec4(0.85, 0.85, 0.85, 1);
	const vec4 color_head = vec4(0.85, 0.30, 0.30, 1);
	const float body_size = 0.400;
	const float edge_size = 0.075;
	const float head_size = 0.125;

	float distance_from_middle = abs(fragmentTexture0.x - 0.5);

	result = vec4(color_body.rgb, 0);

	float body_sdf = distance_from_middle - body_size * 0.5;
	result = mix(result, color_body, mask_aa(body_sdf));

	float edge_sdf = distance_from_middle - body_size * 0.5 + edge_size;
	result = mix(result, color_edge, mask_aa(edge_sdf));

	float head_x = fragmentTexture0.x;
	float head_y = fragmentSegmentSize - fragmentSegmentPosition - head_size;
	float head_sdf = sdTriangle(vec2(head_x, head_y), vec2(0.5, -head_size), vec2(0, 0), vec2(1, 0));
	result = mix(result, color_head, mask_aa(head_sdf));
}
