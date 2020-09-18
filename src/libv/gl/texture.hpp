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

private:
	inline void emulateStorage1D(int32_t levels, GLenum Format, FormatBase format, int32_t width) noexcept {
		object.template assert_target<TextureTarget::_1D>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(to_value(object.target), GL_TEXTURE_MAX_LEVEL, levels - 1);

		for (int32_t i = 0; i < levels; i++) {
			glTexImage1D(to_value(object.target), i, Format, width, 0, to_value(format), GL_BYTE, nullptr);
			width = std::max(1, width / 2);
		}
		checkGL();
	}

	inline void emulateStorage2D(int32_t levels, GLenum Format, FormatBase format, int32_t width, int32_t height) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::CubeMap, TextureTarget::Rectangle>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(to_value(object.target), GL_TEXTURE_MAX_LEVEL, levels - 1);

		if (object.target == TextureTarget::_2D || object.target == TextureTarget::Rectangle) {
			for (int32_t i = 0; i < levels; i++) {
				glTexImage2D(to_value(object.target), i, Format, width, height, 0, to_value(format), GL_BYTE, nullptr);
				width = std::max(1, width / 2);
				height = std::max(1, height / 2);
			}

		} else if (object.target == TextureTarget::CubeMap) {
			for (int32_t i = 0; i < levels; i++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, i, Format, width, height, 0, to_value(format), GL_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, i, Format, width, height, 0, to_value(format), GL_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, i, Format, width, height, 0, to_value(format), GL_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, i, Format, width, height, 0, to_value(format), GL_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, i, Format, width, height, 0, to_value(format), GL_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, i, Format, width, height, 0, to_value(format), GL_BYTE, nullptr);

				width = std::max(1, width / 2);
				height = std::max(1, height / 2);
			}

		} else if (object.target == TextureTarget::_1DArray) {
			for (int32_t i = 0; i < levels; i++) {
				glTexImage2D(to_value(object.target), i, Format, width, height, 0, to_value(format), GL_BYTE, nullptr);
				width = std::max(1, width / 2);
			}
		}
		checkGL();
	}

	inline void emulateStorage3D(int32_t levels, GLenum Format, FormatBase format, int32_t width, int32_t height, int32_t depth) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		glTexParameteri(to_value(object.target), GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(to_value(object.target), GL_TEXTURE_MAX_LEVEL, levels - 1);

		if (object.target == TextureTarget::_3D) {
			for (int32_t i = 0; i < levels; i++) {
				glTexImage3D(to_value(object.target), i, Format, width, height, depth, 0, to_value(format), GL_BYTE, nullptr);
				width = std::max(1, width / 2);
				height = std::max(1, height / 2);
				depth = std::max(1, depth / 2);
			}

		} else if (object.target == TextureTarget::_2DArray || object.target == TextureTarget::CubeMapArray) {
			for (int32_t i = 0; i < levels; i++) {
				glTexImage3D(to_value(object.target), i, Format, width, height, depth, 0, to_value(format), GL_BYTE, nullptr);
				width = std::max(1, width / 2);
				height = std::max(1, height / 2);
			}
		}

		checkGL();
	}

