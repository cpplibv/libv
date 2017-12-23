// File: texture.hpp, Created on 2014. november 28. 17:44, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
// pro
#include <libv/gl/assert.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/texture_object.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

template <typename Texture>
class AccessTexture {
private:
	Texture& object;
	GL& gl;

public:
	inline AccessTexture(Texture& object, GL& gl) noexcept :
		object(object), gl(gl) { }

public:
	inline void create() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id == 0);
		glGenTextures(1, &object.id);
		checkGL();
	}

	inline void destroy() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glDeleteTextures(1, &object.id);
		object.id = 0;
		checkGL();
	}

public:
	inline void bind() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.bind(object);
		checkGL();
	}
	inline void unbind() noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		gl.unbind(object);
		checkGL();
	}

public:
	struct InternalFormat {
		GLenum format;
		BaseInternalFormat base;

		InternalFormat(BaseInternalFormat format) noexcept :
			format(to_value(format)),
			base(format)  { }
		InternalFormat(CompressedFormat format) noexcept :
			format(to_value(format)),
			base(compatibleBaseFormat(format)) { }
		InternalFormat(SizedInternalFormat format) noexcept :
			format(to_value(format)),
			base(compatibleBaseFormat(format)) { }
		InternalFormat(DepthFormat format) noexcept :
			format(to_value(format)),
			base(compatibleBaseFormat(format)) { }
		InternalFormat(DepthStencilFormat format) noexcept :
			format(to_value(format)),
			base(compatibleBaseFormat(format)) { }
		InternalFormat(StencilFormat format) noexcept :
			format(to_value(format)),
			base(compatibleBaseFormat(format)) { }
	};

private:
	inline void emulateStorage1D(GLint levels, GLenum internalFormat, BaseInternalFormat format, uint32_t width) noexcept {
		object.template assert_target<TextureTarget::_1D>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(to_value(object.target), GL_TEXTURE_MAX_LEVEL, levels - 1);

		for (GLint i = 0; i < levels; i++) {
			glTexImage1D(to_value(object.target), i, internalFormat, width, 0, to_value(format), GL_BYTE, nullptr);
			width = std::max(1u, width / 2);
		}
		checkGL();
	}
	inline void emulateStorage2D(GLint levels, GLenum internalFormat, BaseInternalFormat format, uint32_t width, uint32_t height) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::CubeMap, TextureTarget::Rectangle>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(to_value(object.target), GL_TEXTURE_MAX_LEVEL, levels - 1);

		if (object.target == TextureTarget::_2D || object.target == TextureTarget::Rectangle) {
			for (GLint i = 0; i < levels; i++) {
				glTexImage2D(to_value(object.target), i, internalFormat, width, height, 0, to_value(format), GL_BYTE, nullptr);
				width = std::max(1u, width / 2);
				height = std::max(1u, height / 2);
			}

		} else if (object.target == TextureTarget::CubeMap) {
			for (GLint i = 0; i < levels; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, i, internalFormat, width, height, 0, to_value(format), GL_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, i, internalFormat, width, height, 0, to_value(format), GL_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, i, internalFormat, width, height, 0, to_value(format), GL_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, i, internalFormat, width, height, 0, to_value(format), GL_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, i, internalFormat, width, height, 0, to_value(format), GL_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, i, internalFormat, width, height, 0, to_value(format), GL_BYTE, nullptr);

				width = std::max(1u, width / 2);
				height = std::max(1u, height / 2);
			}

		} else if (object.target == TextureTarget::_1DArray) {
			for (GLint i = 0; i < levels; i++) {
				glTexImage2D(to_value(object.target), i, internalFormat, width, height, 0, to_value(format), GL_BYTE, nullptr);
				width = std::max(1u, width / 2);
			}
		}
		checkGL();
	}
	inline void emulateStorage3D(GLint levels, GLenum internalFormat, BaseInternalFormat format, uint32_t width, uint32_t height, uint32_t depth) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(to_value(object.target), GL_TEXTURE_MAX_LEVEL, levels - 1);

		if (object.target == TextureTarget::_3D) {
			for (GLint i = 0; i < levels; i++) {
				glTexImage3D(to_value(object.target), i, internalFormat, width, height, depth, 0, to_value(format), GL_BYTE, nullptr);
				width = std::max(1u, width / 2);
				height = std::max(1u, height / 2);
				depth = std::max(1u, depth / 2);
			}

		} else if (object.target == TextureTarget::_2DArray || object.target == TextureTarget::CubeMapArray) {
			for (GLint i = 0; i < levels; i++) {
				glTexImage3D(to_value(object.target), i, internalFormat, width, height, depth, 0, to_value(format), GL_BYTE, nullptr);
				width = std::max(1u, width / 2);
				height = std::max(1u, height / 2);
			}
		}

		checkGL();
	}

