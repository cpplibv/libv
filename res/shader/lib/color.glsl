#pragma once


// -------------------------------------------------------------------------------------------------

/// All components are in the range [0…1], including hue.
/// Source: http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 hsv_to_rgb(const vec3 c) {
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

/// All components are in the range [0…1], including hue.
/// Source: http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 rgb_to_hsv(const vec3 c) {
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 normalize_hsv(const vec3 hsvColor) {
	return vec3(
		fract(hsvColor.r),
		clamp(hsvColor.gb, 0, 1)
	);
}

// -------------------------------------------------------------------------------------------------