public:
	inline void storage(int32_t levels, Format format, int32_t width) noexcept {
		emulateStorage1D(levels, format.format, format.base, width);
	}
	inline void storage(int32_t levels, Format format, int32_t width, int32_t height) noexcept {
		emulateStorage2D(levels, format.format, format.base, width, height);
	}
	inline void storage(int32_t levels, Format format, int32_t width, int32_t height, int32_t depth) noexcept {
		emulateStorage3D(levels, format.format, format.base, width, height, depth);
	}

	inline void storage_ms(Format format, libv::vec2i size, int32_t samples, bool fixedSamples) noexcept {
		object.template assert_target<TextureTarget::_2DMultisample>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexImage2DMultisample(to_value(object.target), samples, format.format, size.x, size.y, fixedSamples);
		checkGL();
	}
	inline void storage_ms(Format format, libv::vec3i size, int32_t samples, bool fixedSamples) noexcept {
		object.template assert_target<TextureTarget::_2DMultisampleArray>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexImage3DMultisample(to_value(object.target), samples, format.format, size.x, size.y, size.z, fixedSamples);
		checkGL();
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
	inline void image1D(GLenum target, int32_t level, GLenum format, int32_t width, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexImage1D(target, level, format, width, 0, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}
	inline void image2D(GLenum target, int32_t level, GLenum format, int32_t width, int32_t height, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexImage2D(target, level, format, width, height, 0, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}
	inline void image3D(GLenum target, int32_t level, GLenum format, int32_t width, int32_t height, int32_t depth, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexImage3D(target, level, format, width, height, depth, 0, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}

public:
	inline void image(int32_t level, Format format, int32_t width, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::_1D>();
		image1D(to_value(object.target), level, format.format, width, dataFormat, dataType, data);
	}

	inline void image(int32_t level, Format format, int32_t width, int32_t height, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle>();
		image2D(to_value(object.target), level, format.format, width, height, dataFormat, dataType, data);
	}

	inline void image(CubeSide cubeSide, int32_t level, Format format, int32_t width, int32_t height, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::CubeMap>();
		image2D(to_value(cubeSide), level, format.format, width, height, dataFormat, dataType, data);
	}

	inline void image(int32_t level, Format format, int32_t width, int32_t height, int32_t depth, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D>();
		image3D(to_value(object.target), level, format.format, width, height, depth, dataFormat, dataType, data);
	}

	inline void image(CubeSide cubeSide, int32_t level, Format format, int32_t width, int32_t height, int32_t depth, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::CubeMapArray>();
		depth = depth * 6 + to_value(cubeSide) - to_value(CubeSide::PositiveX);
		image3D(GL_TEXTURE_CUBE_MAP_ARRAY, level, format.format, width, height, depth, dataFormat, dataType, data);
	}

private:
	inline void subImage1D(GLenum target, int32_t level, int32_t xOffset, int32_t width, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexSubImage1D(target, level, xOffset, width, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}
	inline void subImage2D(GLenum target, int32_t level, int32_t xOffset, int32_t yOffset, int32_t width, int32_t height, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexSubImage2D(target, level, xOffset, yOffset, width, height, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}
	inline void subImage3D(GLenum target, int32_t level, int32_t xOffset, int32_t yOffset, int32_t zOffset, int32_t width, int32_t height, int32_t depth, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexSubImage3D(target, level, xOffset, yOffset, zOffset, width, height, depth, to_value(dataFormat), to_value(dataType), data);
		checkGL();
	}

public:
	inline void subImage(int32_t level, int32_t xOffset, int32_t width, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::_1D>();
		subImage1D(to_value(object.target), level, xOffset, width, dataFormat, dataType, data);
	}
	inline void subImage(int32_t level, int32_t xOffset, int32_t yOffset, int32_t width, int32_t height, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle>();
		subImage2D(to_value(object.target), level, xOffset, yOffset, width, height, dataFormat, dataType, data);
	}
	inline void subImage(CubeSide cubeSide, int32_t level, int32_t xOffset, int32_t yOffset, int32_t width, int32_t height, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::CubeMap>();
		subImage2D(to_value(cubeSide), level, xOffset, yOffset, width, height, dataFormat, dataType, data);
	}
	inline void subImage(int32_t level, int32_t xOffset, int32_t yOffset, int32_t zOffset, int32_t width, int32_t height, int32_t depth, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D>();
		subImage3D(to_value(object.target), level, xOffset, yOffset, zOffset, width, height, depth, dataFormat, dataType, data);
	}
	inline void subImage(CubeSide cubeSide, int32_t level, int32_t xOffset, int32_t yOffset, int32_t zOffset, int32_t width, int32_t height, int32_t depth, FormatBase dataFormat, DataType dataType, const void* data) noexcept {
		object.template assert_target<TextureTarget::CubeMapArray>();
		depth = depth * 6 + to_value(cubeSide) - to_value(CubeSide::PositiveX);
		subImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, level, xOffset, yOffset, zOffset, width, height, depth, dataFormat, dataType, data);
	}

public:
	inline int32_t getSize1D(int32_t level = 0) noexcept {
		object.template assert_target<TextureTarget::_1D>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		int32_t result;
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_WIDTH, &result);
		checkGL();
		return result;
	}
	inline libv::vec2i getSize2D(int32_t level = 0) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle, TextureTarget::CubeMap>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		libv::vec2i result;
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_WIDTH, &result.x);
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_HEIGHT, &result.y);
		checkGL();
		return result;
	}
	inline libv::vec3i getSize3D(int32_t level = 0) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();
		LIBV_GL_DEBUG_ASSERT(object.id != 0);

		libv::vec3i result;
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_WIDTH, &result.x);
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_HEIGHT, &result.y);
		glGetTexLevelParameteriv(to_value(object.target), level, GL_TEXTURE_DEPTH, &result.z);
		checkGL();
		return result;
	}

private:
	inline void set(GLenum parameter, GLfloat value) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexParameterf(to_value(object.target), parameter, value);
		checkGL();
	}
	inline void set(GLenum parameter, int32_t value) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexParameteri(to_value(object.target), parameter, value);
		checkGL();
	}
	inline void set(GLenum parameter, libv::vec4f value) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexParameterfv(to_value(object.target), parameter, value.ptr());
		checkGL();
	}
	inline void set(GLenum parameter, libv::vec4i value) noexcept {
		LIBV_GL_DEBUG_ASSERT(object.id != 0);
		glTexParameteriv(to_value(object.target), parameter, value.ptr());
		checkGL();
	}

