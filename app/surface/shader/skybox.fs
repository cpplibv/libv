#version 330 core

#include <block/matrices.glsl>


uniform samplerCube textureSkybox;

in vec3 fragmentDirectionW;

out vec4 result;


//uniform bool fogEnabled = true;
//uniform float fogIntensity = 0.05;
//uniform vec4 fogColor = vec4(1, 1, 1, 1);


void main() {
	result = texture(textureSkybox, fragmentDirectionW).rgba;
//	result = vec4(1, 0, 0, 1);

	vec4 fogColor = vec4(0.7, 0.8, 0.9, 1.0);
	result = mix(result, fogColor, smoothstep(0.15, -0.05, normalize(fragmentDirectionW).z));

	//	// --- Fog ---
	//
	//	if (fogEnabled) {
	//		float dist = length(eye - fragmentPositionW);
	////		float dist = gl_FragCoord.z / gl_FragCoord.w;
	//
	////		float fogAmount = fogFactorLinear(dist, 1, 10);
	////		float fogAmount = fogFactorExp(dist, fogIntensity);
	//		fragmentFogFactor = fogFactorExp2(dist, fogIntensity);
	//
	//	} else {
	//		fragmentFogFactor = 0;
	//	}
}
