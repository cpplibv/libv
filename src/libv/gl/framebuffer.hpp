// Project: libv.gl, File: src/libv/gl/framebuffer.hpp

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/utility/guard.hpp>
// std
#include <span>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/framebuffer_object.hpp>
#include <libv/gl/renderbuffer_object.hpp>
#include <libv/gl/texture_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class AccessFramebuffer {
private:
	Framebuffer& object;
	GL& gl;

public:
	inline AccessFramebuffer(Framebuffer& object, GL& gl) noexcept :
		object(object), gl(gl) { }

public:
	inline void create() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		glGenFramebuffers(1, &object.id);
		checkGL();
	}

	inline void destroy() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteFramebuffers(1, &object.id);
		object.id = 0;
		checkGL();
	}

public:
	inline void bind() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.framebuffer(object);
		checkGL();
	}
	inline void bind_draw() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.framebuffer_draw(object);
		checkGL();
	}
	inline void bind_read() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.framebuffer_read(object);
		checkGL();
	}

	inline void unbind() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.framebuffer_default();
		checkGL();
	}
	inline void unbind_draw() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.framebuffer_default_draw();
		checkGL();
	}
	inline void unbind_read() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.framebuffer_default_read();
		checkGL();
	}

	[[nodiscard]] inline auto bind_guard() noexcept {
		return bind(), libv::guard([this] { unbind(); });
	}
	[[nodiscard]] inline auto bind_draw_guard() noexcept {
		return bind_draw(), libv::guard([this] { unbind_draw(); });
	}
	[[nodiscard]] inline auto bind_read_guard() noexcept {
		return bind_read(), libv::guard([this] { unbind_read(); });
	}

private:
	template <typename T, typename... Args>
	[[nodiscard]] inline bool oneOf(T test, Args... options) noexcept {
		return ((test == options) || ...);
	}

	inline void _texture1D(GLenum mode, Attachment attachment, GLenum target, int32_t texture, int32_t level) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture != 0);
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glFramebufferTexture1D(mode, libv::to_value(attachment), target, texture, level);
		checkGL();
	}

	inline void _texture2D(GLenum mode, Attachment attachment, GLenum target, int32_t texture, int32_t level) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture != 0);
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glFramebufferTexture2D(mode, libv::to_value(attachment), target, texture, level);
		checkGL();
	}

	inline void _texture2DLayer(GLenum mode, Attachment attachment, int32_t texture, int32_t level, int32_t layer) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture != 0);
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glFramebufferTextureLayer(mode, libv::to_value(attachment), texture, level, layer);
		checkGL();
	}

	inline void _texture3D(GLenum mode, Attachment attachment, GLenum target, int32_t texture, int32_t level, int32_t layer) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture != 0);
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glFramebufferTexture3D(mode, libv::to_value(attachment), target, texture, level, layer);
		checkGL();
	}

