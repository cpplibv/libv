#version 450 core

in vec3 fragmentDirectionW;

layout(binding = TEXTURE_UNIT_SKY) uniform samplerCube textureSky;
//layout(binding = TEXTURE_UNIT_SKY) uniform sampler2D textureSky;

out vec4 outputColor;

// -------------------------------------------------------------------------------------------------

void main() {
	outputColor = texture(textureSky, fragmentDirectionW).rgba;

	// Other features:
	// equirectangular beside cubemapXYZ
	//
	// Fog
	//
	// Horizon pushing along Z
	//	texture(textureSkybox, normalize(normalize(fragmentDirectionW) + vec3(0, 0, horizonOffset))).rgba
}
