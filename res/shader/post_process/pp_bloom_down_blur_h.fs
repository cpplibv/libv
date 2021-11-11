#version 330 core

#include <lib/sample.glsl>

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture0;

// -------------------------------------------------------------------------------------------------

void main() {
	vec2 pixelSize = 2.0 / textureSize(texture0, 0); // NOTE: 2.0 comes from: We are downsampling so the bound texture is twice as big as we are
	vec3 sample0 = downsampleHorizontalBlur9(texture0, fragmentTexture0, pixelSize).rgb;
	result = vec4(sample0, 1);
}
