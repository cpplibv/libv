#version 450 core

#include <shader/re/lens_flare.glsl>

flat in vec4 fragmentColor;
flat in float fragmentColorLightAttenuation;
in vec2 fragmentUV;

layout(binding = TEXTURE_UNIT_DIFFUSE) uniform sampler2D texture0;

out vec4 outputColor;

uniform float visibility;

void main() {
	vec4 sample0 = texture(texture0, fragmentUV).rgba;

	// Additive blending, premultiply alpha:
	outputColor.rgb =
			sample0.rgb * sample0.a *
			fragmentColor.rgb * fragmentColor.a *
			mix(vec3(1), lightColor.rgb, fragmentColorLightAttenuation) *
			visibility;
	outputColor.a = 1;
}
