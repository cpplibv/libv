// File:   mesh.cpp Author: Vader Created on 2019. január 25., 6:27

// hpp
#include <libv/glr/remote.hpp>
// libv
#include <libv/gl/assert.hpp>
#include <libv/gl/buffer.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/texture.hpp>
#include <libv/gl/vertex_array.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

Remote::Remote() {
}

Remote::~Remote() {
	if (initalized) {
		LIBV_GL_DEBUG_ASSERT(std::this_thread::get_id() == contextThreadID);
		destroy();
	}
}

Queue Remote::queue() {
	return {};
}

void Remote::queue(Queue&& queue) {
	queues.emplace_back(std::move(queue));
}

void Remote::clear() {
	queues.clear();

	for (auto& object : gc_program)
		gl(object).destroy();
	gc_program.clear();

	for (auto& object : gc_textures)
		gl(object).destroy();
	gc_textures.clear();

	for (auto& object : gc_buffers)
		gl(object).destroy();
	gc_buffers.clear();

	for (auto& object : gc_vertexArrays)
		gl(object).destroy();
	gc_vertexArrays.clear();
}

void Remote::enableTrace() {
	gl.enableTrace();
}

void Remote::enableDebug() {
	gl.enableDebug();
}

void Remote::create() {
	// load glew
	contextThreadID = std::this_thread::get_id();
	LIBV_GL_DEBUG_ASSERT(!initalized);
	initalized = true;
}

void Remote::destroy() {
	LIBV_GL_DEBUG_ASSERT(std::this_thread::get_id() == contextThreadID);
	LIBV_GL_DEBUG_ASSERT(initalized);
	clear();
	initalized = false;
}

void Remote::render() {
	for (auto& queue : queues) {
		queue.execute(gl, *this);
	}
	clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
