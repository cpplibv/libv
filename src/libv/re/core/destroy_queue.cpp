// Project: libv.re, File: src/libv/re/core/destroy_queue.cpp

#include <libv/re/core/destroy_queue.hpp>

#include <libv/gl/buffer.hpp>
#include <libv/gl/fence.hpp>
#include <libv/gl/framebuffer.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/query.hpp>
#include <libv/gl/renderbuffer.hpp>
#include <libv/gl/texture.hpp>
#include <libv/gl/vertex_array.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void DestroyQueue::gl_destroy(libv::GL& gl) noexcept {
	{
		auto lock = std::unique_lock(shaders_m);
		for (auto& object : shaders)
			gl(object).destroy();
		shaders.clear();
	} {
		auto lock = std::unique_lock(programs_m);
		for (auto& object : programs)
			gl(object).destroy();
		programs.clear();
	} {
		auto lock = std::unique_lock(fences_m);
		for (auto& object : fences)
			gl(object).destroy();
		fences.clear();
	} {
		auto lock = std::unique_lock(framebuffers_m);
		for (auto& object : framebuffers)
			gl(object).destroy();
		framebuffers.clear();
	} {
		auto lock = std::unique_lock(textures_m);
		for (auto& object : textures)
			gl(object).destroy();
		textures.clear();
	} {
		auto lock = std::unique_lock(renderbuffers_m);
		for (auto& object : renderbuffers)
			gl(object).destroy();
		renderbuffers.clear();
	} {
		auto lock = std::unique_lock(buffers_m);
		for (auto& object : buffers)
			gl(object).destroy();
		buffers.clear();
	} {
		auto lock = std::unique_lock(vertexArrays_m);
		for (auto& object : vertexArrays)
			gl(object).destroy();
		vertexArrays.clear();
	} {
		auto lock = std::unique_lock(queries_m);
		for (auto& object : queries)
			gl(object).destroy();
		queries.clear();
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
