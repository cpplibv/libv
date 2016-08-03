// File: texture.hpp, Created on 2014. november 28. 17:44, Author: Vader

#pragma once

// std
#include <memory>
// pro
#include <libv/gl/enum.hpp>

namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

class BaseTexture {
protected:
	GLuint textureID = 0;
	GLenum bindTarget = 0;

protected:
	inline BaseTexture() = default;
	inline BaseTexture(const BaseTexture&) = delete;
	inline BaseTexture(BaseTexture&&) = delete;
	inline ~BaseTexture() = default;

public:
	inline void create();
	inline void destroy();

	inline void adopt(GLenum target, GLuint id);
	inline void adopt(TextureBindTarget target, GLuint id);

	inline void bind(GLenum target);
	inline void bind(TextureBindTarget target);
	inline void unbind();

	inline void storage1D(size_t levels, InternalFormat internalFormat, uint32_t width);
	inline void storage2D(size_t levels, InternalFormat internalFormat, uint32_t width, uint32_t height);
	inline void storage3D(size_t levels, InternalFormat internalFormat, uint32_t width, uint32_t height, uint32_t depth);

	inline void image1D(size_t level, InternalFormat internalFormat, Format dataFormat, DataType dataType, uint32_t width, const void* data);
	inline void image2D(size_t level, InternalFormat internalFormat, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data);
	inline void image3D(size_t level, InternalFormat internalFormat, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, uint32_t depth, const void* data);

	inline void subImage2D(CubeSide target, size_t level, int32_t xOffset, int32_t yOffset, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data);
	inline void subImage1D(size_t level, int32_t xOffset, Format dataFormat, DataType dataType, uint32_t width, const void* data);
	inline void subImage2D(size_t level, int32_t xOffset, int32_t yOffset, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data);
	inline void subImage3D(size_t level, int32_t xOffset, int32_t yOffset, int32_t zOffset, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, uint32_t depth, const void* data);

	//inline void generateMipmap()?

