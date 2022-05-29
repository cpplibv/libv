#version 330 core

#include <block/matrices.glsl>
#include <builtin/sprite.glsl>
#include <builtin/sprite_definition.glsl>
#include <lib/color.glsl>
#include <lib/fresnel.glsl>
#include <lib/quat.glsl>


in FragmentData fs_in;

out vec4 result;


uniform sampler2DArray textureColor;
uniform sampler2DArray textureNormal;

uniform vec4 fogColor = vec4(0.7, 0.8, 0.9, 1.0);
uniform vec3 sunColor = vec3(1.0, 0.9, 0.8);
uniform vec3 sunDirection = vec3(0.784464, 0.196116, 0.588348);


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

	//	float dither_offset = rand(dither_seed); // !! tile resolution? I think
	//	dither_offset *= 0.5;
	//	dither_offset += (1 - length(fs_in.uv * 2.0 - 1.0) / sqrt(2.0)) * 0.5;
	////	dither_offset += (1 - length(fs_in.uv * 2.0 - 1.0)) * 0.5;
	//
	//	float dither_offset2 = rand(dither_seed + 100); // !! tile resolution? I think
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
	vec3 normal = texture(textureNormal, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb * 2.0 - 1.0;
	normal = qrotate(fs_in.rotationQuat, normalize(normal));

	if (color.a < 0.49)
		discard;

	color.a = 1;

	// --- HSV Color Shift ---

	color.rgb = hsv_to_rgb(normalize_hsv(rgb_to_hsv(color.rgb) + fs_in.hsvColorShift.rgb));

	// --- Lighting ---

	vec3 N = normalize(normal); // Normal vector

//	N = normalize(mix(N, fs_in.surfaceNormal, 0.5)); // Blend surface normals

	vec3 V = normalize(eye - fs_in.positionW); // View vector
	vec3 L = sunDirection; // Light vector

//	float surfaceShadeDiffuse = max(dot(fs_in.surfaceNormal, L), 0.0);
//	float surfaceShadeSpecular = pow(max(dot(V, reflect(-L, fs_in.surfaceNormal)), 0.0), 4);

	float surfaceShade = min(1.0,
			max(dot(fs_in.surfaceNormal, L), 0.0) + // From diffuse
			pow(max(dot(V, reflect(-L, fs_in.surfaceNormal)), 0.0), 16) * 0.2 // From specular
	);

	vec3 R = reflect(-L, N); // Reflection vector

	float strength_ambient = 0.2;
	float strength_diffuse = max(dot(N, L), 0.0);
	float strength_specular = pow(max(dot(V, R), 0.0), 16);

	float attenuation =
			fresnel(1, N, V) * mix(0.1, 0.3, surfaceShade) +
			strength_ambient * 0.4 +
			strength_diffuse * surfaceShade * 0.6 +
			strength_specular * surfaceShade * 0.1;

	color.rgb *= attenuation * sunColor;

	// --- Fog ---

	color.rgb = mix(color.rgb, fogColor.rgb, fs_in.fogFactor);

	// --- Result / Debug

	result = color;

//	result = vec4(1, 0, 0, 1);
//	result.rgb = texture(textureColor, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb;
//	result.a = 1;

//	result.rgb = step(0.0, normal);
//	if (fs_in.dither.y > 0.45)
//		result.a = 0.5;

//	result.rgb = vec3(fs_in.dither.y / 3.14 * 2);

//	result.rgb = vec3(dot(normal, L));
//	result.rgb = normalize(normal);
//	result.rgb = normal;
//	result.rgb = fs_in.surfaceNormal * 0.5 + 0.5;

//	result.rgb = texture(textureNormal, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb * 2f - 1f;
//	result.rgb = texture(textureNormal, vec3(uv, fs_in.type)).rgb * 2f - 1f;
//	result.rgb = normal * 0.5 + 0.5;
//	result.rgb = step(0.75, normal);

//	result.rgb = vec3(fs_in.tile_index.y / 2.0);
//	if (fs_in.tile_index.y < 0)
//		result.rgb = vec3(1, 0, 0);
//	if (fs_in.tile_index.y > 2)
//		result.rgb = vec3(0, 1, 0);

//	result.rgb = vec3(fs_in.tile_index.x / 2.0);
//	if (fs_in.tile_index.x < 0)
//		result.rgb = vec3(1, 0, 0);
//	if (fs_in.tile_index.x > 2)
//		result.rgb = vec3(0, 1, 0);

//			if (abs(fs_in.tile_index.y - 0) < 0.1)
//				result.rgb = vec3(1, 0, 0);
//			if (abs(fs_in.tile_index.y - 1) < 0.1)
//				result.rgb = vec3(0, 1, 0);
//			if (abs(fs_in.tile_index.y - 2) < 0.1)
//				result.rgb = vec3(0, 0, 1);

//			if (abs(fs_in.tile_index.x - 0) < 0.1)
//				result.rgb = vec3(1, 0, 0);
//			if (abs(fs_in.tile_index.x - 1) < 0.1)
//				result.rgb = vec3(0, 1, 0);
//			if (abs(fs_in.tile_index.x - 2) < 0.1)
//				result.rgb = vec3(0, 0, 1);
//	result.rgb = vec3(fs_in.dither.x + 0.5);
//	result.rg = abs(fs_in.dither);
//	result.ba = vec2(0, 1);
//	result.rgb = texture(textureNormal, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb * 2f - 1f;

//	result.a = 0.5;
//	result.rgb = normal;
//	result.rgb = texture(textureColor, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb;
//	result.rgb = texture(textureNormal, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb;
//	result.rgb = texture(textureNormal, vec3(clamp(uv * 1.5 - 0.25, 0, 1), fs_in.type)).rgb;
//	result.rgb = texture(textureNormal, vec3(tile_origin + uv * tile_size, fs_in.type)).rgb * 2 - 1;

//	result.rgb = vec3(fs_in.tile_index.x);

//	result.rgb = vec3(dither_offset);
//	result.rgb = vec3(fs_in.dither0);

//	result.rgb = vec3(dither_offset);
}
