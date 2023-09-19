#version 450 core

#include <shader/block300/pass300.glsl>
#include <shader/re/outline/jump_flood.glsl>

in vec2 fragmentUV;

layout(binding = TEXTURE_UNIT_DIFFUSE) uniform sampler2D textureDiffuse;

out vec4 outputColor;

// -------------------------------------------------------------------------------------------------

void main() {
	outputColor = vec4(0, 0, 0, 1);

	// Sample silhouette texture for sobel
	mat3 values;
	for (int u = 0; u < 3; ++u) {
		for (int v = 0; v < 3; ++v) {
			vec2 uv = fragmentUV + vec2(u - 1, v - 1) * pixelSize;
			values[u][v] = texture(textureDiffuse, uv).r;
		}
	}

	vec2 fragmentPosition = uvToPosition(fragmentUV);

	// Interior, return position
	if (values[1][1] > 0.99) {
		outputColor.xy = fragmentPosition;
		return;
	}

	// Exterior, return no position
	if (values[1][1] < 0.01) {
		outputColor.xy = FLOOD_NULL2;
		return;
	}

	// Sobel to estimate edge direction
	vec2 dir = -vec2(
		values[0][0] + values[0][1] * 2.0 + values[0][2] - values[2][0] - values[2][1] * 2.0 - values[2][2],
		values[0][0] + values[1][0] * 2.0 + values[2][0] - values[0][2] - values[1][2] * 2.0 - values[2][2]
	);

	// If dir length is small, this is either a sub pixel dot or line
	// no way to estimate sub pixel edge, so output position
	if (abs(dir.x) <= 0.005 && abs(dir.y) <= 0.005) {
		outputColor.xy = fragmentPosition;
		return;
	}

	// Normalize direction
	dir = normalize(dir);

	// Sub pixel offset
	vec2 offset = dir * (1.0 - values[1][1]) * pixelSize;

	// Output encoded offset position
	outputColor.xy = uvToPosition(fragmentUV + offset);
}