	inline void setDepthStencilMode(DepthStencilMode mode) {
		glTexParameteri(bindTarget, GL_DEPTH_STENCIL_TEXTURE_MODE, to_value(mode));
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setBaseLevel(size_t level) {
		glTexParameteri(bindTarget, GL_TEXTURE_BASE_LEVEL, level);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setMaxLevel(size_t level) {
		glTexParameteri(bindTarget, GL_TEXTURE_MAX_LEVEL, level);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setBorderColor(float r, float g, float b, float a) {
		float value[]{r, g, b, a};
		glTexParameterfv(bindTarget, GL_TEXTURE_BORDER_COLOR, value);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setBorderColor(GLint r, GLint g, GLint b, GLint a) {
		GLint value[]{r, g, b, a};
		glTexParameteriv(bindTarget, GL_TEXTURE_BORDER_COLOR, value);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setCompareMode(CompareMode mode) {
		glTexParameteri(bindTarget, GL_TEXTURE_COMPARE_MODE, to_value(mode));
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setCompareFunc(TestFunction func) {
		glTexParameteri(bindTarget, GL_TEXTURE_COMPARE_FUNC, to_value(func));
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setLODBias(float bias) {
		glTexParameterf(bindTarget, GL_TEXTURE_LOD_BIAS, bias);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setMagFilter(MagFilter filter) {
		glTexParameteri(bindTarget, GL_TEXTURE_MAG_FILTER, to_value(filter));
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setMaxLOD(float value) {
		glTexParameterf(bindTarget, GL_TEXTURE_MAX_LOD, value);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setMinFilter(MinFilter filter) {
		glTexParameteri(bindTarget, GL_TEXTURE_MIN_FILTER, to_value(filter));
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setMinLOD(float value) {
		glTexParameterf(bindTarget, GL_TEXTURE_MIN_LOD, value);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setSwizzle(GLint r, GLint g, GLint b, GLint a) {
		GLint value[]{r, g, b, a};
		glTexParameteriv(bindTarget, GL_TEXTURE_SWIZZLE_RGBA, value);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setSwizzle(Swizzle r, Swizzle g, Swizzle b, Swizzle a) {
		setSwizzle(to_value(r), to_value(g), to_value(b), to_value(a));
	}
	inline void setSwizzleR(GLint swizzle) {
		glTexParameteri(bindTarget, GL_TEXTURE_SWIZZLE_R, swizzle);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setSwizzleG(GLint swizzle) {
		glTexParameteri(bindTarget, GL_TEXTURE_SWIZZLE_G, swizzle);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setSwizzleB(GLint swizzle) {
		glTexParameteri(bindTarget, GL_TEXTURE_SWIZZLE_B, swizzle);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setSwizzleA(GLint swizzle) {
		glTexParameteri(bindTarget, GL_TEXTURE_SWIZZLE_A, swizzle);
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setSwizzleR(Swizzle swizzle) {
		setSwizzleR(to_value(swizzle));
	}
	inline void setSwizzleG(Swizzle swizzle) {
		setSwizzleG(to_value(swizzle));
	}
	inline void setSwizzleB(Swizzle swizzle) {
		setSwizzleB(to_value(swizzle));
	}
	inline void setSwizzleA(Swizzle swizzle) {
		setSwizzleA(to_value(swizzle));
	}
	inline void setWrap(Wrap s, Wrap t) {
		glTexParameteri(bindTarget, GL_TEXTURE_WRAP_S, to_value(s));
		glTexParameteri(bindTarget, GL_TEXTURE_WRAP_T, to_value(t));
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setWrap(Wrap s, Wrap t, Wrap r) {
		glTexParameteri(bindTarget, GL_TEXTURE_WRAP_S, to_value(s));
		glTexParameteri(bindTarget, GL_TEXTURE_WRAP_T, to_value(t));
		glTexParameteri(bindTarget, GL_TEXTURE_WRAP_R, to_value(r));
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setWrapS(Wrap wrap) {
		glTexParameteri(bindTarget, GL_TEXTURE_WRAP_S, to_value(wrap));
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setWrapT(Wrap wrap) {
		glTexParameteri(bindTarget, GL_TEXTURE_WRAP_T, to_value(wrap));
		LIBV_GL_DEBUG_CHECK_GL();
	}
	inline void setWrapR(Wrap wrap) {
		glTexParameteri(bindTarget, GL_TEXTURE_WRAP_R, to_value(wrap));
		LIBV_GL_DEBUG_CHECK_GL();
	}

public:
	inline auto id() const {
		return textureID;
	}
	inline operator GLuint() const {
		return textureID;
	}
};
inline void BaseTexture::create() {
	LIBV_GL_DEBUG_ASSERT(textureID == 0);
	glGenTextures(1, &textureID);
	LIBV_GL_DEBUG_CHECK_GL();
	if (textureID == 0)
		LIBV_LOG_GL_ERROR("Failed to create texture.");
}
inline void BaseTexture::destroy() {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glDeleteTextures(1, &textureID);
	LIBV_GL_DEBUG_CHECK_GL();
	textureID = 0;
}
inline void BaseTexture::adopt(GLenum target, GLuint id) {
	LIBV_GL_DEBUG_ASSERT(textureID == 0);
	textureID = id;
	bindTarget = target;
}
inline void BaseTexture::adopt(TextureBindTarget target, GLuint id) {
	adopt(to_value(target), id);
}
inline void BaseTexture::bind(TextureBindTarget target) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	bindTarget = to_value(target);
	glBindTexture(bindTarget, textureID);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::bind(GLenum target) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	bindTarget = target;
	glBindTexture(bindTarget, textureID);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::unbind() {
	LIBV_GL_DEBUG_ASSERT(bindTarget != 0);
	glBindTexture(bindTarget, 0);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::storage1D(size_t levels, InternalFormat internalFormat, uint32_t width) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glTexStorage1D(bindTarget, levels, to_value(internalFormat), width);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::storage2D(size_t levels, InternalFormat internalFormat, uint32_t width, uint32_t height) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glTexStorage2D(bindTarget, levels, to_value(internalFormat), width, height);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::storage3D(size_t levels, InternalFormat internalFormat, uint32_t width, uint32_t height, uint32_t depth) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glTexStorage3D(bindTarget, levels, to_value(internalFormat), width, height, depth);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::image1D(size_t level, InternalFormat internalFormat, Format dataFormat, DataType dataType, uint32_t width, const void* data) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glTexImage1D(bindTarget, level, to_value(internalFormat), width, 0, to_value(dataFormat), to_value(dataType), data);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::image2D(size_t level, InternalFormat internalFormat, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glTexImage2D(bindTarget, level, to_value(internalFormat), width, height, 0, to_value(dataFormat), to_value(dataType), data);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::image3D(size_t level, InternalFormat internalFormat, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, uint32_t depth, const void* data) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glTexImage3D(bindTarget, level, to_value(internalFormat), width, height, depth, 0, to_value(dataFormat), to_value(dataType), data);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::subImage1D(size_t level, int32_t xOffset, Format dataFormat, DataType dataType, uint32_t width, const void* data) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glTexSubImage1D(bindTarget, level, xOffset, width, to_value(dataFormat), to_value(dataType), data);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::subImage2D(size_t level, int32_t xOffset, int32_t yOffset, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glTexSubImage2D(bindTarget, level, xOffset, yOffset, width, height, to_value(dataFormat), to_value(dataType), data);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::subImage2D(CubeSide cubeSide, size_t level, int32_t xOffset, int32_t yOffset, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glTexSubImage2D(to_value(cubeSide), level, xOffset, yOffset, width, height, to_value(dataFormat), to_value(dataType), data);
	LIBV_GL_DEBUG_CHECK_GL();
}
inline void BaseTexture::subImage3D(size_t level, int32_t xOffset, int32_t yOffset, int32_t zOffset, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, uint32_t depth, const void* data) {
	LIBV_GL_DEBUG_ASSERT(textureID != 0);
	glTexSubImage3D(bindTarget, level, xOffset, yOffset, zOffset, width, height, depth, to_value(dataFormat), to_value(dataType), data);
	LIBV_GL_DEBUG_CHECK_GL();
}

// TextureAC ----------------------------------------------------------------------------------------

struct TextureAC : public BaseTexture {
	inline TextureAC() {
		create();
	}
	inline TextureAC(const TextureAC&) = delete;
	inline TextureAC(TextureAC&& other) {
		std::swap(textureID, other.textureID);
		std::swap(bindTarget, other.bindTarget);
	}
	inline ~TextureAC() {
		if (id())
			destroy();
	}
	static TextureAC createFromDDS(const char* data, const size_t size);
};

using Texture = TextureAC;

// TextureNC ----------------------------------------------------------------------------------------

struct TextureNC : public BaseTexture {
	inline TextureNC() = default;
	inline TextureNC(const TextureNC&) = delete;
	inline TextureNC(TextureNC&& other) {
		std::swap(textureID, other.textureID);
		std::swap(bindTarget, other.bindTarget);
	}
	inline ~TextureNC() {
		LIBV_GL_DEBUG_ASSERT(id() == 0);
	}
	static TextureNC createFromDDS(const char* data, const size_t size);
};

// Sampler -----------------------------------------------------------------------------------------

struct Sampler {
	// TODO P4: Implement GL Texture samplers

	// void GenSamplers(sizei count, uint *samplers);
	// void DeleteSamplers(sizei count, const uint * samplers);
	// boolean IsSampler(uint sampler);
	// void BindSampler(uint unit, uint sampler);
	// void SamplerParameteri(uint sampler, enum pname, int param);
	// void SamplerParameterf(uint sampler, enum pname, float param);
	// void SamplerParameteriv(uint sampler, enum pname, const int *params);
	// void SamplerParameterfv(uint sampler, enum pname, const float *params);
	// void SamplerParameterIiv(uint sampler, enum pname, const int *params);
	// void SamplerParameterIuiv(uint sampler, enum pname, const uint *params);
	// void GetSamplerParameteriv(uint sampler, enum pname, int *params);
	// void GetSamplerParameterfv(uint sampler, enum pname, float *params);
	// void GetSamplerParameterIiv(uint sampler, enum pname, int *params);
	// void GetSamplerParameterIuiv(uint sampler, enum pname, uint *params);

	// void setWrapping(Wrapping s);
	// void setWrapping(Wrapping s, Wrapping t);
	// void setWrapping(Wrapping s, Wrapping t, Wrapping r);
	// void setFilters(Filter min, Filter mag);
	// void setBorderColor(const Color& color);
};

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace libv


// =================================================================================================

// Emulating glTexStorage*D for pre 4.2 compatibility:
//  Failed iteration on implementation due to compatableDefaultType.
//  Failed iteration on implementation due to compatableDefaultFormat.
//
//
//inline void BaseTexture::emulateStorage1D(TextureBindTarget target, InternalFormat internalFormat, size_t levels, uint32_t width) {
//	glGenTextures(1, &textureID);
//	if (textureID == 0)
//		LIBV_LOG_GL_ERROR("Failed to create 1D texture", to_string(type));
//
//	// Simulating glTexStorage1D for pre 4.2 compatibility
//	DataType type = compatableDefaultType(target);
//	Format format = compatableDefaultFormat(target);
//	glBindTexture(bindTarget, textureID);
//
//	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
//	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, levels - 1);
//
//	for (size_t i = 0; i < levels; i++) {
//		glTexImage1D(target, i, internalFormat, width, 0, format, type, nullptr);
//		width = std::max(1, (width / 2));
//	}
//	LIBV_GL_DEBUG_CHECK_GL();
//}
//
//inline void BaseTexture::emulateStorage2D(TextureBindTarget target, InternalFormat internalFormat, size_t levels, uint32_t width, uint32_t height) {
//	glGenTextures(1, &textureID);
//	if (textureID == 0)
//		LIBV_LOG_GL_ERROR("Failed to create 2D texture", to_string(type));
//
//	// Simulating glTexStorage2D for pre 4.2 compatibility
//	DataType type = compatableDefaultType(target);
//	Format format = compatableDefaultFormat(target);
//	glBindTexture(bindTarget, textureID);
//
//	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
//	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, levels - 1);
//
//	if (target == TextureBindTarget::CubeMap) {
//		for (size_t i = 0; i < levels; i++) {
//			glTexImage2D(to_value(TextureTarget2D::CubeMapPositiveX), i, internalFormat, width, height, 0, format, type, nullptr);
//			glTexImage2D(to_value(TextureTarget2D::CubeMapNegativeX), i, internalFormat, width, height, 0, format, type, nullptr);
//			glTexImage2D(to_value(TextureTarget2D::CubeMapPositiveY), i, internalFormat, width, height, 0, format, type, nullptr);
//			glTexImage2D(to_value(TextureTarget2D::CubeMapNegativeY), i, internalFormat, width, height, 0, format, type, nullptr);
//			glTexImage2D(to_value(TextureTarget2D::CubeMapPositiveZ), i, internalFormat, width, height, 0, format, type, nullptr);
//			glTexImage2D(to_value(TextureTarget2D::CubeMapNegativeZ), i, internalFormat, width, height, 0, format, type, nullptr);
//
//			width = std::max(1, (width / 2));
//			height = std::max(1, (height / 2));
//		}
//	} else if (target == TextureBindTarget::_1DArray) {
//		for (size_t i = 0; i < levels; i++) {
//			glTexImage2D(bindTarget, i, internalFormat, width, height, 0, format, type, nullptr);
//			width = std::max(1, (width / 2));
//		}
//	} else {
//		for (size_t i = 0; i < levels; i++) {
//			glTexImage2D(bindTarget, i, internalFormat, width, height, 0, format, type, nullptr);
//			width = std::max(1, (width / 2));
//			height = std::max(1, (height / 2));
//		}
//	}
//	LIBV_GL_DEBUG_CHECK_GL();
//}
//inline void BaseTexture::emulateStorage3D(TextureBindTarget target, InternalFormat internalFormat, size_t levels, uint32_t width, uint32_t height, uint32_t depth) {
//	glGenTextures(3, &textureID);
//	if (textureID == 0)
//		LIBV_LOG_GL_ERROR("Failed to create 2D texture", to_string(type));
//
//	// Simulating glTexStorage3D for pre 4.2 compatibility
//	DataType type = compatableDefaultType(target);
//	Format format = compatableDefaultFormat(target);
//	glBindTexture(bindTarget, textureID);
//
//	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
//	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, levels - 1);
//
//	if (target == TextureBindTarget::_3D) {
//		for (size_t i = 0; i < levels; i++) {
//			glTexImage3D(target, i, internalFormat, width, height, depth, 0, format, type, nullptr);
//			width = std::max(1, (width / 2));
//			height = std::max(1, (height / 2));
//			depth = std::max(1, (depth / 2));
//		}
//	} else {
//		for (size_t i = 0; i < levels; i++) {
//			glTexImage3D(target, i, internalFormat, width, height, depth, 0, format, type, nullptr);
//			width = std::max(1, (width / 2));
//			height = std::max(1, (height / 2));
//		}
//	}
//	LIBV_GL_DEBUG_CHECK_GL();
//}
