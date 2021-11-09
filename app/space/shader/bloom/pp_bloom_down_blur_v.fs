#version 330 core

#include <bloom/sample.glsl>

in vec2 fragmentTexture0;

out vec4 result;

uniform sampler2D texture0;

// -------------------------------------------------------------------------------------------------

void main() {
	vec2 pixelSize = 1.0 / textureSize(texture0, 0); // NOTE: 1.0 comes from: We are at the sampe mip level as the previous horizontal pass
	vec3 sample0 = downsampleVerticalBlur5(texture0, fragmentTexture0, pixelSize).rgb;
	result = vec4(sample0, 1);
}
