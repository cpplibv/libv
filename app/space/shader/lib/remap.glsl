#pragma once


float lerp(float to_lo, float to_hi, float value) {
	return mix(to_lo, to_hi, value);
}

float inv_lerp(float from_lo, float from_hi, float value) {
	return (value - from_lo) / (from_hi - from_lo);
}

float remap(float from_lo, float from_hi, float to_lo, float to_hi, float value) {
	return to_lo + (value - from_lo) * (to_hi - to_lo) / (from_hi - from_lo);
}

vec2 remap(vec2 from_lo, vec2 from_hi, vec2 to_lo, vec2 to_hi, float value) {
	return to_lo + (value - from_lo) * (to_hi - to_lo) / (from_hi - from_lo);
}

vec3 remap(vec3 from_lo, vec3 from_hi, vec3 to_lo, vec3 to_hi, float value) {
	return to_lo + (value - from_lo) * (to_hi - to_lo) / (from_hi - from_lo);
}

vec4 remap(vec4 from_lo, vec4 from_hi, vec4 to_lo, vec4 to_hi, float value) {
	return to_lo + (value - from_lo) * (to_hi - to_lo) / (from_hi - from_lo);
}
