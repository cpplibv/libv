#version 330 core

#include <rev_sandbox/block/matrices.glsl>


in vec3 fragmentPositionW;
//in vec4 fragmentColor;
in vec3 fragmentNormalW;
//in vec3 fragmentTangentW;
//in vec3 fragmentBitangentW;
in vec2 fragmentTexture0;

out vec4 result;
//layout(location = 0) out vec4 resultColor;
//layout(location = 1) out vec3 resultNormal;
//layout(location = 2) out float resultDepth;

uniform sampler2D texture0;

// -------------------------------------------------------------------------------------------------

void main() {
	vec3 N = normalize(fragmentNormalW);

	N = gl_FrontFacing ? N : -N; // If the two sided normal looks away from the camera, flip it to point toward the camera

	vec4 color = texture(texture0, fragmentTexture0).rgba;
	if (color.a < 0.1)
		discard;
	else
//		color.a = 1.0;
		color.a = (1 - (1 - color.a) * (1 - color.a));

//	result = vec4(N, 1);
	result = color;
//	result = vec4(1 - vec3(gl_FragCoord.z * 1.5), 1);

//	resultColor = vec4(N, 1);
//	resultNormal = color;
//	resultDepth = vec4(1 - vec3(gl_FragCoord.z * 1.5), 1);
}