public:
	// NOTE: glFramebufferTexture is not implemented as specs are very unclear on level / layer
	//	inline void attach_draw(Attachment attachment, Texture texture, int32_t level = 0) noexcept {
	//		oneOf(texture.target,
	//				TextureTarget::_3D,
	//				TextureTarget::CubeMapArray,
	//				TextureTarget::CubeMap,
	//				TextureTarget::_1DArray,
	//				TextureTarget::_2DArray,
	//				TextureTarget::_2DMultisampleArray
	//		);
	// OpenGL Spec: If target matches list above the specified texture level is an array of images, and the framebuffer attachment is considered to be layered.
	//
	//		glFramebufferTexture
	//		_texture(GL_DRAW_FRAMEBUFFER, attachment, libv::to_value(texture.target), texture.id, level);
	//		_texture(GL_READ_FRAMEBUFFER, attachment, libv::to_value(texture.target), texture.id, level);
	//	}

	inline void attach_draw(Attachment attachment, Renderbuffer renderbuffer) noexcept {
		LIBV_GL_DEBUG_ASSERT(renderbuffer.id != 0);
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, libv::to_value(attachment), GL_RENDERBUFFER, renderbuffer.id);
	}

	inline void attach_read(Attachment attachment, Renderbuffer renderbuffer) noexcept {
		LIBV_GL_DEBUG_ASSERT(renderbuffer.id != 0);
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, libv::to_value(attachment), GL_RENDERBUFFER, renderbuffer.id);
	}

	inline void attach_draw1D(Attachment attachment, Texture texture, int32_t level = 0) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture.target == TextureTarget::_1D);

		_texture1D(GL_DRAW_FRAMEBUFFER, attachment, libv::to_value(texture.target), texture.id, level);
	}

	inline void attach_read1D(Attachment attachment, Texture texture, int32_t level = 0) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture.target == TextureTarget::_1D);

		_texture1D(GL_READ_FRAMEBUFFER, attachment, libv::to_value(texture.target), texture.id, level);
	}

	inline void attach_draw2D(Attachment attachment, Texture texture, int32_t level = 0) noexcept {
		LIBV_GL_DEBUG_ASSERT(oneOf(texture.target, TextureTarget::_2D, TextureTarget::Rectangle, TextureTarget::_2DMultisample));
		LIBV_GL_DEBUG_ASSERT(!oneOf(texture.target, TextureTarget::Rectangle, TextureTarget::_2DMultisample) || level == 0);

		_texture2D(GL_DRAW_FRAMEBUFFER, attachment, libv::to_value(texture.target), texture.id, level);
	}

	inline void attach_read2D(Attachment attachment, Texture texture, int32_t level = 0) noexcept {
		LIBV_GL_DEBUG_ASSERT(oneOf(texture.target, TextureTarget::_2D, TextureTarget::Rectangle, TextureTarget::_2DMultisample));
		LIBV_GL_DEBUG_ASSERT(!oneOf(texture.target, TextureTarget::Rectangle, TextureTarget::_2DMultisample) || level == 0);

		_texture2D(GL_READ_FRAMEBUFFER, attachment, libv::to_value(texture.target), texture.id, level);
	}

	inline void attach_draw2D(Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture.target == TextureTarget::_2DArray);
		_texture2DLayer(GL_DRAW_FRAMEBUFFER, attachment, texture.id, level, layer);
	}

	inline void attach_read2D(Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture.target == TextureTarget::_2DArray);
		_texture2DLayer(GL_READ_FRAMEBUFFER, attachment, texture.id, level, layer);
	}

	inline void attach_draw2D(Attachment attachment, Texture texture, CubeSide side, int32_t level = 0) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture.target == TextureTarget::CubeMap);
		_texture2D(GL_DRAW_FRAMEBUFFER, attachment, libv::to_value(side), texture.id, level);
	}

	inline void attach_read2D(Attachment attachment, Texture texture, CubeSide side, int32_t level = 0) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture.target == TextureTarget::CubeMap);
		_texture2D(GL_READ_FRAMEBUFFER, attachment, libv::to_value(side), texture.id, level);
	}

	inline void attach_draw2D(Attachment attachment, TextureCube texture, CubeSide side, int32_t level = 0) noexcept {
		_texture2D(GL_DRAW_FRAMEBUFFER, attachment, libv::to_value(side), texture.id, level);
	}

	inline void attach_read2D(Attachment attachment, TextureCube texture, CubeSide side, int32_t level = 0) noexcept {
		_texture2D(GL_READ_FRAMEBUFFER, attachment, libv::to_value(side), texture.id, level);
	}

	inline void attach_draw3D(Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture.target == TextureTarget::_3D);

		_texture3D(GL_DRAW_FRAMEBUFFER, attachment, libv::to_value(texture.target), texture.id, level, layer);
	}

	inline void attach_read3D(Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept {
		LIBV_GL_DEBUG_ASSERT(texture.target == TextureTarget::_3D);

		_texture3D(GL_READ_FRAMEBUFFER, attachment, libv::to_value(texture.target), texture.id, level, layer);
	}

	inline void attach(Attachment attachment, Renderbuffer renderbuffer) noexcept {
		attach_draw(attachment, renderbuffer);
	}

	inline void attach1D(Attachment attachment, Texture texture, int32_t level = 0) noexcept {
		attach_draw1D(attachment, texture, level);
	}

	inline void attach2D(Attachment attachment, Texture texture, int32_t level = 0) noexcept {
		attach_draw2D(attachment, texture, level);
	}

	inline void attach2D(Attachment attachment, Texture texture, CubeSide side, int32_t level = 0) noexcept {
		attach_draw2D(attachment, texture, side, level);
	}

	inline void attach2D(Attachment attachment, TextureCube texture, CubeSide side, int32_t level = 0) noexcept {
		attach_draw2D(attachment, texture, side, level);
	}

	inline void attach3D(Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept {
		attach_draw3D(attachment, texture, level, layer);
	}

public:
	inline void readBuffer(Attachment attachment) noexcept {
		glReadBuffer(libv::to_underlying(attachment));
		checkGL();
	}

	inline void drawBuffer(Attachment attachment) noexcept {
		glDrawBuffer(libv::to_underlying(attachment));
		checkGL();
	}
	inline void drawBuffers(Attachment a0) noexcept {
		glDrawBuffers(1, reinterpret_cast<GLenum*>(&a0));
		checkGL();
	}
	inline void drawBuffers(Attachment a0, Attachment a1) noexcept {
		Attachment attachments[] = {a0, a1};
		glDrawBuffers(2, reinterpret_cast<GLenum*>(attachments));
		checkGL();
	}
	inline void drawBuffers(Attachment a0, Attachment a1, Attachment a2) noexcept {
		Attachment attachments[] = {a0, a1, a2};
		glDrawBuffers(3, reinterpret_cast<GLenum*>(attachments));
		checkGL();
	}
	inline void drawBuffers(Attachment a0, Attachment a1, Attachment a2, Attachment a3) noexcept {
		Attachment attachments[] = {a0, a1, a2, a3};
		glDrawBuffers(4, reinterpret_cast<GLenum*>(attachments));
		checkGL();
	}
	inline void drawBuffers(std::span<Attachment> attachments) noexcept {
		glDrawBuffers(
				static_cast<GLsizei>(attachments.size()),
				reinterpret_cast<GLenum*>(attachments.data()));
		checkGL();
	}

public:
	[[nodiscard]] inline FramebufferStatus status_draw() const noexcept {
		GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		return FramebufferStatus{status};
	}

	[[nodiscard]] inline FramebufferStatus status_read() const noexcept {
		GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		return FramebufferStatus{status};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
