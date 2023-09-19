#version 450 core

#include <shader/re/outline/jump_flood.glsl>

in vec2 fragmentUV;

layout(binding = TEXTURE_UNIT_DIFFUSE) uniform sampler2D textureDiffuse;
uniform float stepWidth;

out vec4 outputColor;

// -------------------------------------------------------------------------------------------------

void main() {
	outputColor = vec4(0, 0, 0, 1);

	// Deal with aspect ratios to have simpler steps later, Have to down scale to fit into 16_UNORM
	vec2 fragmentPosition = uvToPosition(fragmentUV);

	float closestDistanceSQ = inf;
	vec2 closestPosition = FLOOD_NULL2;

	// Find closest from jump samples
	for (int u = -1; u <= 1; ++u) {
		for (int v = -1; v <= 1; ++v) {
			vec2 probeUVOffset = vec2(u, v) * stepWidth * pixelSize;
			vec2 probeUV = fragmentUV + probeUVOffset;

			vec2 probePosition = texture(textureDiffuse, probeUV).rg;
			if (isNullPosition(probePosition))
				continue;

			vec2 diff = fragmentPosition - probePosition;
			float distanceSQ = dot(diff, diff);

			if (closestDistanceSQ > distanceSQ) {
				closestDistanceSQ = distanceSQ;
				closestPosition = probePosition;
			}
		}
	}

	outputColor.xy = closestPosition;
}
