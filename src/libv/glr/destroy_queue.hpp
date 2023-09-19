// Project: libv.glr, File: src/libv/glr/destroy_queue.hpp

#pragma once

// libv
#include <libv/gl/buffer_object.hpp>
#include <libv/gl/framebuffer_object.hpp>
#include <libv/gl/fwd.hpp>
#include <libv/gl/program_object.hpp>
#include <libv/gl/renderbuffer_object.hpp>
#include <libv/gl/texture_object.hpp>
#include <libv/gl/vertex_array_object.hpp>
// std
#include <vector>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

class DestroyQueues {
	std::vector<libv::gl::Buffer> gc_buffers;
	std::vector<libv::gl::Framebuffer> gc_framebuffer;
	std::vector<libv::gl::Program> gc_program;
	std::vector<libv::gl::Renderbuffer> gc_renderbuffer;
	std::vector<libv::gl::Texture> gc_textures;
	std::vector<libv::gl::VertexArray> gc_vertexArrays;

public:
	inline void gc(libv::gl::Buffer object) {
		gc_buffers.emplace_back(object);
	}
	inline void gc(libv::gl::Framebuffer object) {
		gc_framebuffer.emplace_back(object);
	}
	inline void gc(libv::gl::Program object) {
		gc_program.emplace_back(object);
	}
	inline void gc(libv::gl::Renderbuffer object) {
		gc_renderbuffer.emplace_back(object);
	}
	inline void gc(libv::gl::Texture object) {
		gc_textures.emplace_back(object);
	}
	inline void gc(libv::gl::VertexArray object) {
		gc_vertexArrays.emplace_back(object);
	}

public:
	void clear(libv::GL& gl) noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
