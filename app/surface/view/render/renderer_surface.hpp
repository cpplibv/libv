// Created by dbobula on 4/20/2022.

#pragma once

//libv
#include <libv/glr/mesh.hpp>

#include <unordered_map>

//surface
//#include <surface/view/render/shaders.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/rev/texture.hpp>

//#include <surface/view/render/renderer.hpp>
#include <surface/surface/chunk.hpp>
#include <surface/view/render/shaders.hpp>
#include <surface/view/vec_hash.hpp>
#include <surface/view/frustum.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

class RendererResourceContext;

// -------------------------------------------------------------------------------------------------

struct UniformsSurface {
	libv::glr::Uniform_texture textureBase;
	libv::glr::Uniform_bool fogEnabled;
	libv::glr::Uniform_float fogIntensity;
	libv::glr::Uniform_vec4f fogColor;

	template <typename Access> void access_uniforms(Access& access) {
		access(textureBase, "textureBase", textureChannel_diffuse);
		access(fogEnabled, "fogEnabled", true);
		access(fogIntensity, "fogIntensity", 0.05f);
		access(fogColor, "fogColor", libv::vec4f{0.7f, 0.8f, 0.9f, 1.0f});
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

using ShaderSurface = libv::rev::Shader<UniformsSurface>;

// -------------------------------------------------------------------------------------------------

struct RendererSurface {
private:
	struct ChunkMesh {
		int generation = -1;
		libv::vec3f pos;
		libv::vec2f size;
		libv::glr::Mesh mesh{libv::gl::Primitive::TriangleStrip, libv::gl::BufferUsage::StaticDraw};
	};

private:
	std::unordered_map<libv::vec2i, std::vector<uint32_t>, VecHash> stripIndices;
	std::unordered_map<libv::vec2i, ChunkMesh, VecHash> chunkMeshMap;
	ShaderSurface shader;

	libv::rev::Texture texture;

public:
	bool fogEnabled = true;
	float fogIntensity = 0.05f;
	libv::vec4f fogColor = libv::vec4f{0.7f, 0.8f, 0.9f, 1.0f};

public:
	explicit RendererSurface(RendererResourceContext& rctx);

	void addChunk(int generation, const std::shared_ptr<surface::Chunk>& chunk);
	void clear();
//	void addFirstChunk(const surface::Chunk& chunk);

//	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Chunk& chunk);
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const Frustum& frustum);

private:
	void buildMesh(libv::glr::Mesh& mesh, const std::shared_ptr<surface::Chunk>& chunk);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface

