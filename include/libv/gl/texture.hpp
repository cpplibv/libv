// File: texture.hpp, Created on 2014. november 28. 17:44, Author: Vader

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <memory>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/log.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

template <typename Target>
class BindGuard {
	Target& target;
public:
	inline BindGuard(Target& target) : target(target) {
		target.bind();
	}
	inline ~BindGuard() {
		target.unbind();
	}
};

// -------------------------------------------------------------------------------------------------

template <TextureBindTarget BindTarget>
class Texture {
	static constexpr GLuint invalidID = 0;

protected:
	GLuint textureID = invalidID;

public:
	inline Texture() = default;
	inline Texture(const Texture&) = delete;
	inline Texture(Texture&& orig) noexcept : textureID(orig.textureID) {
		orig.textureID = invalidID;
	}
	inline ~Texture() {
		LIBV_GL_DEBUG_ASSERT(id() == invalidID);
	}

public:
	//inline void generateMipmap()?

	inline void create() {
		LIBV_GL_DEBUG_ASSERT(textureID == invalidID);
		glGenTextures(1, &textureID);
		LIBV_GL_DEBUG_CHECK();
		if (textureID == invalidID)
			LIBV_LOG_GL_ERROR("Failed to create texture.");
	}
	inline void destroy() {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glDeleteTextures(1, &textureID);
		LIBV_GL_DEBUG_CHECK();
		textureID = invalidID;
	}
	inline void adopt(GLuint id) {
		LIBV_GL_DEBUG_ASSERT(textureID == invalidID);
		textureID = id;
	}
	inline void bind() {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glBindTexture(to_value(BindTarget), textureID);
		LIBV_GL_DEBUG_CHECK();
	}
	inline BindGuard<Texture<BindTarget>> bindGuard() {
		return BindGuard<Texture<BindTarget>>(*this);
	}
	inline void unbind() {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glBindTexture(to_value(BindTarget), invalidID);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void storage1D(size_t levels, InternalFormat internalFormat, uint32_t width) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glTexStorage1D(to_value(BindTarget), levels, to_value(internalFormat), width);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void storage2D(size_t levels, InternalFormat internalFormat, uint32_t width, uint32_t height) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glTexStorage2D(to_value(BindTarget), levels, to_value(internalFormat), width, height);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void storage3D(size_t levels, InternalFormat internalFormat, uint32_t width, uint32_t height, uint32_t depth) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glTexStorage3D(to_value(BindTarget), levels, to_value(internalFormat), width, height, depth);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void image1D(size_t level, InternalFormat internalFormat, Format dataFormat, DataType dataType, uint32_t width, const void* data) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glTexImage1D(to_value(BindTarget), level, to_value(internalFormat), width, 0, to_value(dataFormat), to_value(dataType), data);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void image2D(size_t level, InternalFormat internalFormat, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glTexImage2D(to_value(BindTarget), level, to_value(internalFormat), width, height, 0, to_value(dataFormat), to_value(dataType), data);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void image3D(size_t level, InternalFormat internalFormat, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, uint32_t depth, const void* data) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glTexImage3D(to_value(BindTarget), level, to_value(internalFormat), width, height, depth, 0, to_value(dataFormat), to_value(dataType), data);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void subImage1D(size_t level, int32_t xOffset, Format dataFormat, DataType dataType, uint32_t width, const void* data) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glTexSubImage1D(to_value(BindTarget), level, xOffset, width, to_value(dataFormat), to_value(dataType), data);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void subImage2D(size_t level, int32_t xOffset, int32_t yOffset, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glTexSubImage2D(to_value(BindTarget), level, xOffset, yOffset, width, height, to_value(dataFormat), to_value(dataType), data);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void subImage2D(CubeSide cubeSide, size_t level, int32_t xOffset, int32_t yOffset, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, const void* data) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glTexSubImage2D(to_value(cubeSide), level, xOffset, yOffset, width, height, to_value(dataFormat), to_value(dataType), data);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void subImage3D(size_t level, int32_t xOffset, int32_t yOffset, int32_t zOffset, Format dataFormat, DataType dataType, uint32_t width, uint32_t height, uint32_t depth, const void* data) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		glTexSubImage3D(to_value(BindTarget), level, xOffset, yOffset, zOffset, width, height, depth, to_value(dataFormat), to_value(dataType), data);
		LIBV_GL_DEBUG_CHECK();
	}

public:
	inline libv::vec2i getSize2D(size_t level = 0) {
		LIBV_GL_DEBUG_ASSERT(textureID != invalidID);
		libv::vec2i result;
		glGetTexLevelParameteriv(to_value(BindTarget), level, GL_TEXTURE_WIDTH, &result.x);
		glGetTexLevelParameteriv(to_value(BindTarget), level, GL_TEXTURE_HEIGHT, &result.y);
		LIBV_GL_DEBUG_CHECK();
		return result;
	}

public:
	inline void setDepthStencilMode(DepthStencilMode mode) {
		glTexParameteri(to_value(BindTarget), GL_DEPTH_STENCIL_TEXTURE_MODE, to_value(mode));
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setBaseLevel(size_t level) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_BASE_LEVEL, level);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setMaxLevel(size_t level) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_MAX_LEVEL, level);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setBorderColor(float r, float g, float b, float a) {
		float value[]{r, g, b, a};
		glTexParameterfv(to_value(BindTarget), GL_TEXTURE_BORDER_COLOR, value);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setBorderColor(GLint r, GLint g, GLint b, GLint a) {
		GLint value[]{r, g, b, a};
		glTexParameteriv(to_value(BindTarget), GL_TEXTURE_BORDER_COLOR, value);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setCompareMode(CompareMode mode) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_COMPARE_MODE, to_value(mode));
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setCompareFunc(TestFunction func) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_COMPARE_FUNC, to_value(func));
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setLODBias(float bias) {
		glTexParameterf(to_value(BindTarget), GL_TEXTURE_LOD_BIAS, bias);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setMagFilter(MagFilter filter) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_MAG_FILTER, to_value(filter));
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setMaxLOD(float value) {
		glTexParameterf(to_value(BindTarget), GL_TEXTURE_MAX_LOD, value);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setMinFilter(MinFilter filter) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_MIN_FILTER, to_value(filter));
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setMinLOD(float value) {
		glTexParameterf(to_value(BindTarget), GL_TEXTURE_MIN_LOD, value);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setSwizzle(GLint r, GLint g, GLint b, GLint a) {
		GLint value[]{r, g, b, a};
		glTexParameteriv(to_value(BindTarget), GL_TEXTURE_SWIZZLE_RGBA, value);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setSwizzle(Swizzle r, Swizzle g, Swizzle b, Swizzle a) {
		setSwizzle(to_value(r), to_value(g), to_value(b), to_value(a));
	}
	inline void setSwizzleR(GLint swizzle) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_SWIZZLE_R, swizzle);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setSwizzleG(GLint swizzle) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_SWIZZLE_G, swizzle);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setSwizzleB(GLint swizzle) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_SWIZZLE_B, swizzle);
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setSwizzleA(GLint swizzle) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_SWIZZLE_A, swizzle);
		LIBV_GL_DEBUG_CHECK();
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
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_WRAP_S, to_value(s));
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_WRAP_T, to_value(t));
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setWrap(Wrap s, Wrap t, Wrap r) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_WRAP_S, to_value(s));
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_WRAP_T, to_value(t));
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_WRAP_R, to_value(r));
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setWrapS(Wrap wrap) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_WRAP_S, to_value(wrap));
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setWrapT(Wrap wrap) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_WRAP_T, to_value(wrap));
		LIBV_GL_DEBUG_CHECK();
	}
	inline void setWrapR(Wrap wrap) {
		glTexParameteri(to_value(BindTarget), GL_TEXTURE_WRAP_R, to_value(wrap));
		LIBV_GL_DEBUG_CHECK();
	}

