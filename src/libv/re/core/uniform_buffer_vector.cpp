// Project: libv.re, File: src/libv/re/core/uniform_buffer_vector.cpp

#include <libv/re/core/uniform_buffer_vector.hpp>

#include <libv/gl/gl.hpp>
#include <libv/gl/uniform_buffer.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void BaseUniformBufferVector::gl_create(libv::GL& gl, libv::gl::UniformBuffer& ubo) {
	gl(ubo).dsa_create();
}
void BaseUniformBufferVector::gl_upload(libv::GL& gl, libv::gl::UniformBuffer& ubo, const void* data, std::size_t size, libv::gl::BufferUsage usage) {
	// Buffer orphaning
	gl(ubo).dsa_data(nullptr, size, usage);
	gl(ubo).dsa_subData(0, data, size);
}
void BaseUniformBufferVector::gl_bind(libv::GL& gl, libv::gl::UniformBuffer& ubo, BlockBinding binding, uint32_t offset, uint32_t size) {
	gl(ubo).bindRange(+binding, offset, size);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
