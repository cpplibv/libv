#version 330 core

// -------------------------------------------------------------------------------------------------

in vec2 fragmentTexture0;
in vec4 fragmentColor0;

// -------------------------------------------------------------------------------------------------

out vec4 result;

// -------------------------------------------------------------------------------------------------

uniform sampler2D texture0;

// -------------------------------------------------------------------------------------------------

void main() {
	result = texture(texture0, fragmentTexture0).rgba * fragmentColor0;
//	result = fragmentColor0 * vec4(1, 1, 1, texture(texture0, fragmentTexture0).a);
//	result = fragmentColor0 * vec4(1, 0, 0, 0.5);
//	result = fragmentColor0 * vec4(1, 1, 1, 0.5);
}