public:
	// glTexStorage1D(to_value(object.target), levels, to_value(internalFormat), width);
	// glTexStorage2D(to_value(object.target), levels, to_value(internalFormat), width, height);
	// glTexStorage3D(to_value(object.target), levels, to_value(internalFormat), width, height, depth);

	inline void storage(GLint levels, InternalFormat format, uint32_t width) noexcept {
		emulateStorage1D(levels, format.format, format.base, width);
	}
	inline void storage(GLint levels, InternalFormat format, uint32_t width, uint32_t height) noexcept {
		emulateStorage2D(levels, format.format, format.base, width, height);
	}
	inline void storage(GLint levels, InternalFormat format, uint32_t width, uint32_t height, uint32_t depth) noexcept {
		emulateStorage3D(levels, format.format, format.base, width, height, depth);
	}

public:
	inline void generateMipmap() noexcept {
		object.template assert_target<TextureTarget::_1D, TextureTarget::_2D, TextureTarget::_3D,
				TextureTarget::_1DArray, TextureTarget::_2DArray, TextureTarget::CubeMap, TextureTarget::CubeMapArray>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glGenerateMipmap(object.target);
		checkGL();
	}

private:
	inline void image1D(GLenum target, GLint level, GLenum format, BaseInternalFormat dataFormat, DataType dataType, uint32_t width, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexImage1D(target, level, format, width, 0, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}
	inline void image2D(GLenum target, GLint level, GLenum format, BaseInternalFormat dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexImage2D(target, level, format, width, height, 0, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}
	inline void image3D(GLenum target, GLint level, GLenum format, BaseInternalFormat dataFormat, DataType dataType, uint32_t width, uint32_t height, uint32_t depth, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexImage3D(target, level, format, width, height, depth, 0, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}

public:
	inline void image(GLint level, InternalFormat format, BaseInternalFormat dataFormat, DataType dataType, uint32_t width, const void* data) noexcept {
		object.template assert_target<TextureTarget::_1D>();
		image1D(to_value(object.target), level, to_value(format), dataFormat, dataType, width, data);
	}

	inline void image(GLint level, InternalFormat format, BaseInternalFormat dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle>();
		image2D(to_value(object.target), level, to_value(format), dataFormat, dataType, width, height, data);
	}

	inline void image(CubeSide cubeSide, GLint level, InternalFormat format, BaseInternalFormat dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data) noexcept {
		object.template assert_target<TextureTarget::CubeMap>();
		image2D(to_value(cubeSide), level, to_value(format), dataFormat, dataType, width, height, data);
	}

	inline void image(GLint level, InternalFormat format, BaseInternalFormat dataFormat, DataType dataType, uint32_t width, uint32_t height, uint32_t depth, const void* data) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D>();
		image3D(to_value(object.target), level, to_value(format), dataFormat, dataType, width, height, depth, data);
	}

	inline void image(CubeSide cubeSide, GLint level, InternalFormat format, BaseInternalFormat dataFormat, DataType dataType, uint32_t width, uint32_t height, uint32_t depth, const void* data) noexcept {
		object.template assert_target<TextureTarget::CubeMapArray>();
		depth = depth * 6 + to_value(cubeSide) - to_value(CubeSide::PositiveX);
		image3D(GL_TEXTURE_CUBE_MAP_ARRAY, level, to_value(format), dataFormat, dataType, width, height, depth, data);
	}

private:
	inline void subImage1D(GLenum target, GLint level, int32_t xOffset, uint32_t width, BaseInternalFormat dataFormat, DataType dataType, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexSubImage1D(target, level, xOffset, width, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}
	inline void subImage2D(GLenum target, GLint level, int32_t xOffset, int32_t yOffset, uint32_t width, uint32_t height, BaseInternalFormat dataFormat, DataType dataType, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexSubImage2D(target, level, xOffset, yOffset, width, height, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}
	inline void subImage3D(GLenum target, GLint level, int32_t xOffset, int32_t yOffset, int32_t zOffset, uint32_t width, uint32_t height, uint32_t depth, BaseInternalFormat dataFormat, DataType dataType, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexSubImage3D(target, level, xOffset, yOffset, zOffset, width, height, depth, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}

public:
	inline void subImage(GLint level, int32_t xOffset, uint32_t width, BaseInternalFormat dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::_1D>();
		subImage1D(to_value(object.target), level, xOffset, width, dataFormat, dataType, data);
	}
	inline void subImage(GLint level, int32_t xOffset, int32_t yOffset, uint32_t width, uint32_t height, BaseInternalFormat dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle>();
		subImage2D(to_value(object.target), level, xOffset, yOffset, width, height, dataFormat, dataType, data);
	}
	inline void subImage(CubeSide cubeSide, GLint level, int32_t xOffset, int32_t yOffset, uint32_t width, uint32_t height, BaseInternalFormat dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::CubeMap>();
		subImage2D(to_value(cubeSide), level, xOffset, yOffset, width, height, dataFormat, dataType, data);
	}
	inline void subImage(GLint level, int32_t xOffset, int32_t yOffset, int32_t zOffset, uint32_t width, uint32_t height, uint32_t depth, BaseInternalFormat dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D>();
		subImage3D(to_value(object.target), level, xOffset, yOffset, zOffset, width, height, depth, dataFormat, dataType, data);
	}
	inline void subImage(CubeSide cubeSide, GLint level, int32_t xOffset, int32_t yOffset, int32_t zOffset, uint32_t width, uint32_t height, uint32_t depth, BaseInternalFormat dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::CubeMapArray>();
		depth = depth * 6 + to_value(cubeSide) - to_value(CubeSide::PositiveX);
		subImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, level, xOffset, yOffset, zOffset, width, height, depth, dataFormat, dataType, data);
	}

public:
	inline int32_t getSize1D(GLint level = 0) noexcept {
		object.template assert_target<TextureTarget::_1D>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		int32_t result;
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_WIDTH, &result);
		checkGL();
		return result;
	}
	inline libv::vec2i getSize2D(GLint level = 0) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle, TextureTarget::CubeMap>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		libv::vec2i result;
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_WIDTH, &result.x);
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_HEIGHT, &result.y);
		checkGL();
		return result;
	}
	inline libv::vec3i getSize3D(GLint level = 0) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		libv::vec3i result;
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_WIDTH, &result.x);
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_HEIGHT, &result.y);
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_DEPTH, &result.z);
		checkGL();
		return result;
	}

