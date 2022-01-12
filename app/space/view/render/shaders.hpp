// Project: libv, File: app/space/view/render/shaders.hpp

#pragma once

// libv
#include <libv/glr/attribute.hpp>
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/rev/shader.hpp>
//#include <libv/glr/layout_to_string.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

constexpr auto attribute_position   = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal     = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0     = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_bondID     = libv::glr::Attribute<4, libv::vec4i>{};
constexpr auto attribute_bondWeight = libv::glr::Attribute<5, libv::vec4f>{};
constexpr auto attribute_tangent    = libv::glr::Attribute<6, libv::vec3f>{};
constexpr auto attribute_bitangent  = libv::glr::Attribute<7, libv::vec3f>{};
constexpr auto attribute_texture0   = libv::glr::Attribute<8, libv::vec2f>{};
constexpr auto attribute_custom1    = libv::glr::Attribute<14, libv::vec4f>{};
constexpr auto attribute_custom0    = libv::glr::Attribute<15, libv::vec4f>{};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};
constexpr auto textureChannel_normal = libv::gl::TextureChannel{1};
constexpr auto textureChannel_pattern = libv::gl::TextureChannel{7};

struct UniformLayoutMatrices {
	// Could be split into two: Camera and Model
	// 			Camera: matV, matP, near, far, ...
	// 			Model: matM, matMVP

	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matP;
	libv::glr::Uniform_mat4f matM;
	libv::glr::Uniform_vec3f eye;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matP);
	LIBV_REFLECTION_ACCESS(matM);
	LIBV_REFLECTION_ACCESS(eye);
};

//struct UniformLayoutPass {
//	libv::glr::Uniform_float time_sim;
//	libv::glr::Uniform_float time_real;
//	libv::glr::Uniform_float time_real_hcap;
//	libv::glr::Uniform_vec2f render_resolution;
//	libv::glr::Uniform_vec2f pixel_size; // 1.0 / render_resolution
//
//	libv::glr::Uniform_int32 test_mode0;
//
//    libv::glr::Uniform_vec4f frame_random_vec4; // [0..1] random numbers changing each frame
//    libv::glr::Uniform_vec4i frame_random_ivec4; // [int_min..int_max] random numbers changing each frame
//
//	LIBV_REFLECTION_ACCESS(time_sim);
//	LIBV_REFLECTION_ACCESS(time_real);
//	LIBV_REFLECTION_ACCESS(time_real_hcap);
//	LIBV_REFLECTION_ACCESS(render_resolution);
//	LIBV_REFLECTION_ACCESS(pixel_size);
//
//	LIBV_REFLECTION_ACCESS(test_mode0);
//
//	LIBV_REFLECTION_ACCESS(frame_random_vec4);
//	LIBV_REFLECTION_ACCESS(frame_random_ivec4);
//};

const auto uniformBlock_matrices = libv::glr::UniformBlockBinding{0, "Matrices"};
//const auto uniformBlock_model = libv::glr::UniformBlockBinding{1, "Model"};
//const auto uniformBlock_pass = libv::glr::UniformBlockBinding{?, "Pass"};
const auto layout_matrices = libv::glr::layout_std140<UniformLayoutMatrices>(uniformBlock_matrices);
//const auto layout_pass = libv::glr::layout_std140<UniformLayoutPass>(uniformBlock_pass);

// -------------------------------------------------------------------------------------------------

// TODO P2: libv.glr: Shader automated block binding by watching the includes
//          So this should work:
//          shader_loader.register_block<UniformLayoutMatrices>("block/sphere.glsl");
//          Note that this also could generate the file block/sphere.glsl (OR just be an in memory resource)
//          And this would be called on any program that includes block/sphere.glsl
//          program.block_binding(uniformBlock_matrices) and access_blocks(Access&) will be replaced by this system
//          | issue: some struct might have already been defined, so block to string might have to skip them
//                  This mean tracking of structs OR pushing the problem back to the include system pragma once solution
//                  With additional mapping and includes to struct/my_struct_that_is_in_a_block.glsl
//			std::cout << libv::glr::layout_to_string<UniformLayoutMatrices>("Sphere") << std::endl;

struct UniformsColor {
	libv::glr::Uniform_vec4f base_color;

