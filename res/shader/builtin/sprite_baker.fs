#version 330 core

#include <block/matrices.glsl>


in vec3 fragmentPositionW;
//in vec4 fragmentColor;
in vec3 fragmentNormalW;
//in vec3 fragmentTangentW;
//in vec3 fragmentBitangentW;
in vec2 fragmentTexture0;


//out vec4 result;
layout(location = 0) out vec4 resultColor;
layout(location = 1) out vec4 resultNormal;
//layout(location = 2) out float resultDepth;



uniform sampler2D texture0;

// -------------------------------------------------------------------------------------------------



void main() {
	vec3 N = normalize(fragmentNormalW);

//	N = gl_FrontFacing ? N : -N; // If the two sided normal looks away from the camera, flip it to point toward the camera
	// NOTE: Models with 'fixed' vertex normals the inversion would break the normal vectors

	//gvec textureLod(gsampler sampler, vec texCoord, float lod);
	vec4 color = textureLod(texture0, fragmentTexture0, 0).rgba;
//	vec4 color = texture(texture0, fragmentTexture0).rgba;
	if (color.a < 0.1)
		discard;
	else
//		color.a = 1.0;
		color.a = (1 - (1 - color.a) * (1 - color.a));

//	result = color;
//	result = vec4(N, 1);
//	result = vec4(1 - vec3(gl_FragCoord.z * 1.5), 1);

//	resultColor = color;
//	resultNormal = N;

//	vec3 V = normalize(eye - fragmentNormalW);

	resultColor = color;
//	resultColor = vec4(1, 1, 1, 1);
	resultNormal = vec4(N * 0.5 + 0.5, 1);



//	resultColor = vec4(resultNormal.rgb, 1);



//	resultNormal = vec4(normalize(fragmentPositionW - vec3(0, 0, 1)), 1);
//	resultNormal = vec4(normalize(fragmentPositionW - vec3(0, 0, 30)) * 0.5 + 0.5, 1);
//	resultNormal = vec4(mix(N, V, 0.5) * 0.5 + 0.5, 1);
//	resultNormal = vec4(V * 0.5 + 0.5, 1);
//	resultNormal = vec4(-V, 1);
//	resultNormal = vec4(N, 1);
//	resultNormal = vec4(.5, .5, 1, 1);
//	resultColor = vec4(step(0.9, N), 1);
//	resultColor.rgb = resultNormal.rgb * 2 - 1;

//	resultColor.rgb = N;
}
