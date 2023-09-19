#version 330 core

in vec2 fragmentTexture0;

out vec4 result;

uniform float progress = 1.0;

uniform sampler2D texture0;

// -------------------------------------------------------------------------------------------------

void main() {
	vec2 cross_mask = step(0.45, fragmentTexture0) * step(0.45, 1.0 - fragmentTexture0);

	result = vec4(1, fragmentTexture0 + 4.0 * cross_mask, 1);

	result = texture(texture0, fragmentTexture0).rgba;
}
