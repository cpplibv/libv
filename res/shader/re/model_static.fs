#version 450 core

//#include <lib_vm4/depth.glsl>
//#include <lib_vm4/gamma.glsl>
//#include <lib_vm4/light.glsl>
//#include <lib_vm4/material.glsl>
//#include <lib_vm4/phong.glsl>
//
//
//#define MODE_RED                     0
//#define MODE_COLOR                   1
//
//#define MODE_FRAGMENT_POSITION      10
//#define MODE_FRAGMENT_POSITION_MOD  11
//#define MODE_FRAGMENT_POSITION_MOD2 12
//
//#define MODE_FRAGMENT_NORMAL        20
//#define MODE_FRAGMENT_NORMAL_ABS    21
//#define MODE_FRAGMENT_TANGENT       22
//#define MODE_FRAGMENT_TANGENT_ABS   23
//#define MODE_FRAGMENT_BITANGENT     24
//#define MODE_FRAGMENT_BITANGENT_ABS 25
//
//#define MODE_TEXTURE0               30
//
//#define MODE_TEXTURE0_SAMPLE0       40
//#define MODE_TEXTURE0_SAMPLE1       41
//#define MODE_TEXTURE0_SAMPLE2       42
//#define MODE_TEXTURE0_SAMPLE3       43
//
//#define MODE_DEPTH                  50
//#define MODE_DEPTH_LINEARIZE        51
//#define MODE_DEPTH_LINEARIZE_MOD    52
//
//#define MODE_PHONG_GREY             70
//#define MODE_PHONG_GREY_GAMMA       71

in vec3 fragmentPositionW;
in vec2 fragmentUV;
in vec3 fragmentNormalUnW;
in vec3 fragmentTangentUnW;
in vec3 fragmentBitangentUnW;

out vec4 result;

//uniform int mode = 71;
//
//uniform sampler2D textureSampler0;
//uniform sampler2D textureSampler1;
//uniform sampler2D textureSampler2;
//uniform sampler2D textureSampler3;

void main() {
	vec3 fragmentNormalW = normalize(fragmentNormalUnW);
	vec3 fragmentTangentW = normalize(fragmentTangentUnW);
	vec3 fragmentBitangentW = normalize(fragmentBitangentUnW);

//	Material material_gray;
//
//	material_gray.ambient = vec3(0.8, 0.8, 0.8);
//	material_gray.diffuse = vec3(0.8, 0.8, 0.8);
//	material_gray.specular = vec3(1.0, 1.0, 1.0);
//	material_gray.emission = vec3(0.0, 0.0, 0.0);
//
//	material_gray.shininess = 16.0;
//
//	if (mode == MODE_RED)
//		result = vec4(1, 0, 0, 1);
//	else if (mode == MODE_COLOR)
//		result = color;
//
//	else if (mode == MODE_FRAGMENT_POSITION)
//		result = vec4(fragmentPositionW, 1);
//	else if (mode == MODE_FRAGMENT_POSITION_MOD)
//		result = vec4(abs(mod(fragmentPositionW, 50)) / 50, 1);
//	else if (mode == MODE_FRAGMENT_POSITION_MOD2)
//		result = vec4(abs(mod(fragmentPositionW, 5)) / 5, 1);
//
//	else if (mode == MODE_FRAGMENT_NORMAL)
//		result = vec4(normalize(fragmentNormalW), 1);
//	else if (mode == MODE_FRAGMENT_NORMAL_ABS)
//		result = vec4(normalize(abs(fragmentNormalW)), 1);
//	else if (mode == MODE_FRAGMENT_TANGENT)
//		result = vec4(normalize(fragmentTangentW), 1);
//	else if (mode == MODE_FRAGMENT_TANGENT_ABS)
//		result = vec4(normalize(abs(fragmentTangentW)), 1);
//	else if (mode == MODE_FRAGMENT_BITANGENT)
//		result = vec4(normalize(fragmentBitangentW), 1);
//	else if (mode == MODE_FRAGMENT_BITANGENT_ABS)
//		result = vec4(normalize(abs(fragmentBitangentW)), 1);
//
//	else if (mode == MODE_TEXTURE0)
//		result = vec4(fragmentTexture0, 0, 1);
//
//	else if (mode == MODE_TEXTURE0_SAMPLE0)
//		result = texture(textureSampler0, fragmentTexture0);
//	else if (mode == MODE_TEXTURE0_SAMPLE1)
//		result = texture(textureSampler1, fragmentTexture0);
//	else if (mode == MODE_TEXTURE0_SAMPLE2)
//		result = texture(textureSampler2, fragmentTexture0);
//	else if (mode == MODE_TEXTURE0_SAMPLE3)
//		result = texture(textureSampler3, fragmentTexture0);
//
//	else if (mode == MODE_DEPTH)
//		result = vec4(vec3(1, 1, 1) * gl_FragCoord.z, 1);
//	else if (mode == MODE_DEPTH_LINEARIZE)
//		result = vec4(vec3(1, 1, 1) * linearize_depth(gl_FragCoord.z, near, far), 1);
//	else if (mode == MODE_DEPTH_LINEARIZE_MOD)
//		result = vec4(vec3(1, 1, 1) * mod(linearize_depth(gl_FragCoord.z, near, far), 25) / 25, 1);
//
//	else if (mode == MODE_PHONG_GREY)
//		result = phong(eyeW, fragmentPositionW, normalize(fragmentNormalW), material_gray, sun);
//
//	else if (mode == MODE_PHONG_GREY_GAMMA) {
//		result = phong(eyeW, fragmentPositionW, normalize(fragmentNormalW), material_gray, sun);
//		result.rgb = gamma_correction(result.rgb, gamma);
//	}
//
//	else
		result = vec4(1, 0, 1, 1);



//		result = vec4(fragmentPositionW, 1);
//		result = vec4(fragmentUV, 0, 1);
		result = vec4(normalize(fragmentNormalW), 1);
//		result = vec4(normalize(fragmentTangentW), 1);
//		result = vec4(normalize(fragmentBitangentW), 1);
}



//enum class RenderMode {
//	color = 0,
//
//	positionW = 1,
//	normalW = 2,
//	tangentW = 3,
//	bitangentW = 4,
//
//	texture0 = 5,
//	depth = 6,
//
//	sample0 = 10,
//	sample1 = 11,
//	sample2 = 12,
//	sample3 = 13,
//	sample4 = 14,
//	sample5 = 15,
//	sample6 = 16,
//	sample7 = 17,
//
//	lambert = 20,
//};
//
//ShaderModel()
//	define("MODE_COLOR", RenderMode::color);
//
//	define("MODE_POSITION_W", RenderMode::positionW);
//	define("MODE_NORMAL_W", RenderMode::normalW);
//	define("MODE_TANGENT_W", RenderMode::tangentW);
//	define("MODE_BITANGENT_W", RenderMode::bitangentW);
//
//	define("MODE_TEXTURE0", RenderMode::texture0);
//	define("MODE_DEPTH", RenderMode::depth);
//
//	define("MODE_SAMPLE0", RenderMode::sample0);
//	define("MODE_SAMPLE1", RenderMode::sample1);
//	define("MODE_SAMPLE2", RenderMode::sample2);
//	define("MODE_SAMPLE3", RenderMode::sample3);
//	define("MODE_SAMPLE4", RenderMode::sample4);
//	define("MODE_SAMPLE5", RenderMode::sample5);
//	define("MODE_SAMPLE6", RenderMode::sample6);
//	define("MODE_SAMPLE7", RenderMode::sample7);
//
//	define("MODE_LAMBERT", RenderMode::lambert);
