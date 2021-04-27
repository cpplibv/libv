#pragma once

float saturate(in float v) {
	return clamp(v, 0.0, 1.0);
}
