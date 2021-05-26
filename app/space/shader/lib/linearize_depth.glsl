#pragma once

float linearize_depth(in float zoverw, in float near, in float far) {
	return (2.0 * near) / (far + near - zoverw * (far - near)) * (far - near);
}

float to_depth(in float d, in float near, in float far) {
	return (d * (far + near) + 2.0 * near * (near - far)) / (d * (far - near));
}
