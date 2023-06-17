// Project: libv.rev, File: src/libv/rev/materials/material_sprite_baker.cpp

// hpp
#include <libv/rev/materials/material_editor_grid.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/vm4/texture_type.hpp>
// pro
#include <libv/rev/shader/attribute.hpp>
#include <libv/rev/resource/material_scanner.hpp>
#include <libv/rev/resource/resource_manager.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

Material MaterialEditorGrid::create(const MaterialScanner& scan) {
	return Material::create<MaterialEditorGrid>(
		scan.shader<UniformsEditorGrid>("editor/editor_grid.vs", "editor/editor_grid.fs")
		// scan.texture_or(libv::vm4::TextureType::diffuse, "builtin:white")
//		scan.texture_or(libv::vm4::TextureType::specular, "builtin:white"),
//		scan.texture_or(libv::vm4::TextureType::normals, "builtin:up")
	);
}

Material MaterialEditorGrid::create(libv::rev::ResourceManager& rm) {
// 		shader(rm.shader, "editor/editor_grid.vs", "editor/editor_grid.fs") {
	return Material::create<MaterialEditorGrid>(
			rm.shader.load<UniformsEditorGrid>("editor/editor_grid.vs", "editor/editor_grid.fs")
	);
}

void MaterialEditorGrid::bind(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	glr.program(shader.program());

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
