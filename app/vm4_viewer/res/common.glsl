#pragma once

float linearize_depth(float zoverw, float near, float far) {
	return (2.0 * near) / (far + near - zoverw * (far - near)) * (far - near);
}
