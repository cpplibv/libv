// Project: libv.rev, File: src/libv/rev/materials/material_red.cpp

// hpp
#include <libv/rev/materials/material_red.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/glr/uniform_buffer.hpp>
// pro
#include <libv/rev/resource/material_scanner.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

Material MaterialRed::create(const MaterialScanner& scan) {
	return Material::create<MaterialRed>(
		scan.shader<UniformsRed>("builtin/red.vs", "builtin/red.fs")
	);
}

void MaterialRed::bind(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
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
