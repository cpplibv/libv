#pragma once

#include <shader/lib/constants.glsl>


// equirectangular, rectilinear
vec2 dirToEquirectangularUV(vec3 dir) {
	// dir is assumed to be unit length
	float u = atan(dir.y, dir.x) / tau + 0.5;
	float v = asin(dir.z) / pi + 0.5;
	return vec2(u, v);
}
