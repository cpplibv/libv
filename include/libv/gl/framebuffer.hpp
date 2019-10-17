// File: vertex_buffer.hpp Author: Vader Created on 2016. július 17., 0:51

#pragma once

// ext
#include <GL/glew.h>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/framebuffer_object.hpp>
#include <libv/gl/renderbuffer_object.hpp>
#include <libv/gl/texture_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class AccessFramebuffer {
private:
	Framebuffer& object;

public:
	AccessFramebuffer(Framebuffer& object) :
		object(object) { }

public:
	inline void create() {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		glGenFramebuffers(1, &object.id);
		checkGL();
	}

	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteFramebuffers(1, &object.id);
		object.id = 0;
		checkGL();
	}

private:
	inline void _bind(GLenum target) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
	    glBindFramebuffer(target, object.id);
		checkGL();
	}

	inline void _unbind(GLenum target) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
	    glBindFramebuffer(target, 0);
		checkGL();
	}

public:
	inline void bind() noexcept {
		_bind(GL_FRAMEBUFFER);
	}
	inline void bind_draw() noexcept {
		_bind(GL_DRAW_FRAMEBUFFER);
	}
	inline void bind_read() noexcept {
		_bind(GL_READ_FRAMEBUFFER);
	}

	inline void unbind() noexcept {
		_unbind(GL_FRAMEBUFFER);
	}
	inline void unbind_draw() noexcept {
		_unbind(GL_DRAW_FRAMEBUFFER);
	}
	inline void unbind_read() noexcept {
		_unbind(GL_READ_FRAMEBUFFER);
	}

	[[nodiscard]] inline auto bind_guard() noexcept {
		return bind(), Guard([this] { unbind(); });
	}
	[[nodiscard]] inline auto bind_draw_guard() noexcept {
		return bind_draw(), Guard([this] { unbind_draw(); });
	}
	[[nodiscard]] inline auto bind_read_guard() noexcept {
		return bind_read(), Guard([this] { unbind_read(); });
	}

private:
	template <typename T, typename... Args>
	inline bool oneOf(T test, Args... options) noexcept {
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

	inline void attach2D(Attachment attachment, TextureCube texture, CubeSide side, int32_t level = 0) noexcept {
		attach_draw2D(attachment, texture, side, level);
	}

	inline void attach3D(Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept {
		attach_draw3D(attachment, texture, level, layer);
	}

public:
	inline FramebufferStatus status_draw() noexcept {
		GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		return FramebufferStatus{status};
	}

	inline FramebufferStatus status_read() noexcept {
		GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		return FramebufferStatus{status};
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
