#version 450 core

#include <shader/lib/equirectangular.glsl>


in vec3 fragmentDirectionUnW;

layout(binding = TEXTURE_UNIT_SKY) uniform samplerCube textureSky;
layout(binding = TEXTURE_UNIT_DIFFUSE) uniform sampler2D textureDiffuse;

out vec4 outputColor;

const uint typeCubemapXYZ = 0;
const uint typeCubemapZXY = 1;
const uint typeEquirectangular = 2;
uniform uint skyboxType = typeCubemapXYZ;
uniform float intensity = 1.0;
uniform float horizonOffset = 0.0;
uniform mat2 matRotateZ = mat2(1);


// -------------------------------------------------------------------------------------------------

void main() {
	vec3 direction = normalize(fragmentDirectionUnW);

	// Horizon offset
	direction = normalize(direction + vec3(0, 0, horizonOffset));

	// Rotate Z
	direction.xy = matRotateZ * direction.xy;

	// Sample
	vec4 sampleSky;
	switch (skyboxType) {
	case typeCubemapXYZ:
		sampleSky = texture(textureSky, direction).rgba;
		break;
	case typeCubemapZXY:
		sampleSky = texture(textureSky, direction.xzy).rgba;
		break;
	case typeEquirectangular:
//		sampleSky = texture(textureDiffuse, dirToEquirectangularUV(normalize(fragmentDirectionUnW))).rgba;
		sampleSky = textureLod(textureDiffuse, dirToEquirectangularUV(direction), 0).rgba;
		break;
	}

	// Alpha settings: source, one, zero
	// outputColor = sampleSky;
	// outputColor = vec4(sampleSky.rgb, 1);
	outputColor = vec4(sampleSky.rgb, 0);

	// Intensity
	outputColor.rgb *= intensity;

	// TODO P5: Fog
}
