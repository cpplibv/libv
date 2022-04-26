// Created by dbobula on 4/20/2022.

#include <surface/view/render/renderer_surface.hpp>

//libv
#include <libv/glr/queue.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------
RendererSurface::RendererSurface(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "flat_color.vs", "flat_color.fs") {}

void RendererSurface::addChunk(const std::shared_ptr<surface::Chunk>& chunk) {
	const auto& pair = chunkMeshMap.emplace(chunk->index, libv::glr::Mesh{libv::gl::Primitive::TriangleStrip, libv::gl::BufferUsage::StaticDraw});
	auto& mesh = pair.first->second;
	if(!pair.second)
		mesh.clear();

	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();
	libv::glr::VertexIndex vi = 0;

	const auto rowSize = chunk->height.size_y();
	for (unsigned int y = 0; y < rowSize; y++) {
		for (unsigned int x = 0; x < chunk->height.size_x(); ++x) {
			position(chunk->height(x, y).pos);
			color0(chunk->biomeMap(x, y));
		}
	}

	for (size_t i = 0; i < rowSize - 1; ++i) {
		index(vi);
		const auto colSize = chunk->height.size_x();
		const auto colSizeVi = static_cast<libv::glr::VertexIndex>(colSize);

		for (size_t j = 0; j < colSize; ++j) {
			index(vi);
			index(vi + colSizeVi);
			vi += 1;
		}
		index(vi + colSizeVi - 1);
	}
	vi += static_cast<libv::glr::VertexIndex>(rowSize);
}

void RendererSurface::clear() {
	chunkMeshMap.clear();
}

void RendererSurface::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {

	glr.program(shader.program());
	for (const auto& [_, mesh] : chunkMeshMap) {
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh);
	}
}
// -------------------------------------------------------------------------------------------------

} // namespace surface
