// Created by dbobula on 4/20/2022.

#pragma once

#include <surface/view/render/renderer.hpp>


namespace surface {
using Mesh = libv::glr::Mesh;

struct RendererSurfaceTexture {
	struct ChunkTexture {
		libv::glr::Texture texture;
		libv::vec2f pos; //down-left corner
		libv::vec2f size;
	};
private:
	Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderSurface shader;
//	std::vector<ChunkTexture> chunks;
	std::unordered_map<libv::vec2i, ChunkTexture, VecHash> chunkMap;


	void build_mesh();
public:
	void addTexture(const libv::glr::Texture& texture, const libv::vec2i& index, const libv::vec2f chunkPos, const libv::vec2f size);
	void clear();
//	void addFirstTexture(libv::glr::Texture& texture, const libv::vec2f chunkPos);
	explicit RendererSurfaceTexture(RendererResourceContext& rctx);

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

} // namespace surface