public:
	inline void setDepthStencilMode(DepthStencilMode mode) noexcept {
		set(GL_DEPTH_STENCIL_TEXTURE_MODE, to_value(mode));
	}
	inline void setBaseLevel(int32_t level) noexcept {
		set(GL_TEXTURE_BASE_LEVEL, level);
	}
	inline void setMaxLevel(int32_t level) noexcept {
		set(GL_TEXTURE_MAX_LEVEL, level);
	}
	inline void setBorderColor(float r, float g, float b, float a) noexcept {
		set(GL_TEXTURE_BORDER_COLOR, {r, g, b, a});
	}
	inline void setBorderColor(int32_t r, int32_t g, int32_t b, int32_t a) noexcept {
		set(GL_TEXTURE_BORDER_COLOR, {r, g, b, a});
	}
	inline void setCompareMode(CompareMode mode) noexcept {
		set(GL_TEXTURE_COMPARE_MODE, to_value(mode));
	}
	inline void setCompareFunc(TestFunction func) noexcept {
		set(GL_TEXTURE_COMPARE_FUNC, to_value(func));
	}
	inline void setLODBias(float bias) noexcept {
		set(GL_TEXTURE_LOD_BIAS, bias);
	}
	inline void setMagFilter(MagFilter filter) noexcept {
		object.template assert_not_target<TextureTarget::_2DMultisample, TextureTarget::_2DMultisampleArray>();
		set(GL_TEXTURE_MAG_FILTER, static_cast<int32_t>(to_value(filter)));
	}
	inline void setMaxLOD(float value) noexcept {
		set(GL_TEXTURE_MAX_LOD, value);
	}
	inline void setMinFilter(MinFilter filter) noexcept {
		object.template assert_not_target<TextureTarget::_2DMultisample, TextureTarget::_2DMultisampleArray>();
		set(GL_TEXTURE_MIN_FILTER, static_cast<int32_t>(to_value(filter)));
	}
	inline void setMinLOD(float value) noexcept {
		set(GL_TEXTURE_MIN_LOD, value);
	}
	inline void setSwizzle(Swizzle r, Swizzle g, Swizzle b, Swizzle a) noexcept {
		set(GL_TEXTURE_SWIZZLE_RGBA, {to_value(r), to_value(g), to_value(b), to_value(a)});
	}
	inline void setSwizzleR(Swizzle swizzle) noexcept {
		set(GL_TEXTURE_SWIZZLE_R, swizzle);
	}
	inline void setSwizzleG(Swizzle swizzle) noexcept {
		set(GL_TEXTURE_SWIZZLE_G, swizzle);
	}
	inline void setSwizzleB(Swizzle swizzle) noexcept {
		set(GL_TEXTURE_SWIZZLE_B, swizzle);
	}
	inline void setSwizzleA(Swizzle swizzle) noexcept {
		set(GL_TEXTURE_SWIZZLE_A, swizzle);
	}

	inline void setWrap(Wrap s) noexcept {
		object.template assert_target<TextureTarget::_1D>();

		set(GL_TEXTURE_WRAP_S, static_cast<int32_t>(to_value(s)));
	}
	inline void setWrap(Wrap s, Wrap t) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle, TextureTarget::CubeMap>();

		set(GL_TEXTURE_WRAP_S, static_cast<int32_t>(to_value(s)));
		set(GL_TEXTURE_WRAP_T, static_cast<int32_t>(to_value(t)));
	}
	inline void setWrap(Wrap s, Wrap t, Wrap r) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();

		set(GL_TEXTURE_WRAP_S, static_cast<int32_t>(to_value(s)));
		set(GL_TEXTURE_WRAP_T, static_cast<int32_t>(to_value(t)));
		set(GL_TEXTURE_WRAP_R, static_cast<int32_t>(to_value(r)));
	}
	inline void setWrapS(Wrap wrap) noexcept {
		object.template assert_target<TextureTarget::_1D, TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle,
				TextureTarget::CubeMap, TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();

		set(GL_TEXTURE_WRAP_S, static_cast<int32_t>(to_value(wrap)));
	}
	inline void setWrapT(Wrap wrap) noexcept {
		object.template assert_target<TextureTarget::_1DArray, TextureTarget::_2D, TextureTarget::Rectangle,
				TextureTarget::CubeMap, TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();

		set(GL_TEXTURE_WRAP_T, static_cast<int32_t>(to_value(wrap)));
	}
	inline void setWrapR(Wrap wrap) noexcept {
		object.template assert_target<TextureTarget::_2DArray, TextureTarget::_3D, TextureTarget::CubeMapArray>();

		set(GL_TEXTURE_WRAP_R, static_cast<int32_t>(to_value(wrap)));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
