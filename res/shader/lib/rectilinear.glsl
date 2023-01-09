#pragma once


vec2 dirToRectilinear(vec3 dir) {
	float x = atan2(dir.y, dir.x) / tau + 0.5;
	float y = dir.z * 0.5 + 0.5;
	return vec2(x, y);
}
