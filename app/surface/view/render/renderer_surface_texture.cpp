// Project: libv, File: app/surface/view/render/renderer_surface_texture.cpp

#include <surface/view/render/renderer_surface_texture.hpp>

#include <libv/glr/queue.hpp>
#include <libv/math/vec.hpp>

#include <surface/view/render/renderer.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererSurfaceTexture::RendererSurfaceTexture(RendererResourceContext& rctx) :
		shader(rctx.loader.shader, "surface/surface_texture.vs", "surface/surface_texture.fs") {
	build_mesh();
}

void RendererSurfaceTexture::build_mesh() {
	auto position = mesh.attribute(attribute_position);
	auto texture0 = mesh.attribute(attribute_texture0);
	auto index = mesh.index();

	position(0, 0, 0); //left-down
	texture0(0, 0);

	position(1, 0, 0); //right-down
	texture0(1, 0);

	position(1, 1, 0); //right-up
	texture0(1, 1);

	position(0, 1, 0); //left-up
	texture0(0, 1);

	index.quad(0, 1, 2, 3);
}

void RendererSurfaceTexture::addTexture(const libv::glr::Texture& texture, const libv::vec2i& index,
		const libv::vec2f chunkPos, const libv::vec2f size) {
	chunkMap.insert_or_assign(index, ChunkTexture{texture, chunkPos, size});
}

//void RendererSurfaceTexture::clear() {
//	chunks.clear();
//	mesh.clear();
//}

void RendererSurfaceTexture::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	glr.program(shader.program());

	for (const auto&[_, chunk] : chunkMap) {
		auto texture = chunk.texture;
		glr.texture(texture, textureUnit_diffuse);

		auto guard = glr.model.push_guard();
		glr.model.translate(libv::vec3f{chunk.pos - chunk.size * 0.5f, 0});
		glr.model.scale({chunk.size, 1});

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
