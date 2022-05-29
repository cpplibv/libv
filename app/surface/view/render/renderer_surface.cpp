// Created by dbobula on 4/20/2022.

// hpp
#include <surface/view/render/renderer_surface.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/utility/timer.hpp>
// pro
#include <surface/log.hpp>
#include <surface/surface/surface_constants.hpp>
#include <surface/view/render/renderer.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererSurface::RendererSurface(RendererResourceContext& rctx) :
		shader(rctx.loader.shader, "surface/surface.vs", "surface/surface.fs"),
		texture(rctx.loader.texture.load("dirt_grayscale.jpg")) {}
//		texture(rctx.loader.texture.load("dirt.jpg")) {}
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
	libv::Timer timer;

	mesh.clear();

	auto position = mesh.attribute(attribute_position);
	auto normal = mesh.attribute(attribute_normal);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();

	auto& cachedIndices = stripIndices[chunk->height.size().cast<int32_t>()];
	if (cachedIndices.empty()) {
		libv::glr::VertexIndex vi = 0;
		const auto rowSize = chunk->height.size_y();
		for (size_t i = 0; i < rowSize - 1; ++i) {
			cachedIndices.emplace_back(vi);
			const auto colSize = chunk->height.size_x();
			const auto colSizeVi = static_cast<libv::glr::VertexIndex>(colSize);

			for (size_t j = 0; j < colSize; ++j) {
				cachedIndices.emplace_back(vi);
				cachedIndices.emplace_back(vi + colSizeVi);
				vi += 1;
			}
			cachedIndices.emplace_back(vi + colSizeVi - 1);
		}
		vi += static_cast<libv::glr::VertexIndex>(rowSize);
	}

	position.set_from_range(chunk->height.span());
	normal.set_from_range(chunk->normal.span());
	color0.set_from_range(chunk->color.span());
	index.set_from_range(cachedIndices);

//	auto t0 = timer.timef_ms().count();
//	log_surface.trace_if(t0 > 0.01f, "{:25}:{:8.4f} ms {}", "buildMesh", t0, chunk->index);
}

void RendererSurface::clear() {
	chunkMeshMap.clear();
}

void RendererSurface::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Frustum& frustum) {
	glr.program(shader.program());

	glr.uniform(shader.uniform().fogEnabled, fogEnabled);
	glr.uniform(shader.uniform().fogIntensity, fogIntensity);
	glr.uniform(shader.uniform().fogColor, fogColor);
	glr.uniform(shader.uniform().sunColor, sunColor);
	glr.uniform(shader.uniform().sunDirection, sunDirection);
	glr.texture(texture.texture(), textureChannel_diffuse);

	std::erase_if(chunkMeshMap, [&](const auto& pair) {
		const auto& [index, chunkMesh] = pair;

//		const auto eye = glr.eye();
//		const auto chunkPosition = libv::vec3f(index.template cast<float>() * 32.0f, 0);
//		if (libv::length_sq(eye - chunkPosition) > 32.f * 32.f * 8.f * 8.f) // !!! Constants regarding size and chunk count
//			return true;

		const auto eye = glr.eye();
		const auto chunkPosition = index.template cast<float>() * 32.0f;
		const auto threshold = worldRangeUnloadRender;
		if (libv::length_sq(xy(eye) - chunkPosition) > threshold * threshold) { // !!! Constants regarding size and chunk count
//			log_surface.error("Destroying render mesh {} (range: {}) eye {}", index, libv::length(xy(eye) - chunkPosition), eye);
			return true;
		}

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
