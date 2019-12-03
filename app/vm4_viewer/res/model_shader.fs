#version 330 core

in vec3 fragmentPositionW;
in vec3 fragmentNormalW;
in vec3 fragmentTangentW;
in vec3 fragmentBitangentW;
in vec2 fragmentTexture0;

out vec4 output;

uniform int mode = 2;
uniform vec4 color;
uniform float near; // camera z near
uniform float far; // camera z far
//uniform sampler2D textureSampler;

float linearize_depth(float zoverw) {
	return (2.0 * near) / (far + near - zoverw * (far - near)) * (far - near);
}

void main() {
//vec4 sample = texture(textureSampler, fragmentTexture0).xyzw;
//vec4 sample = texture(textureSampler, fragmentTexture0);

	if (mode == 0)
		output = color;

	else if (mode == 1)
		output = vec4(abs(mod(fragmentPositionW, 5)) / 5, 1);
	else if (mode == 2)
		output = vec4(abs(fragmentNormalW), 1);
	else if (mode == 3)
		output = vec4(abs(fragmentTangentW), 1);
	else if (mode == 4)
		output = vec4(abs(fragmentBitangentW), 1);
	else if (mode == 5)
		output = vec4(fragmentTexture0, 0, 1);

	else if (mode == 6) {
		float depth = mod(linearize_depth(gl_FragCoord.z), 20) / 20;
		output = vec4(depth, depth, depth, 1);
	}

//	else if (mode == 10) {
//		vec4 sample = texture(textureSampler, fragmentTexture0);
//		output = sample;
//	}
}
