// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// pro
#include <vm4_viewer/scene/shader.hpp>
#include <vm4_viewer/scene/light.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

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

struct UniformsShaderModel {
	libv::glr::Uniform_vec4f uniform_color;
	libv::glr::Uniform_mat4f uniform_Mmat;
	libv::glr::Uniform_mat4f uniform_MVPmat;
	libv::glr::Uniform_texture uniform_texture;
	libv::glr::Uniform_int32 uniform_mode;
	libv::glr::Uniform_float uniform_near;
	libv::glr::Uniform_float uniform_far;
	libv::glr::Uniform_vec3f uniform_eyeW;

	UniformLight uniform_sun;

	constexpr static libv::gl::TextureChannel textureChannel{0};

	template <typename Access>
	void update_uniforms(Access& access) {
		access(uniform_color, "color", libv::vec4f(1, 1, 1, 1));
		access(uniform_Mmat, "Mmat", libv::mat4f::identity());
		access(uniform_MVPmat, "MVPmat", libv::mat4f::identity());
		access(uniform_texture, "textureSampler", textureChannel);
		access(uniform_mode, "mode");
//		access(uniform_mode, "mode", 2);
//		access(uniform_mode, "mode", RenderMode::normalW);
		access(uniform_near, "near", 0.1f);
		access(uniform_far, "far", 1000.0f);
		access(uniform_eyeW, "eyeW");

		uniform_sun.update_uniforms(access);
//		access(sun, "sun");
	}

//	void update(libv::span<const Light> lights);
//	void update(const Material& material);
//	void update(const Camera& camera);
};

using ShaderModel = app::Shader<UniformsShaderModel>;

// -------------------------------------------------------------------------------------------------

} // namespace app
