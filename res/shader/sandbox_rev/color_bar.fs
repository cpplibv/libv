#version 330 core

#include <lib/color_spaces.glsl>
#include <lib/gamma.glsl>

in vec2 fragmentTexture0;

out vec4 result;

// -------------------------------------------------------------------------------------------------

void main() {
	//	result = vec4(1, fragmentTexture0, 1);

//		if (fragmentTexture0.y > 0.75)
//			result = vec4(vec3(fragmentTexture0.x), 1);
//		else if (fragmentTexture0.y > 0.50)
//			result = vec4(rgb_to_srgb(vec3(fragmentTexture0.x)), 1);
//		else if (fragmentTexture0.y > 0.25)
//			result = vec4(srgb_to_rgb(vec3(fragmentTexture0.x)), 1);
//		else
//			result = vec4(vec3(fragmentTexture0.x), 1);

	if (fragmentTexture0.y > 0.50)
		result = vec4(rgb_to_srgb(vec3(fragmentTexture0.x)), 1);
	else
		result = vec4(vec3(fragmentTexture0.x), 1);
}