public:
	inline auto id() const {
		return textureID;
	}
	inline operator GLuint() const {
		return textureID;
	}
};

// TextureGuard ------------------------------------------------------------------------------------

template <TextureBindTarget BindTarget>
struct TextureGuard : public Texture<BindTarget> {
	inline TextureGuard() = default;
	inline TextureGuard(const TextureGuard&) = delete;
	inline TextureGuard(TextureGuard&& other) {
		std::swap(this->textureID, other.textureID);
	}
	inline TextureGuard(Texture<BindTarget>&& other) {
		std::swap(this->textureID, other.textureID);
	}
	inline ~TextureGuard() {
		if(this->textureID)
			this->destroy();
	}
};

// TextureAliases ----------------------------------------------------------------------------------

struct Texture2D : Texture<TextureBindTarget::_2D> {};
struct TextureCube : Texture<TextureBindTarget::CubeMap> {};

using Texture2DGuard = TextureGuard<TextureBindTarget::_2D>;
using TextureCubeGuard = TextureGuard<TextureBindTarget::CubeMap>;

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

} // namespace gl
} // namespace libv


// =================================================================================================

// Emulating glTexStorage*D for pre 4.2 compatibility:
//  Failed iteration on implementation due to compatableDefaultType.
//  Failed iteration on implementation due to compatableDefaultFormat.
//
//
//inline void emulateStorage1D(TextureBindTarget target, InternalFormat internalFormat, size_t levels, uint32_t width) {
//	glGenTextures(1, &textureID);
//	if (textureID == invalidID)
//		LIBV_LOG_GL_ERROR("Failed to create 1D texture", to_string(type));
//
//	// Simulating glTexStorage1D for pre 4.2 compatibility
//	DataType type = compatableDefaultType(target);
//	Format format = compatableDefaultFormat(target);
//	glBindTexture(to_value(BindTarget), textureID);
//
//	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
//	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, levels - 1);
//
//	for (size_t i = 0; i < levels; i++) {
//		glTexImage1D(target, i, internalFormat, width, 0, format, type, nullptr);
//		width = std::max(1, (width / 2));
//	}
//	LIBV_GL_DEBUG_CHECK();
//}
//
//inline void emulateStorage2D(TextureBindTarget target, InternalFormat internalFormat, size_t levels, uint32_t width, uint32_t height) {
//	glGenTextures(1, &textureID);
//	if (textureID == invalidID)
//		LIBV_LOG_GL_ERROR("Failed to create 2D texture", to_string(type));
//
//	// Simulating glTexStorage2D for pre 4.2 compatibility
//	DataType type = compatableDefaultType(target);
//	Format format = compatableDefaultFormat(target);
//	glBindTexture(to_value(BindTarget), textureID);
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
//			glTexImage2D(to_value(BindTarget), i, internalFormat, width, height, 0, format, type, nullptr);
//			width = std::max(1, (width / 2));
//		}
//	} else {
//		for (size_t i = 0; i < levels; i++) {
//			glTexImage2D(to_value(BindTarget), i, internalFormat, width, height, 0, format, type, nullptr);
//			width = std::max(1, (width / 2));
//			height = std::max(1, (height / 2));
//		}
//	}
//	LIBV_GL_DEBUG_CHECK();
//}
//inline void emulateStorage3D(TextureBindTarget target, InternalFormat internalFormat, size_t levels, uint32_t width, uint32_t height, uint32_t depth) {
//	glGenTextures(3, &textureID);
//	if (textureID == invalidID)
//		LIBV_LOG_GL_ERROR("Failed to create 2D texture", to_string(type));
//
//	// Simulating glTexStorage3D for pre 4.2 compatibility
//	DataType type = compatableDefaultType(target);
//	Format format = compatableDefaultFormat(target);
//	glBindTexture(to_value(BindTarget), textureID);
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
//	LIBV_GL_DEBUG_CHECK();
//}