	template <typename Access> void access_uniforms(Access& access) {
		access(base_color, "base_color");
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

struct UniformsCommandArrow {
	libv::glr::Uniform_int32 test_mode;
	libv::glr::Uniform_float time;
	libv::glr::Uniform_vec2f render_resolution;
	libv::glr::Uniform_vec4f color;

	template <typename Access> void access_uniforms(Access& access) {
		access(color, "color");
		access(render_resolution, "render_resolution");
		access(test_mode, "test_mode", 0);
		access(time, "time", 0.f);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

struct UniformsEditorBackground {
	libv::glr::Uniform_texture texture_noise;
	libv::glr::Uniform_vec2f render_resolution;
	libv::glr::Uniform_vec4f noise_scale;
	libv::glr::Uniform_vec4f base_color;

	template <typename Access> void access_uniforms(Access& access) {
		access(texture_noise, "texture_noise", textureChannel_pattern);
		access(render_resolution, "render_resolution");
		access(base_color, "base_color");
		access(noise_scale, "noise_scale");
	}

	template <typename Access> void access_blocks(Access&) {
	}
};

struct UniformsFleet {
	libv::glr::Uniform_vec4f base_color;
	libv::glr::Uniform_int32 selected;

	template <typename Access> void access_uniforms(Access& access) {
		access(base_color, "base_color");
		access(selected, "selected");
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

struct UniformsPlanet {
	libv::glr::Uniform_vec4f base_color0;
	libv::glr::Uniform_vec4f base_color1;
//	libv::glr::Uniform_int32 selected;

	template <typename Access> void access_uniforms(Access& access) {
		access(base_color0, "base_color0");
		access(base_color1, "base_color1");
//		access(selected, "selected");
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

struct UniformsTestMode {
	libv::glr::Uniform_int32 test_mode;

	template <typename Access> void access_uniforms(Access& access) {
		access(test_mode, "test_mode", 0);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

struct UniformsText {
	libv::glr::Uniform_texture font_texture;

	template <typename Access> void access_uniforms(Access& access) {
		access(font_texture, "texture_font", textureChannel_diffuse);
	}

//	template <typename Access> void access_blocks(Access&) {
	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

//struct UniformLight {
//	libv::glr::Uniform_int32 type;
//
//	libv::glr::Uniform_vec3f position;
//	libv::glr::Uniform_vec3f direction;
//
//	libv::glr::Uniform_vec3f ambient;
//	libv::glr::Uniform_vec3f diffuse;
//	libv::glr::Uniform_vec3f specular;
//
////	libv::glr::Uniform_float range;
//	libv::glr::Uniform_float innerCosAngle;
//	libv::glr::Uniform_float outerCosAngle;
//
////	libv::glr::Uniform_bool shadowCast;
////	libv::glr::Uniform_int32 shadowMapSampler;
////	libv::glr::Uniform_mat4f shadowMVPTmat;
//
////	UniformLight(const std::string& name);
//
//	template <typename Access>
//	void update_uniforms(Access& access) {
//		access(type, "sun.type", libv::to_value(LightType::point));
//
//		access(position, "sun.position", libv::vec3f(0, 0, 0));
//		access(direction, "sun.direction", libv::vec3f(0, 0, -1));
//
//		access(ambient, "sun.ambient", libv::vec3f(0.1f, 0.1f, 0.1f));
//		access(diffuse, "sun.diffuse", libv::vec3f(1, 1, 1));
//		access(specular, "sun.specular", libv::vec3f(1, 1, 1));
//
////		access(range, "sun.range", 100);
//		access(innerCosAngle, "sun.innerCosAngle", 0.8f);
//		access(outerCosAngle, "sun.outerCosAngle", 0.6f);
//
////		access(shadowCast, "sun.shadowCast", false);
////		access(shadowMapSampler, "sun.shadowMapSampler");
////		access(shadowMVPTmat, "sun.shadowMVPTmat");
//	}
//
//	void set(libv::glr::Queue& gl, const Light& v) {
//		// TODO P3: gl::uniform and glr::uniform template overloads to accept any enum type and call to_value by itself
//		gl.uniform(type, libv::to_value(v.type));
//
//		gl.uniform(position, v.position);
//		gl.uniform(direction, v.direction);
//
//		gl.uniform(ambient, v.ambient);
//		gl.uniform(diffuse, v.diffuse);
//		gl.uniform(specular, v.specular);
//
//		gl.uniform(innerCosAngle, v.innerCosAngle);
//		gl.uniform(outerCosAngle, v.outerCosAngle);
//	}
//};
//
//struct UniformsModel {
//	libv::glr::Uniform_vec4f color;
////	libv::glr::Uniform_mat4f matM;
////	libv::glr::Uniform_mat4f matMVP;
//	libv::glr::Uniform_texture texture;
//	libv::glr::Uniform_int32 mode;
//	libv::glr::Uniform_float near;
//	libv::glr::Uniform_float far;
////	libv::glr::Uniform_vec3f eyeW;
//
////	UniformLight sun;
//
//	template <typename Access> void access_uniforms(Access& access) {
//		access(color, "color", libv::vec4f(1, 1, 1, 1));
////		access(matM, "matM", libv::mat4f::identity());
////		access(matMVP, "matMVP", libv::mat4f::identity());
//		access(texture, "textureSampler", textureChannel_diffuse);
//		access(mode, "mode");
////		access(mode, "mode", 2);
////		access(mode, "mode", RenderMode::normalW);
//		access(near, "near", 0.1f);
//		access(far, "far", 1000.0f);
////		access(eyeW, "eyeW");
//
//		// TODO P2: sun auto iteration and name auto appending chaining
////		sun.update_uniforms(access);
////		access(sun, "sun");
//	}
//
//	template <typename Access> void access_blocks(Access& access) {
//		access(uniformBlock_matrices);
////		access(uniformBlock_model);
//	}
//};

using ShaderColor = libv::rev::Shader<UniformsColor>;
using ShaderCommandArrow = libv::rev::Shader<UniformsCommandArrow>;
using ShaderEditorBackground = libv::rev::Shader<UniformsEditorBackground>;
using ShaderFleet = libv::rev::Shader<UniformsFleet>;
using ShaderPlanet = libv::rev::Shader<UniformsPlanet>;
using ShaderTestMode = libv::rev::Shader<UniformsTestMode>;
using ShaderText = libv::rev::Shader<UniformsText>;
//using ShaderModel = libv::rev::Shader<UniformsModel>;
//using Shader = libv::rev::Shader<>;

// -------------------------------------------------------------------------------------------------

} // namespace space
