// Created by dbobula on 4/20/2022.

#pragma once

//libv
#include <libv/glr/mesh.hpp>

//surface
//#include <surface/view/render/shaders.hpp>
#include <surface/surface/chunk.hpp>
#include <libv/glr/uniform_buffer.hpp>

#include <surface/view/render/renderer.hpp>



namespace surface {
using Mesh = libv::glr::Mesh;

struct RendererSurface {
//private:
//	struct ChunkMesh {
//		Mesh mesh{libv::gl::Primitive::TriangleStrip, libv::gl::BufferUsage::StaticDraw};
////		Chunk chunk;
//	};
private:
//	Mesh mesh{libv::gl::Primitive::TriangleStrip, libv::gl::BufferUsage::StaticDraw};
	std::unordered_map<libv::vec2i, Mesh, VecHash> chunkMeshMap;
	ShaderSurface shader;
//	libv::glr::VertexIndex vi = 0;
//	bool dirty = true;

//	void build_mesh(const std::vector<surface::Chunk>& chunks);
public:
	explicit RendererSurface(RendererResourceContext& rctx);

	void addChunk(const std::shared_ptr<surface::Chunk>& chunk);
	void clear();
//	void addFirstChunk(const surface::Chunk& chunk);

//	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Chunk& chunk);
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

} // namespace surface


