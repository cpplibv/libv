#version 330 core

#include <builtin/sprite.glsl>
#include <builtin/sprite_definition.glsl>


in FragmentData fs_in;

out vec4 result;

uniform sampler2DArray textureColor;
uniform sampler2DArray textureNormal;


// -------------------------------------------------------------------------------------------------

void main() {
//	if (color.a < 0.1)
//		discard;
//	else
////		color.a = 1.0;
//		color.a = (1 - (1 - color.a) * (1 - color.a));
//
////	result = color;
////	result = vec4(N, 1);
////	result = vec4(1 - vec3(gl_FragCoord.z * 1.5), 1);
//
////	resultColor = color;
////	resultNormal = N;
//
//	resultColor = color;
//	resultNormal = vec4(N * 0.5 + 0.5, 1);

//	result += texture(textureColor, vec2(0, 0)).rgba;
//	result += texture(textureNormal, vec2(0, 0)).rgba;

	// -------------------------------------------------------------------------------------------------

//	SpriteDefinition def = spriteDefinitions[0];
	SpriteDefinition def = spriteDefinitions[int(fs_in.type)];

//	vec2 tile_size = vec2(1.0 / 16.0);
//	vec2 tile_origin = fs_in.tile_index * tile_size;
//
//	vec4 color = texture(textureColor, tile_origin + fs_in.uv * tile_size).rgba;
//	vec3 normal = texture(textureNormal, tile_origin + fs_in.uv * tile_size).rgb;
////	vec4 color = textureLod(textureColor, fs_in.uv * (1/16.0), 1).rgba;
//
//	color.a = clamp(color.a * 32, 0, 1);
//
//	if (color.a < 0.1)
//		discard;
//
//	//	color.rgb = fs_in.eyeDir;
//	//	color.rgb = normal;
//	//	result = vec4(1, 0, 0, 1);
//	result = color;
//
//
//	//	float x = rand(fs_in.uv);
//	//	float x = rand(trunc(fs_in.uv * 64)); // !!! tile resolution? I think
//	//	if (x < 0.5)
//	//		result *= vec4(1, 1-x, 1-x, 1);
//	//		result *= vec4(1, 0, 0, 1);
//	//		result *= vec4(x, x, x, 1);
//
//	//	result.rgb = vec3(x);
//
//
//	float dither_offset = rand(trunc(fs_in.uv * 64)); // !!! tile resolution? I think
//	dither_offset *= 0.5;
////	dither_offset += (1 - length(fs_in.uv * 2.0 - 1.0) / sqrt(2.0)) * 0.5;
//	dither_offset += (1 - length(fs_in.uv * 2.0 - 1.0)) * 0.5;
//
//
//	result.rgb = vec3(dither_offset);
//	result.rgb = vec3(fs_in.dither0);

	// -------------------------------------------------------------------------------------------------

//	vec2 tile_size = vec2(1.0 / 8, 1.0 / 8);
//	vec2 tile_size = vec2(1.0 / def.tile_num_x, 1.0 / def.tile_num_y);
//	vec2 tile_origin = fs_in.tile_index * tile_size;

//	vec2 dither_seed = gl_FragCoord.xy;
//	vec2 dither_seed = trunc(fs_in.uv * 64);
//	sprite random + sprite pos based viewport position, so, combine this two

//	float dither_offset = rand(dither_seed); // !!! tile resolution? I think
//	dither_offset *= 0.5;
//	dither_offset += (1 - length(fs_in.uv * 2.0 - 1.0) / sqrt(2.0)) * 0.5;
////	dither_offset += (1 - length(fs_in.uv * 2.0 - 1.0)) * 0.5;
//
//	float dither_offset2 = rand(dither_seed + 100); // !!! tile resolution? I think
//	dither_offset2 *= 0.5;
//	dither_offset2 += (1 - length(fs_in.uv * 2.0 - 1.0) / sqrt(2.0)) * 0.5;
//
//	if (fs_in.dither0 * 1.5 + dither_offset * 0.5 < 1.0) {
//		tile_origin.y -= tile_size.y;
////		result = vec4(1, 1, 1, 1);
////		return;
//	} else {
////		result = vec4(0, 0, 0, 1);
////		return;
//	}
//
//	if (fs_in.dither1 * 1.5 + dither_offset2 * 0.5 < 1.0) {
//		tile_origin.x += tile_size.x;
////		result = vec4(1, 1, 1, 1);
////		return;
//	} else {
////		result = vec4(0, 0, 0, 1);
////		return;
//	}

	vec2 tile_size = vec2(1.0 / def.tile_num_x, 1.0 / def.tile_num_y);
	vec2 tile_origin = fs_in.tile_index * tile_size;
	vec2 uv = fs_in.uv;



	vec4 color = texture(textureColor, vec3(tile_origin + uv * tile_size, fs_in.type)).rgba;
	vec3 normal = texture(textureNormal, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb;

//	if (color.a < 0.49)
//		color.rgb = vec3(0, 0, 0);
	if (color.a < 0.49)
		discard;

	color.a = 1;
	result = color;

//	result.a = 0.5;
//	result.rgb = normal;

//	result.rgb = vec3(fs_in.tile_index.x);

//	result.rgb = vec3(dither_offset);
//	result.rgb = vec3(fs_in.dither0);

//	result.rgb = vec3(dither_offset);
}
