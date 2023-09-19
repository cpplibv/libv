#version 450 core

#include <shader/lib/constants.glsl>
#define BUILTIN_BLOCK_IDENTIFIER_PASS pass
#include <shader/block300/pass300.glsl>
#include <shader/block300/light300.glsl>
#include <shader/re/lighting.glsl>

in vec4 fragmentColor;
in vec3 fragmentPositionW;
in vec2 fragmentUV;
in vec3 fragmentNormalUnW;
in vec3 fragmentTangentUnW;
in vec3 fragmentBitangentUnW;

layout(binding = TEXTURE_UNIT_DIFFUSE) uniform sampler2D texture0;
layout(binding = TEXTURE_UNIT_NORMAL) uniform sampler2D texture1;
layout(binding = TEXTURE_UNIT_RMAO) uniform sampler2D texture2;
layout(binding = 5) uniform samplerCube texture5;
layout(binding = TEXTURE_UNIT_SKY) uniform samplerCube textureSky;

out vec4 outputColor;

uniform vec4 uniform_color;
uniform float uniform_dynamic;
uniform int mode;

struct UniformStruct {
	vec4 color0;
	vec4 color1;
};
uniform UniformStruct uniform_struct;


bool section(vec3 normal, float i, float sectionCount) {
//	if (fragmentNormalW.x < -0.5 && fragmentNormalW.y < -0.5)
	float r = atan(normal.y, normal.x);
	r = mod(r + tau * 0.5, tau);
	float rstep = tau / sectionCount;
	float begin = i * rstep;
	float end = (i + 1.0) * rstep;
	return begin <= r && r < end;
}

void main() {
	vec3 fragmentNormalW = normalize(fragmentNormalUnW);
	vec3 fragmentTangentW = normalize(fragmentTangentUnW);
	vec3 fragmentBitangentW = normalize(fragmentBitangentUnW);

	vec4 surfaceColor = vec4(0, 0, 0, 1);
	surfaceColor.rgb = pow(abs(fragmentNormalW), vec3(3)) * 0.4 + 0.2;
	surfaceColor.a = 1;
	vec4 skySample = texture(textureSky, fragmentNormalW).rgba;
	surfaceColor.rgb = mix(surfaceColor.rgb, skySample.rgb, skySample.a);
	if (mode == 1)
		surfaceColor.rgba = texture(texture0, fragmentUV).rgba;
	if (mode == 2) {
		// Convert Non SRGB texture to 'SRGB'
		surfaceColor.rgba = texture(texture0, fragmentUV).rgba;
		surfaceColor.rgb = pow(surfaceColor.rgb, vec3(2.2));
	}
	outputColor.rgb = surfaceColor.rgb;
	outputColor.a = surfaceColor.a;

	//

	float debugSections = 32;
	if (section(fragmentNormalW, 1, debugSections))
		outputColor = uniform_color;
	else if (section(fragmentNormalW, 2, debugSections))
		outputColor.rg = vec2(fract(uniform_dynamic * 0.2));
	else if (section(fragmentNormalW, 3, debugSections)) {
		#ifdef TEST
		if (TEST == 0)
			outputColor.rgb = vec3(1, 0, 0);
		else
			outputColor.rgb = vec3(0, 1, 0);
		#else
			outputColor.rgb = vec3(0, 0, 1);
		#endif
	}
	else if (section(fragmentNormalW, 4, debugSections))
		outputColor.a = abs(fragmentNormalW.z);
	else {
		vec3 lightColor = vec3(0, 0, 0);

		vec3 V = normalize(pass.eyeW - fragmentPositionW);
		vec3 N = normalize(fragmentNormalW); // TODO P2: Find a better naming to indicate if fragment input has to be normalized

		for (int i = 0; i < numLightDirectionals; ++i) {
			LightDirectional300 light = lightDirectionals[i];
			vec3 L = -light.directionW;
			vec3 H = normalize(L + V);
			float NdotL = clamp(dot(N, L), 0.0, 1.0);
			float NdotH = clamp(dot(N, H), 0.0, 1.0);

			float attenuation = 1.0;

			lightColor += attenuation * light.radiance * NdotL; // Diffuse
			if (NdotL > 0)
				lightColor += attenuation * light.radiance * pow(NdotH, 32); // Specular
		}

		for (int i = 0; i < numLightPoints; ++i) {
			LightPoint300 light = lightPoints[i];
			vec3 L = normalize(light.positionW - fragmentPositionW);
			vec3 H = normalize(L + V);
			float NdotL = clamp(dot(N, L), 0.0, 1.0);
			float NdotH = clamp(dot(N, H), 0.0, 1.0);
			float dist = distance(fragmentPositionW, light.positionW);

			float attenuation = 1.0;
			attenuation *= lightDistanceAttenuation(light.attenuationFormula, dist, light.range, light.falloff);

			lightColor += attenuation * light.radiance * NdotL; // Diffuse
			if (NdotL > 0)
				lightColor += attenuation * light.radiance * pow(NdotH, 32); // Specular
		}

		for (int i = 0; i < numLightSpots; ++i) {
			LightSpot300 light = lightSpots[i];
			vec3 L = normalize(light.positionW - fragmentPositionW);
			vec3 H = normalize(L + V);
			float NdotL = clamp(dot(N, L), 0.0, 1.0);
			float NdotH = clamp(dot(N, H), 0.0, 1.0);
			float dist = distance(fragmentPositionW, light.positionW);

			float attenuation = 1.0;
			attenuation *= lightDistanceAttenuation(light.attenuationFormula, dist, light.range, light.falloff);
			attenuation *= lightSpotAttenuate(light.directionW, L, light.cosAngleInner, light.cosAngleOuter);

			lightColor += attenuation * light.radiance * NdotL; // Diffuse
			if (NdotL > 0)
				lightColor += attenuation * light.radiance * pow(NdotH, 32); // Specular
		}

		outputColor.rgb =
				0.1 * surfaceColor.rgb +
				lightColor * surfaceColor.rgb;
		outputColor.a = 1;
	}

	//	outputColor.rgb = surfaceColor.rgb;
	// TODO P4: uniform unique block testing
	// TODO P4: uniform shared block testing
	// TODO P4: uniform_struct testing

//	outputColor.rgb = fragmentTangentW;
//	outputColor.rgb = fragmentNormalW;
}
