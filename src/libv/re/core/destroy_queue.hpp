// Project: libv.re, File: src/libv/re/core/destroy_queue.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/gl/buffer_object.hpp>
#include <libv/gl/fence_object.hpp>
#include <libv/gl/framebuffer_object.hpp>
#include <libv/gl/program_object.hpp>
#include <libv/gl/query_object.hpp>
#include <libv/gl/renderbuffer_object.hpp>
#include <libv/gl/shader_object.hpp>
#include <libv/gl/texture_object.hpp>
#include <libv/gl/vertex_array_object.hpp>
#include <libv/re/core/engine.hpp> // Convenience include to access the libv::re::r.destroy object

#include <mutex>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class DestroyQueue {
	std::mutex buffers_m;
	std::vector<libv::gl::Buffer> buffers;
	std::mutex fences_m;
	std::vector<libv::gl::Fence> fences;
	std::mutex framebuffers_m;
	std::vector<libv::gl::Framebuffer> framebuffers;
	std::mutex programs_m;
	std::vector<libv::gl::Program> programs;
	std::mutex renderbuffers_m;
	std::vector<libv::gl::Renderbuffer> renderbuffers;
	std::mutex shaders_m;
	std::vector<libv::gl::Shader> shaders;
	std::mutex textures_m;
	std::vector<libv::gl::Texture> textures;
	std::mutex vertexArrays_m;
	std::vector<libv::gl::VertexArray> vertexArrays;
	std::mutex queries_m;
	std::vector<libv::gl::Query> queries;

public:
	inline void gc(libv::gl::Buffer object) {
		auto lock = std::unique_lock(buffers_m);
		buffers.emplace_back(object);
	}
	inline void gc(libv::gl::Fence object) {
		auto lock = std::unique_lock(fences_m);
		fences.emplace_back(object);
	}
	inline void gc(libv::gl::Framebuffer object) {
		auto lock = std::unique_lock(framebuffers_m);
		framebuffers.emplace_back(object);
	}
	inline void gc(libv::gl::Program object) {
		auto lock = std::unique_lock(programs_m);
		programs.emplace_back(object);
	}
	inline void gc(libv::gl::Renderbuffer object) {
		auto lock = std::unique_lock(renderbuffers_m);
		renderbuffers.emplace_back(object);
	}
	inline void gc(libv::gl::Shader object) {
		auto lock = std::unique_lock(shaders_m);
		shaders.emplace_back(object);
	}
	inline void gc(libv::gl::Texture object) {
		auto lock = std::unique_lock(textures_m);
		textures.emplace_back(object);
	}
	inline void gc(libv::gl::VertexArray object) {
		auto lock = std::unique_lock(vertexArrays_m);
		vertexArrays.emplace_back(object);
	}
	inline void gc(libv::gl::Query object) {
		auto lock = std::unique_lock(vertexArrays_m);
		queries.emplace_back(object);
	}

public:
	void gl_destroy(libv::GL& gl) noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
