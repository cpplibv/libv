// Project: libv.re, File: src/libv/re/core/indirect_comand_buffer.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/gl/indirect_command.hpp>
#include <libv/re/core/safe_gl.hpp>

#include <vector>
#include <span>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

using IndirectCommandBufferOffset = uint32_t;

class IndirectCommandBuffer {
public:
	std::vector<libv::gl::IndirectCommandDrawElements> data;
	libv::gl::BufferUsage usage = libv::gl::BufferUsage::StreamDraw;
	bool created = false;

private:
	GLSafeIndirectCommandBuffer icbo;

public:
	explicit IndirectCommandBuffer(libv::gl::BufferUsage usage);
	~IndirectCommandBuffer();

public:
	IndirectCommandBufferOffset add(std::span<const libv::gl::IndirectCommandDrawElements> commands);

private:
	void gl_create(libv::GL& gl);
public:
	void gl_swap(libv::GL& gl);
	void gl_upload(libv::GL& gl);
	void gl_bind(libv::GL& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
