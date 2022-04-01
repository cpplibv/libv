// Project: libv.rev, File: src/libv/rev/materials/material_sprite_baker.cpp

// hpp
#include <libv/rev/materials/material_sprite_baker.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/vm4/texture_type.hpp>
// pro
#include <libv/rev/resource/attribute.hpp>
#include <libv/rev/resource/material_scanner.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

Material MaterialSpriteBaker::create(const MaterialScanner& scan) {
	return Material::create<MaterialSpriteBaker>(
		scan.shader<UniformsSpriteBaker>("builtin/sprite_baker.vs", "builtin/sprite_baker.fs"),
		scan.texture_or(libv::vm4::TextureType::diffuse, "builtin:white")
//		scan.texture_or(libv::vm4::TextureType::specular, "builtin:white"),
//		scan.texture_or(libv::vm4::TextureType::normals, "builtin:up")
	);
}

void MaterialSpriteBaker::bind(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	glr.program(shader.program());
	glr.texture(diffuse.texture(), textureChannel_diffuse);

	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = glr.mvp();
	uniforms[layout_matrices.matM] = glr.model;
	uniforms[layout_matrices.matP] = glr.projection;
	uniforms[layout_matrices.eye] = glr.eye();

	glr.uniform(std::move(uniforms));
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
