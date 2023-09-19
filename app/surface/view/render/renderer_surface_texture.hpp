// Project: libv, File: app/surface/view/render/renderer_surface_texture.hpp

#pragma once

// libv
#include <libv/glr/texture.hpp>
#include <libv/glr/mesh.hpp>
// pro
//#include <surface/view/render/renderer.hpp>
#include <surface/view/vec_hash.hpp>
#include <surface/view/render/shaders.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

class RendererResourceContext;

// -------------------------------------------------------------------------------------------------

struct UniformsSurfaceTexture {
//	libv::glr::Uniform_texture texture_noise;

	template <typename Access> void access_uniforms(Access&) {
//		access(texture_noise, "texture_noise", textureUnit_diffuse);
//		access(base_color, "base_color");
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

using ShaderSurfaceTexture = libv::rev::Shader<UniformsSurfaceTexture>;

// -------------------------------------------------------------------------------------------------

struct RendererSurfaceTexture {
	struct ChunkTexture {
		libv::glr::Texture texture;
		libv::vec2f pos; //down-left corner
		libv::vec2f size;
	};

private:
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderSurfaceTexture shader;
//	std::vector<ChunkTexture> chunks;
	std::unordered_map<libv::vec2i, ChunkTexture, VecHash> chunkMap;

private:
	void build_mesh();

public:
	void addTexture(const libv::glr::Texture& texture, const libv::vec2i& index, const libv::vec2f chunkPos, const libv::vec2f size);
	void clear();
//	void addFirstTexture(libv::glr::Texture& texture, const libv::vec2f chunkPos);
	explicit RendererSurfaceTexture(RendererResourceContext& rctx);

	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface


