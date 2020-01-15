#pragma once

vec3 gamma_correction(vec3 color, float gamma) {
	return pow(color, vec3(1.0 / gamma));
}
