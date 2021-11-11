#pragma once

vec3 contrast(in vec3 color, in float contrast_value) {
	return (color - 0.5) * (1.0 + contrast_value) + 0.5;
}

vec4 contrast(in vec4 color, in float contrast_value) {
	return vec4(contrast(color.rgb, contrast_value), color.a);
}
