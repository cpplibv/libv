// Project: libv.re, File: src/libv/re/core/safe_gl.hpp

#pragma once

#include <libv/gl/array_buffer_object.hpp>
#include <libv/gl/buffer_object.hpp>
#include <libv/gl/fence_object.hpp>
#include <libv/gl/framebuffer_object.hpp>
#include <libv/gl/indirect_command_buffer_object.hpp>
#include <libv/gl/pixel_buffer_object.hpp>
#include <libv/gl/program_object.hpp>
#include <libv/gl/query_object.hpp>
#include <libv/gl/renderbuffer_object.hpp>
#include <libv/gl/shader_object.hpp>
#include <libv/gl/shader_storage_buffer_object.hpp>
#include <libv/gl/texture_object.hpp>
#include <libv/gl/uniform_buffer_object.hpp>
#include <libv/gl/vertex_array_object.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct GLSafeArrayBuffer : libv::gl::ArrayBuffer {
	~GLSafeArrayBuffer();
};
struct GLSafeBuffer : libv::gl::Buffer {
	~GLSafeBuffer();
};
struct GLSafeFence : libv::gl::Fence {
	~GLSafeFence();
};
struct GLSafeFramebuffer : libv::gl::Framebuffer {
	~GLSafeFramebuffer();
};
struct GLSafePixelBuffer : libv::gl::PixelBuffer {
	~GLSafePixelBuffer();
};
struct GLSafeProgram : libv::gl::Program {
	~GLSafeProgram();
};
struct GLSafeQuery : libv::gl::Query {
	~GLSafeQuery();
};
struct GLSafeRenderbuffer : libv::gl::Renderbuffer {
	~GLSafeRenderbuffer();
};
struct GLSafeShader : libv::gl::Shader {
	~GLSafeShader();
};
struct GLSafeShaderStorageBuffer : libv::gl::ShaderStorageBuffer {
	~GLSafeShaderStorageBuffer();
};
struct GLSafeUniformBuffer : libv::gl::UniformBuffer {
	~GLSafeUniformBuffer();
};
struct GLSafeIndirectCommandBuffer : libv::gl::IndirectCommandBuffer {
	~GLSafeIndirectCommandBuffer();
};
struct GLSafeVertexArray : libv::gl::VertexArray {
	~GLSafeVertexArray();
};

struct GLSafeTexture : libv::gl::Texture {
	~GLSafeTexture();
};
struct GLSafeTexture1D : libv::gl::Texture1D {
	~GLSafeTexture1D();
};
struct GLSafeTexture1DArray : libv::gl::Texture1DArray {
	~GLSafeTexture1DArray();
};
struct GLSafeTexture2D : libv::gl::Texture2D {
	~GLSafeTexture2D();
};
struct GLSafeTexture2DArray : libv::gl::Texture2DArray {
	~GLSafeTexture2DArray();
};
struct GLSafeTexture2DMultisample : libv::gl::Texture2DMultisample {
	~GLSafeTexture2DMultisample();
};
struct GLSafeTexture2DMultisampleArray : libv::gl::Texture2DMultisampleArray {
	~GLSafeTexture2DMultisampleArray();
};
struct GLSafeTexture3D : libv::gl::Texture3D {
	~GLSafeTexture3D();
};
struct GLSafeTextureCube : libv::gl::TextureCube {
	~GLSafeTextureCube();
};
struct GLSafeTextureCubeArray : libv::gl::TextureCubeArray {
	~GLSafeTextureCubeArray();
};
struct GLSafeTextureRectangle : libv::gl::TextureRectangle {
	~GLSafeTextureRectangle();
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
