// Created by dbobula on 4/20/2022.

// hpp
#include <surface/view/render/renderer_surface.hpp>
// libv
#include <libv/glr/queue.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererSurface::RendererSurface(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "surface.vs", "surface.fs") {}

void RendererSurface::addChunk(int generation, const std::shared_ptr<surface::Chunk>& chunk) {
	auto& chunkRenderData = chunkMeshMap[chunk->index];
	if (chunkRenderData.generation == generation)
		return;

	chunkRenderData.generation = generation;
	buildMesh(chunkRenderData.mesh, chunk);
}


void RendererSurface::buildMesh(Mesh& mesh, const std::shared_ptr<surface::Chunk>& chunk) {
	mesh.clear();
	auto position = mesh.attribute(attribute_position);
	auto normal = mesh.attribute(attribute_normal);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();
	libv::glr::VertexIndex vi = 0;

	const auto calculateNormal = [&](uint32_t x, uint32_t y) {
		if (x == 0 || x == chunk->height.size_x() - 1)
			return libv::vec3f{0, 0, 1};
		if (y == 0 || y == chunk->height.size_y() - 1)
			return libv::vec3f{0, 0, 1};

		const auto pL = chunk->height(x - 1, y).pos;
		const auto pR = chunk->height(x + 1, y).pos;
		const auto pU = chunk->height(x, y + 1).pos;
		const auto pD = chunk->height(x, y - 1).pos;

		return normalize(libv::cross(pR - pL, pU - pD));
	};

	const auto rowSize = chunk->height.size_y();
	for (uint32_t y = 0; y < rowSize; y++) {
		for (uint32_t x = 0; x < chunk->height.size_x(); ++x) {
			position(chunk->height(x, y).pos);
			normal(calculateNormal(x, y));
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
	for (const auto&[_, chunkMesh] : chunkMeshMap) {
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(chunkMesh.mesh);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
