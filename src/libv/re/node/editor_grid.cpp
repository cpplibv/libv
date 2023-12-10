// Project: libv.re, File: src/libv/re/node/editorGrid.cpp

#include <libv/re/node/editor_grid.hpp>

#include <libv/re/material/material.hpp>
#include <libv/re/mesh/mesh.hpp>
#include <libv/re/resource/shader_loader.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(EditorGrid* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(EditorGrid* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

EditorGrid::EditorGrid() :
	Object(libv::vec3f{},
		Material::create(r.shader.load("shader/re/editor/editor_grid.vs", "shader/re/editor/editor_grid.fs"), nullptr),
		Mesh::create()) {

	material->layers = layerTransparent | layerNoFrustumCull;

	material->state.depthTest(true);
	material->state.depthWrite(false);
	material->state.blend(true);
	material->state.blendSrc(libv::gl::BlendFunction::SourceAlpha);
	material->state.blendDst(libv::gl::BlendFunction::One_Minus_SourceAlpha);

	mesh->primitive = libv::gl::Primitive::Triangles;

	const uint32_t numVertices = 4;
	const uint32_t numIndices = 6 * 2;

	auto position = mesh->addAttribute(libv::gl::BufferUsage::StaticDraw, attrPosition, numVertices);
	auto index = mesh->addIndex<uint32_t>(libv::gl::BufferUsage::StaticDraw, numIndices);

	position[0] = libv::vec3f{-1, -1, 0};
	position[1] = libv::vec3f{+1, -1, 0};
	position[2] = libv::vec3f{+1, +1, 0};
	position[3] = libv::vec3f{-1, +1, 0};
	index[0 + 0] = 0; // Top face
	index[0 + 1] = 1;
	index[0 + 2] = 2;
	index[0 + 3] = 2;
	index[0 + 4] = 3;
	index[0 + 5] = 0;
	index[6 + 0] = 0; // Bottom face
	index[6 + 1] = 3;
	index[6 + 2] = 2;
	index[6 + 3] = 2;
	index[6 + 4] = 1;
	index[6 + 5] = 0;
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
