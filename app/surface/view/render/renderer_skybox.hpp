// Created by dbobula on 4/23/2022.

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/rev/texture.hpp>
//#include <libv/math/vec.hpp>
//#include <array>
//#include <vector>
// pro
//#include <surface/view/render/renderer.hpp>
#include <surface/view/render/shaders.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

class RendererResourceContext;

// -------------------------------------------------------------------------------------------------

struct UniformsSkybox {
	libv::glr::Uniform_bool fogEnabled;
//	libv::glr::Uniform_float fogIntensity;
	libv::glr::Uniform_vec4f fogColor;
	libv::glr::Uniform_texture textureSkybox;

	template <typename Access> void access_uniforms(Access& access) {
		access(textureSkybox, "textureSkybox", textureChannel_diffuse);
		access(fogEnabled, "fogEnabled", true);
//		access(fogIntensity, "fogIntensity", 0.05f);
		access(fogColor, "fogColor", libv::vec4f{0.7f, 0.8f, 0.9f, 1.0f});
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

using ShaderSkybox = libv::rev::Shader<UniformsSkybox>;

// -------------------------------------------------------------------------------------------------

struct RendererSkybox {
private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderSkybox shader;
	libv::rev::Texture texture;

public:
	bool fogEnabled = true;
	float fogIntensity = 0.05f;
	libv::vec4f fogColor = libv::vec4f{0.7f, 0.8f, 0.9f, 1.0f};

public:
	explicit RendererSkybox(RendererResourceContext& rctx);

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
