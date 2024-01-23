#version 330 core

#include <block/matrices.glsl>


uniform samplerCube textureSkybox;

in vec3 fragmentDirectionW;

out vec4 result;


uniform bool fogEnabled = true;
//uniform float fogIntensity = 0.05;
uniform vec4 fogColor = vec4(0.7, 0.8, 0.9, 1.0);


vec3 tonemapReinhard2(vec3 x) {
	const float L_white = 4.0;
	return (x * (1.0 + x / (L_white * L_white))) / (1.0 + x);
}

void main() {
	// Skybox but the horizon is pushed down a little to better match the fake spherical surface
	result = texture(textureSkybox, normalize(normalize(fragmentDirectionW.xzy) + vec3(0, 0.05, 0))).rgba;
	// Normal skybox
//	result = texture(textureSkybox, fragmentDirectionW.xzy).rgba;

	// HACK: Tonemap and gamma hack to support new HDR skys without the correct post processing pipeline
	result.rgb = tonemapReinhard2(result.rgb * 3.0);
	result.rgb = pow(result.rgb, vec3(1.0 / 2.2));

	// --- Fog ---

	if (fogEnabled)
//		result = mix(result, fogColor, smoothstep(0.15, -0.05, normalize(fragmentDirectionW).z));
		result = mix(result, fogColor, smoothstep(0.05, -0.10, normalize(fragmentDirectionW).z));
//		result = mix(result, fogColor, smoothstep(0.05, -0.15, normalize(fragmentDirectionW).z));
}
