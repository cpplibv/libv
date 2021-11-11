#pragma once

vec3 gamma_correction(in vec3 color, in float gamma) {
	return pow(color, vec3(1.0 / gamma));
}
