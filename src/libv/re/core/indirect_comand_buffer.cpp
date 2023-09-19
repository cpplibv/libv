// Project: libv.re, File: src/libv/re/core/indirect_comand_buffer.cpp

#include <libv/re/core/indirect_comand_buffer.hpp>

#include <libv/gl/gl.hpp>
#include <libv/gl/indirect_command_buffer.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

IndirectCommandBuffer::IndirectCommandBuffer(libv::gl::BufferUsage usage) :
	usage(usage) {}

IndirectCommandBuffer::~IndirectCommandBuffer() {
	// For the sake of forward declared types
}

IndirectCommandBufferOffset IndirectCommandBuffer::add(std::span<const libv::gl::IndirectCommandDrawElements> commands) {
	const auto offset = static_cast<IndirectCommandBufferOffset>(data.size());
	data.insert(data.end(), commands.begin(), commands.end());
	return offset * sizeof(libv::gl::IndirectCommandDrawElements);
}

void IndirectCommandBuffer::gl_create(libv::GL& gl) {
	gl(icbo).dsa_create();
}

void IndirectCommandBuffer::gl_swap(libv::GL& gl) {
	if (!created) {
		gl_create(gl);
		created = true;
	}

	data.clear();
}

void IndirectCommandBuffer::gl_upload(libv::GL& gl) {
	if (!data.empty()) {
		const auto size = data.size() * sizeof(libv::gl::IndirectCommandDrawElements);
		gl(icbo).dsa_data(nullptr, size, usage); // Buffer orphaning
		gl(icbo).dsa_subData(0, data.data(), size);
	}
}

void IndirectCommandBuffer::gl_bind(libv::GL& gl) {
	if (!data.empty())
		gl(icbo).bind();
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
