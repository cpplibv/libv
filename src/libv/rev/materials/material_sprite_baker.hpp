// Project: libv.rev, File: src/libv/rev/materials/material_sprite_baker.hpp

#pragma once

// libv
#include <libv/glr/fwd.hpp>
// pro
#include <libv/rev/fwd.hpp>
#include <libv/rev/resource/material.hpp>
#include <libv/rev/materials/block_matrices.hpp>
#include <libv/rev/resource/texture.hpp>
#include <libv/rev/shader.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct UniformsSpriteBaker {
//	libv::glr::Uniform_vec4f base_color;

	template <typename Access> void access_uniforms(Access&) {
//		access(base_color, "base_color");
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

using ShaderSpriteBaker = libv::rev::Shader<UniformsSpriteBaker>;

// -------------------------------------------------------------------------------------------------

class MaterialSpriteBaker : public BaseMaterial {
public:
	ShaderSpriteBaker shader;

	Texture diffuse;
	//Texture normal;
	//Texture specular;

public:
	inline MaterialSpriteBaker(ShaderSpriteBaker shader, Texture diffuse) :
		shader(std::move(shader)),
		diffuse(std::move(diffuse)) {}

public:
	static Material create(const MaterialScanner& scan);

public:
	virtual void bind(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) override;
	// virtual bool utilizes(ModelVertexAttribute attribute) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
