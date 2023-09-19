#version 450 core

#include <shader/block300/pass300.glsl>

in vec2 fragmentUV;

layout(binding = TEXTURE_UNIT_DIFFUSE) uniform sampler2D textureDiffuse;

out vec4 outputColor;

// -------------------------------------------------------------------------------------------------

void main() {
	vec2 uv = fragmentUV;
	ivec2 size = textureSize(textureDiffuse, 0);

//	if (true) {
	if (false) {
		// Center
		if (canvasSize != size) {
			vec2 ratios = vec2(canvasSize) / vec2(size);
			float minRatio = min(ratios.x, ratios.y);
			float scale = 1.0 / min(minRatio, 1.0);
			uv = uv * ratios * scale - (ratios * scale - 1.0) / 2.0;
		}
	} else {
		// Strech
//	} else {
		// TODO P5: Implement Fit (scale size to max that still fits)
		// Fit
	}

	// Checker outside
	if (uv.x < 0 || uv.y < 0 || uv.x >= 1 || uv.y >= 1) {
		const float gridBoxSize = 8.0;
		vec2 v = fract(fragmentUV * canvasSize / gridBoxSize / 2.0);
		if (v.x < 0.5 != v.y < 0.5)
			outputColor = vec4(0.21, 0.21, 0.21, 1);
		else
			outputColor = vec4(0.15, 0.15, 0.15, 1);
		return;
	}

	outputColor = texture(textureDiffuse, uv).rgba;
}

