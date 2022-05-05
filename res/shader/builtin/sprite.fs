#version 330 core

#include <block/matrices.glsl>
#include <builtin/sprite.glsl>
#include <builtin/sprite_definition.glsl>


in FragmentData fs_in;

out vec4 result;


uniform sampler2DArray textureColor;
uniform sampler2DArray textureNormal;

uniform vec4 fogColor = vec4(0.7, 0.8, 0.9, 1.0);


// -------------------------------------------------------------------------------------------------

void main() {

	SpriteDefinition def = spriteDefinitions[int(fs_in.type)];

	// --- Dither ---

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

	// --- Sample / Alpha test ---

	vec2 tile_size = vec2(1.0 / def.tile_num_x, 1.0 / def.tile_num_y);
	vec2 tile_origin = fs_in.tile_index * tile_size;
	vec2 uv = fs_in.uv;

	vec4 color = texture(textureColor, vec3(tile_origin + uv * tile_size, fs_in.type)).rgba;
//	vec3 normal = normalize(texture(textureNormal, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb * 2f - 1f);
	vec3 normal = texture(textureNormal, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb * 2f - 1f;

	if (color.a < 0.49)
		discard;

	color.a = 1;

	// --- Fake lighting ---

	vec3 N = normalize(normal); // Normal vector
	vec3 V = normalize(eye - fs_in.positionW); // View vector
	vec3 L = normalize(vec3(0.6, 0.8, 1)); // Light vector

	vec3 R = reflect(-L, N); // Reflection vector

	float strength_ambient = 0.4;
	float strength_diffuse = max(dot(N, L), 0.0);
	float strength_specular = pow(max(dot(V, R), 0.0), 8);

	float attenuation =
			strength_ambient +
			strength_diffuse +
			strength_specular * 0.4;

	color.rgb *= attenuation;

	// --- Fog ---

	color.rgb = mix(color.rgb, fogColor.rgb, fs_in.fogFactor);

	// --- Result / Debug

	result = color;

//	result.a = 0.5;
//	result.rgb = normal;
//	result.rgb = texture(textureColor, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb;
//	result.rgb = texture(textureNormal, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb;
//	result.rgb = texture(textureNormal, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb * 2 - 1;

//	result.rgb = vec3(fs_in.tile_index.x);

//	result.rgb = vec3(dither_offset);
//	result.rgb = vec3(fs_in.dither0);

//	result.rgb = vec3(dither_offset);
}
