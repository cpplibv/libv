#version 330 core

#include <bloom/sample.glsl>

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture0;

// -------------------------------------------------------------------------------------------------

void main() {
	vec2 pixelSize = 0.5 / textureSize(texture0, 0); // NOTE: 0.5 comes from: We are upsampling so the bound texture is half as big as we are
	vec3 sample0 = upsampleTent9(texture0, fragmentTexture0, pixelSize, 2.0).rgb;
	result = vec4(sample0, 1);
}