public:
	inline void setDepthStencilMode(DepthStencilMode mode) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_DEPTH_STENCIL_TEXTURE_MODE, to_value(mode));
		checkGL();
	}
	inline void setBaseLevel(GLint level) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_BASE_LEVEL, level);
		checkGL();
	}
	inline void setMaxLevel(GLint level) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_MAX_LEVEL, level);
		checkGL();
	}
	inline void setBorderColor(float r, float g, float b, float a) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		float value[]{r, g, b, a};
		glTexParameterfv(to_value(object.target), GL_TEXTURE_BORDER_COLOR, value);
		checkGL();
	}
	inline void setBorderColor(GLint r, GLint g, GLint b, GLint a) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		GLint value[]{r, g, b, a};
		glTexParameteriv(to_value(object.target), GL_TEXTURE_BORDER_COLOR, value);
		checkGL();
	}
	inline void setCompareMode(CompareMode mode) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_COMPARE_MODE, to_value(mode));
		checkGL();
	}
	inline void setCompareFunc(TestFunction func) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_COMPARE_FUNC, to_value(func));
		checkGL();
	}
	inline void setLODBias(float bias) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameterf(to_value(object.target), GL_TEXTURE_LOD_BIAS, bias);
		checkGL();
	}
	inline void setMagFilter(MagFilter filter) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_MAG_FILTER, to_value(filter));
		checkGL();
	}
	inline void setMaxLOD(float value) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameterf(to_value(object.target), GL_TEXTURE_MAX_LOD, value);
		checkGL();
	}
	inline void setMinFilter(MinFilter filter) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_MIN_FILTER, to_value(filter));
		checkGL();
	}
	inline void setMinLOD(float value) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameterf(to_value(object.target), GL_TEXTURE_MIN_LOD, value);
		checkGL();
	}
	inline void setSwizzle(GLint r, GLint g, GLint b, GLint a) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		GLint value[]{r, g, b, a};
		glTexParameteriv(to_value(object.target), GL_TEXTURE_SWIZZLE_RGBA, value);
		checkGL();
	}
	inline void setSwizzle(Swizzle r, Swizzle g, Swizzle b, Swizzle a) noexcept {
		setSwizzle(to_value(r), to_value(g), to_value(b), to_value(a));
	}
	inline void setSwizzleR(GLint swizzle) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_SWIZZLE_R, swizzle);
		checkGL();
	}
	inline void setSwizzleG(GLint swizzle) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_SWIZZLE_G, swizzle);
		checkGL();
	}
	inline void setSwizzleB(GLint swizzle) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_SWIZZLE_B, swizzle);
		checkGL();
	}
	inline void setSwizzleA(GLint swizzle) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_SWIZZLE_A, swizzle);
		checkGL();
	}
	inline void setSwizzleR(Swizzle swizzle) noexcept {
		setSwizzleR(to_value(swizzle));
	}
	inline void setSwizzleG(Swizzle swizzle) noexcept {
		setSwizzleG(to_value(swizzle));
	}
	inline void setSwizzleB(Swizzle swizzle) noexcept {
		setSwizzleB(to_value(swizzle));
	}
	inline void setSwizzleA(Swizzle swizzle) noexcept {
		setSwizzleA(to_value(swizzle));
	}
	inline void setWrap(Wrap s) noexcept {
		object.template assert_target<TextureTarget::_1D>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_WRAP_S, to_value(s));
		checkGL();
	}
	inline void setWrap(Wrap s, Wrap t) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle, TextureTarget::CubeMap>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_WRAP_S, to_value(s));
		glTexParameteri(to_value(object.target), GL_TEXTURE_WRAP_T, to_value(t));
		checkGL();
	}
	inline void setWrap(Wrap s, Wrap t, Wrap r) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_WRAP_S, to_value(s));
		glTexParameteri(to_value(object.target), GL_TEXTURE_WRAP_T, to_value(t));
		glTexParameteri(to_value(object.target), GL_TEXTURE_WRAP_R, to_value(r));
		checkGL();
	}
	inline void setWrapS(Wrap wrap) noexcept {
		object.template assert_target<TextureTarget::_1D, TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle,
				TextureTarget::CubeMap, TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_WRAP_S, to_value(wrap));
		checkGL();
	}
	inline void setWrapT(Wrap wrap) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle,
				TextureTarget::CubeMap, TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_WRAP_T, to_value(wrap));
		checkGL();
	}
	inline void setWrapR(Wrap wrap) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_WRAP_R, to_value(wrap));
		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
