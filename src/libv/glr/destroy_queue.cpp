// Project: libv.glr, File: src/libv/glr/destroy_queue.cpp

// hpp
#include <libv/glr/destroy_queue.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/gl/buffer.hpp>
#include <libv/gl/framebuffer.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/renderbuffer.hpp>
#include <libv/gl/texture.hpp>
#include <libv/gl/vertex_array.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

void DestroyQueues::clear(libv::GL& gl) noexcept {
	for (auto& object : gc_program)
		gl(object).destroy();
	gc_program.clear();

	for (auto& object : gc_framebuffer)
		gl(object).destroy();
	gc_framebuffer.clear();

	for (auto& object : gc_textures)
		gl(object).destroy();
	gc_textures.clear();

	for (auto& object : gc_renderbuffer)
		gl(object).destroy();
	gc_renderbuffer.clear();

	for (auto& object : gc_buffers)
		gl(object).destroy();
	gc_buffers.clear();

	for (auto& object : gc_vertexArrays)
		gl(object).destroy();
	gc_vertexArrays.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
