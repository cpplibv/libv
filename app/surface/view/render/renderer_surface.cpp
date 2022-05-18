// Created by dbobula on 4/20/2022.

// hpp
#include <surface/view/render/renderer_surface.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <surface/view/render/renderer.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererSurface::RendererSurface(RendererResourceContext& rctx) :
		shader(rctx.loader.shader, "surface.vs", "surface.fs"),
		texture(rctx.loader.texture.load("dirt.jpg")) {}
//		texture(rctx.loader.texture.load("salt.jpg")) {}
//		texture(rctx.loader.texture.load("grass.jpg")) {}
//		texture(rctx.loader.texture.load("stone.jpg")) {}

void RendererSurface::addChunk(int generation, const std::shared_ptr<surface::Chunk>& chunk) {
	auto& chunkRenderData = chunkMeshMap[chunk->index];
	if (chunkRenderData.generation == generation)
		return;

	chunkRenderData.generation = generation;
	chunkRenderData.pos = libv::vec3f{chunk->position, 0.f};
	chunkRenderData.size = chunk->size;
	chunkRenderData.generation = generation;
	buildMesh(chunkRenderData.mesh, chunk);
}

void RendererSurface::buildMesh(libv::glr::Mesh& mesh, const std::shared_ptr<surface::Chunk>& chunk) {
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

		const auto pL = chunk->height(x - 1, y);
		const auto pR = chunk->height(x + 1, y);
		const auto pU = chunk->height(x, y + 1);
		const auto pD = chunk->height(x, y - 1);

		return normalize(libv::cross(pR - pL, pU - pD));
	};

	const auto rowSize = chunk->height.size_y();
	for (uint32_t y = 0; y < rowSize; y++)
		for (uint32_t x = 0; x < chunk->height.size_x(); ++x)
			normal(calculateNormal(x, y));

	position.set_from_range(chunk->height.span());
	color0.set_from_range(chunk->color.span());

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

void RendererSurface::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Frustum& frustum) {
	glr.program(shader.program());

	glr.uniform(shader.uniform().fogEnabled, fogEnabled);
	glr.uniform(shader.uniform().fogIntensity, fogIntensity);
	glr.uniform(shader.uniform().fogColor, fogColor);
	glr.texture(texture.texture(), textureChannel_diffuse);

	std::erase_if(chunkMeshMap, [&](const auto& pair) {
		const auto& [index, chunkMesh] = pair;

//		const auto eye = glr.eye();
//		const auto chunkPosition = libv::vec3f(index.template cast<float>() * 32.0f, 0);
//		if (libv::length_sq(eye - chunkPosition) > 32.f * 32.f * 8.f * 8.f) // !!! Constants regarding size and chunk count
//			return true;

		const auto eye = glr.eye();
		const auto chunkPosition = index.template cast<float>() * 32.0f;
		if (libv::length_sq(xy(eye) - chunkPosition) > 32.f * 32.f * 8.f * 8.f) // !!! Constants regarding size and chunk count
			return true;

		if (frustum.sphereInFrustum(chunkMesh.pos, chunkMesh.size.length() / 2.f) != Frustum::Position::OUTSIDE) {
			auto uniforms = uniform_stream.block_unique(layout_matrices);
			uniforms[layout_matrices.matMVP] = glr.mvp();
			uniforms[layout_matrices.matM] = glr.model;
			uniforms[layout_matrices.matP] = glr.projection;
			uniforms[layout_matrices.eye] = eye;
			glr.uniform(std::move(uniforms));
			glr.render(chunkMesh.mesh);
		}

		return false;
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
