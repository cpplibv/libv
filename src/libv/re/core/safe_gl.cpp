// Project: libv.re, File: src/libv/re/core/safe_gl.cpp

#include <libv/re/core/destroy_queue.hpp>
#include <libv/re/core/safe_gl.hpp>


namespace libv {
namespace re {
namespace {

// -------------------------------------------------------------------------------------------------

template <typename T>
void queueDestroy(T& object) {
	if (object.id != 0)
		libv::re::r.destroy.gc(object);
}

} // namespace -------------------------------------------------------------------------------------

GLSafeArrayBuffer::~GLSafeArrayBuffer() {
	queueDestroy(static_cast<libv::gl::ArrayBuffer&>(*this));
}
GLSafeBuffer::~GLSafeBuffer() {
	queueDestroy(static_cast<libv::gl::Buffer&>(*this));
}
GLSafeFence::~GLSafeFence() {
	queueDestroy(static_cast<libv::gl::Fence&>(*this));
}
GLSafeFramebuffer::~GLSafeFramebuffer() {
	queueDestroy(static_cast<libv::gl::Framebuffer&>(*this));
}
GLSafePixelBuffer::~GLSafePixelBuffer() {
	queueDestroy(static_cast<libv::gl::PixelBuffer&>(*this));
}
GLSafeProgram::~GLSafeProgram() {
	queueDestroy(static_cast<libv::gl::Program&>(*this));
}
GLSafeQuery::~GLSafeQuery() {
	queueDestroy(static_cast<libv::gl::Query&>(*this));
}
GLSafeRenderbuffer::~GLSafeRenderbuffer() {
	queueDestroy(static_cast<libv::gl::Renderbuffer&>(*this));
}
GLSafeShader::~GLSafeShader() {
	queueDestroy(static_cast<libv::gl::Shader&>(*this));
}
GLSafeShaderStorageBuffer::~GLSafeShaderStorageBuffer() {
	queueDestroy(static_cast<libv::gl::ShaderStorageBuffer&>(*this));
}
GLSafeUniformBuffer::~GLSafeUniformBuffer() {
	queueDestroy(static_cast<libv::gl::UniformBuffer&>(*this));
}
GLSafeIndirectCommandBuffer::~GLSafeIndirectCommandBuffer() {
	queueDestroy(static_cast<libv::gl::IndirectCommandBuffer&>(*this));
}
GLSafeVertexArray::~GLSafeVertexArray() {
	queueDestroy(static_cast<libv::gl::VertexArray&>(*this));
}

GLSafeTexture::~GLSafeTexture() {
	queueDestroy(static_cast<libv::gl::Texture&>(*this));
}
GLSafeTexture1D::~GLSafeTexture1D() {
	queueDestroy(static_cast<libv::gl::Texture1D&>(*this));
}
GLSafeTexture1DArray::~GLSafeTexture1DArray() {
	queueDestroy(static_cast<libv::gl::Texture1DArray&>(*this));
}
GLSafeTexture2D::~GLSafeTexture2D() {
	queueDestroy(static_cast<libv::gl::Texture2D&>(*this));
}
GLSafeTexture2DArray::~GLSafeTexture2DArray() {
	queueDestroy(static_cast<libv::gl::Texture2DArray&>(*this));
}
GLSafeTexture2DMultisample::~GLSafeTexture2DMultisample() {
	queueDestroy(static_cast<libv::gl::Texture2DMultisample&>(*this));
}
GLSafeTexture2DMultisampleArray::~GLSafeTexture2DMultisampleArray() {
	queueDestroy(static_cast<libv::gl::Texture2DMultisampleArray&>(*this));
}
GLSafeTexture3D::~GLSafeTexture3D() {
	queueDestroy(static_cast<libv::gl::Texture3D&>(*this));
}
GLSafeTextureCube::~GLSafeTextureCube() {
	queueDestroy(static_cast<libv::gl::TextureCube&>(*this));
}
GLSafeTextureCubeArray::~GLSafeTextureCubeArray() {
	queueDestroy(static_cast<libv::gl::TextureCubeArray&>(*this));
}
GLSafeTextureRectangle::~GLSafeTextureRectangle() {
	queueDestroy(static_cast<libv::gl::TextureRectangle&>(*this));
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
