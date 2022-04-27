#pragma once


/// Usage:
///
///		Uniforms:
///			uniform vec3 fogColor = vec3(0.8, 0.9, 1);
///			uniform float fogIntensity = 0.05;
///
///			varying fogAmount;
///
///		Vertex Shader - Linear:
///			float fogAmount = fogFactorLinear(dist, 1, 10);
///
///		Vertex Shader - Exp:
/// 		float dist = gl_FragCoord.z / gl_FragCoord.w;
///			float fogAmount = fogFactorExp(dist, fogIntensity);
///
///		Vertex Shader - Exp2:
/// 		float dist = gl_FragCoord.z / gl_FragCoord.w;
///			float fogAmount = fogFactorExp2(dist, fogIntensity);
///
///		Fragment shader:
///			result.rgb = mix(result.rgb, fogColor, fogAmount);


float fogFactorLinear(const float dist, const float start, const float end) {
	return 1.0 - clamp((end - dist) / (end - start), 0.0, 1.0);
}

float fogFactorExp(const float dist, const float density) {
	return 1.0 - clamp(exp(-density * dist), 0.0, 1.0);
}

float fogFactorExp2(const float dist, const float density ) {
	const float LOG2 = -1.442695;
	float d = density * dist;
	return 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
}
