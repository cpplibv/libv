#version 330 core

in vec2 fragmentTexture0;

out vec4 result;

uniform float progress = 1.0;

// -------------------------------------------------------------------------------------------------

void main() {
//	result = vec4(1, fragmentTexture0, 1);

//	result = vec4(1, pow(fragmentTexture0, vec2(2.0)) * 2, 1);


//	vec2 cross_mask = vec2(1, 1);
	vec2 cross_mask = step(0.45, fragmentTexture0) * step(0.45, 1.0 - fragmentTexture0);

//	result = vec4(0, 4.0 * cross_mask, 1);
//	result = vec4(vec3(191./255, 138./255, 3./255) * 6, 1);
//	result = vec4(vec3(1, 1, 0.7) + vec3(191./255, 138./255, 3./255) * 4, 1);

//	result = vec4(1, 2.0 * cross_mask, 1);
//	result = vec4(fragmentTexture0.x, 2.0 * cross_mask, 1);

	result = vec4(1, fragmentTexture0 + 4.0 * cross_mask, 1);
//	result = vec4(1, fragmentTexture0 + 2.0 * cross_mask, 1);
//	result = vec4(1, fragmentTexture0 + 1.0 * cross_mask, 1);
//	result = vec4(1, fragmentTexture0 + 0.1 * cross_mask, 1);
//	result = vec4(cross_mask.x * vec3(2), 1);

//	result = vec4(1, fragmentTexture0.x + 2.0 * cross_mask.x, 1, 1);
//	result = vec4(1, 1, fragmentTexture0.x + 2.0 * cross_mask.x, 1);

//	result = vec4(0, 0, fragmentTexture0.x + 2.0 * cross_mask.x, 1);
//	result = vec4(0, fragmentTexture0.x + 2.0 * cross_mask.x, 0, 1);
//	result = vec4(fragmentTexture0.x + 2.0 * cross_mask.x, 0, 0, 1);

//	result = vec4(0, 0, 2, 1);
//	result = vec4(0, 0, 4, 1);

//	result = vec4(5, 4, 1, 1);

//	result = vec4(5, 1, 1, 1);
//	result = vec4(1, 5, 1, 1);
//	result = vec4(1, 1, 5, 1);

//	result = vec4(3, 1, 1, 1);
}
