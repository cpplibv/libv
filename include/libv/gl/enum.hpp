// File: enum.hpp Author: Vader Created on 2016. július 27., 4:55

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/utility/enum.hpp>


// -------------------------------------------------------------------------------------------------

namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

enum class BufferBit : GLenum {
	Color = GL_COLOR_BUFFER_BIT,
	Depth = GL_DEPTH_BUFFER_BIT,
	Stencil = GL_STENCIL_BUFFER_BIT,
};
inline BufferBit operator|(BufferBit lhs, BufferBit rhs) {
	return static_cast<BufferBit>(libv::to_value(lhs) | libv::to_value(rhs));
}

enum class BufferUsage : GLenum {
	StreamDraw = GL_STREAM_DRAW,
	StreamRead = GL_STREAM_READ,
	StreamCopy = GL_STREAM_COPY,
	StaticDraw = GL_STATIC_DRAW,
	StaticRead = GL_STATIC_READ,
	StaticCopy = GL_STATIC_COPY,
	DynamicDraw = GL_DYNAMIC_DRAW,
	DynamicRead = GL_DYNAMIC_READ,
	DynamicCopy = GL_DYNAMIC_COPY,
};

enum class BufferTarget : GLenum {
	Array = GL_ARRAY_BUFFER, /// Vertex attributes
	AtomicCounter = GL_ATOMIC_COUNTER_BUFFER, /// Atomic counter storage
	CopyRead = GL_COPY_READ_BUFFER, /// Buffer copy source
	CopyWrite = GL_COPY_WRITE_BUFFER, /// Buffer copy destination
	DispatchIndirect_ = GL_DISPATCH_INDIRECT_BUFFER, /// Indirect compute dispatch commands
	DrawIndirect = GL_DRAW_INDIRECT_BUFFER, /// Indirect command arguments
	ElementArray = GL_ELEMENT_ARRAY_BUFFER, /// Vertex array indices
	PixelPack = GL_PIXEL_PACK_BUFFER, /// Pixel read target
	PixelUnpack = GL_PIXEL_UNPACK_BUFFER, /// Texture data source
	Query = GL_QUERY_BUFFER, /// Query result buffer
	ShaderStorage = GL_SHADER_STORAGE_BUFFER, /// Read-write storage for shaders
	Texture = GL_TEXTURE_BUFFER, /// Texture data buffer
	TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER, /// Transform feedback buffer
	Uniform = GL_UNIFORM_BUFFER, /// Uniform block storage
};

enum class Capability : GLenum {
	Blend = GL_BLEND,
	CullFace = GL_CULL_FACE,
	DepthTest = GL_DEPTH_TEST,
	RasterizerDiscard = GL_RASTERIZER_DISCARD,
	ScissorTest = GL_SCISSOR_TEST,
	StencilTest = GL_STENCIL_TEST,
	TextureCubeMapSeamless = GL_TEXTURE_CUBE_MAP_SEAMLESS,
};

enum class Primitive : GLenum {
	LineLoop = GL_LINE_LOOP,
	LineStrip = GL_LINE_STRIP,
	LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
	Lines = GL_LINES,
	LinesAdjacency = GL_LINES_ADJACENCY,
	Patches = GL_PATCHES,
	Points = GL_POINTS,
	TriangleFan = GL_TRIANGLE_FAN,
	TriangleStrip = GL_TRIANGLE_STRIP,
	TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
	Triangles = GL_TRIANGLES,
	TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
};

enum class StencilAction : GLenum {
	Keep = GL_KEEP,
	Zero = GL_ZERO,
	Replace = GL_REPLACE,
	Increase = GL_INCR,
	IncreaseWrap = GL_INCR_WRAP,
	Decrease = GL_DECR,
	DecreaseWrap = GL_DECR_WRAP,
	Invert = GL_INVERT,
};

// -------------------------------------------------------------------------------------------------

//enum class TextureTarget {
//	_1D = GL_TEXTURE_1D,
//	_1DArray = GL_TEXTURE_1D_ARRAY,
//	_2D = GL_TEXTURE_2D,
//	_2DArray = GL_TEXTURE_2D_ARRAY,
//	_2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
//	_2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
//	_3D = GL_TEXTURE_3D,
//	_Buffer = GL_TEXTURE_BUFFER,
//	_CubeMap = GL_TEXTURE_CUBE_MAP,
//	_CubeMapPositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
//	_CubeMapNegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
//	_CubeMapPositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
//	_CubeMapNegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
//	_CubeMapPositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
//	_CubeMapNegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
//	_CubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
//	_Rectangle = GL_TEXTURE_RECTANGLE,
//};

// -------------------------------------------------------------------------------------------------

enum class CubeSide : GLenum {
	PositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	NegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	PositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	NegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	PositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	NegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

//enum class TextureTarget1D {
//	_1D = GL_TEXTURE_1D,
//	Proxy1D = GL_PROXY_TEXTURE_1D,
//};
//
//enum class TextureTarget2D {
//	_2D = GL_TEXTURE_2D,
//	_1DArray = GL_TEXTURE_1D_ARRAY,
//	Rectangle = GL_TEXTURE_RECTANGLE,
//	CubeMapPositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
//	CubeMapNegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
//	CubeMapPositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
//	CubeMapNegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
//	CubeMapPositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
//	CubeMapNegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	//	Proxy2D = GL_PROXY_TEXTURE_2D,
	//	Proxy1DArray = GL_PROXY_TEXTURE_1D_ARRAY,
	//	ProxyRectangle = GL_PROXY_TEXTURE_RECTANGLE,
	//	ProxyCubeMap = GL_PROXY_TEXTURE_CUBE_MAP,
//};
//
//enum class TextureTarget3D {
//	_3D = GL_TEXTURE_3D,
//	_2DArray = GL_TEXTURE_2D_ARRAY,
	//	Proxy3D = GL_PROXY_TEXTURE_3D,
	//	Proxy2DArray = GL_PROXY_TEXTURE_2D_ARRAY,
//};

enum class TextureTarget : GLenum {
	_1D = GL_TEXTURE_1D,
	_2D = GL_TEXTURE_2D,
	_3D = GL_TEXTURE_3D,
	_1DArray = GL_TEXTURE_1D_ARRAY,
	_2DArray = GL_TEXTURE_2D_ARRAY,
	Rectangle = GL_TEXTURE_RECTANGLE,
	CubeMap = GL_TEXTURE_CUBE_MAP,
	CubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
	Buffer = GL_TEXTURE_BUFFER,
	_2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
	_2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
};

//constexpr inline TextureTarget translateToBindTarget(TextureTarget1D target) {
//	switch (target) {
//	default:
//	case TextureTarget1D::_1D: return TextureTarget::_1D;
//	case TextureTarget1D::Proxy1D: return TextureTarget::_1D;
//	}
//}
//constexpr inline TextureTarget translateToBindTarget(TextureTarget2D target) {
//	switch (target) {
//	default:
//	case TextureTarget2D::_2D: return TextureTarget::_2D;
//	case TextureTarget2D::_1DArray: return TextureTarget::_1DArray;
//	case TextureTarget2D::Rectangle: return TextureTarget::Rectangle;
//	case TextureTarget2D::CubeMapPositiveX: return TextureTarget::CubeMap;
//	case TextureTarget2D::CubeMapNegativeX: return TextureTarget::CubeMap;
//	case TextureTarget2D::CubeMapPositiveY: return TextureTarget::CubeMap;
//	case TextureTarget2D::CubeMapNegativeY: return TextureTarget::CubeMap;
//	case TextureTarget2D::CubeMapPositiveZ: return TextureTarget::CubeMap;
//	case TextureTarget2D::CubeMapNegativeZ: return TextureTarget::CubeMap;
//	case TextureTarget2D::Proxy2D: return TextureTarget::_2D;
//	case TextureTarget2D::Proxy1DArray: return TextureTarget::_1DArray;
//	case TextureTarget2D::ProxyRectangle: return TextureTarget::Rectangle;
//	case TextureTarget2D::ProxyCubeMap: return TextureTarget::CubeMap;
//	}
//}
//constexpr inline TextureTarget translateToBindTarget(TextureTarget3D target) {
//	switch (target) {
//	default:
//	case TextureTarget3D::_3D: return TextureTarget::_3D;
//	case TextureTarget3D::_2DArray: return TextureTarget::_2DArray;
//	case TextureTarget3D::Proxy3D: return TextureTarget::_3D;
//	case TextureTarget3D::Proxy2DArray: return TextureTarget::_2DArray;
//	}
//}

// -------------------------------------------------------------------------------------------------

enum class DepthStencilMode : GLenum {
	DepthComponent = GL_DEPTH_COMPONENT,
	StencilIndex = GL_STENCIL_INDEX,
};

enum class CompareMode : GLenum {
	None = GL_NONE,
	CompareRefToTexture = GL_COMPARE_REF_TO_TEXTURE,
};

enum class TestFunction : GLenum {
	LEqual = GL_LEQUAL,
	GEqual = GL_GEQUAL,
	Less = GL_LESS,
	Greater = GL_GREATER,
	Equal = GL_EQUAL,
	NotEqual = GL_NOTEQUAL,
	Always = GL_ALWAYS,
	Never = GL_NEVER,
};

enum class BlendFunction : GLenum {
	ConstantAlpha = GL_CONSTANT_ALPHA,
	ConstantColor = GL_CONSTANT_COLOR,
	DestinationAlpha = GL_DST_ALPHA,
	DestinationColor = GL_DST_COLOR,
	One = GL_ONE,
	One_Minus_ConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
	One_Minus_ConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
	One_Minus_DestinationAlpha = GL_ONE_MINUS_DST_ALPHA,
	One_Minus_DestinationColor = GL_ONE_MINUS_DST_COLOR,
	One_Minus_Source1Alpha = GL_ONE_MINUS_SRC1_ALPHA,
	One_Minus_Source1Color = GL_ONE_MINUS_SRC1_COLOR,
	One_Minus_SourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
	One_Minus_SourceColor = GL_ONE_MINUS_SRC_COLOR,
	Source1Alpha = GL_SRC1_ALPHA,
	Source1Color = GL_SRC1_COLOR,
	SourceAlpha = GL_SRC_ALPHA,
	SourceAlphaSaturate = GL_SRC_ALPHA_SATURATE,
	SourceColor = GL_SRC_COLOR,
	Zero = GL_ZERO,
};

enum class MagFilter : GLenum {
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
};

enum class MinFilter : GLenum {
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
	NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
};

enum class Swizzle : GLenum {
	Red = GL_RED,
	Green = GL_GREEN,
	Blue = GL_BLUE,
	Alpha = GL_ALPHA,
	Zero = GL_ZERO,
	One = GL_ONE,
};

enum class Wrap : GLenum {
	ClampToEdge = GL_CLAMP_TO_EDGE,
	Repeat = GL_REPEAT,
	ClampToBorder = GL_CLAMP_TO_BORDER,
	MirroredRepeat = GL_MIRRORED_REPEAT,
	MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE,
};

enum class BaseInternalFormat : GLenum {
	RED = GL_RED,
	RG = GL_RG,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
	DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
	DEPTH_STENCIL = GL_DEPTH_STENCIL,
	STENCIL_INDEX = GL_STENCIL_INDEX,
};

enum class CompressedFormat : GLenum {
	RED = GL_COMPRESSED_RED,
	RG = GL_COMPRESSED_RG,
	RGB = GL_COMPRESSED_RGB,
	RGBA = GL_COMPRESSED_RGBA,
	SRGB = GL_COMPRESSED_SRGB,
	SRGB_ALPHA = GL_COMPRESSED_SRGB_ALPHA,
	RED_RGTC1 = GL_COMPRESSED_RED_RGTC1,
	SIGNED_RED_RGTC1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
	RG_RGTC2 = GL_COMPRESSED_RG_RGTC2,
	SIGNED_RG_RGTC2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
	RGBA_BPTC_UNORM = GL_COMPRESSED_RGBA_BPTC_UNORM,
	SRGB_ALPHA_BPTC_UNORM = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
	RGB_BPTC_SIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
	RGB_BPTC_UNSIGNED_FLOAT = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,

	// S3TC formats
	RGB_S3TC_DXT1_EXT = GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
	SRGB_S3TC_DXT1_EXT = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT,
	RGBA_S3TC_DXT1_EXT = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
	SRGB_ALPHA_S3TC_DXT1_EXT = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT,
	RGBA_S3TC_DXT3_EXT = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
	SRGB_ALPHA_S3TC_DXT3_EXT = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT,
	RGBA_S3TC_DXT5_EXT = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
	SRGB_ALPHA_S3TC_DXT5_EXT = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT,
};

enum class DepthFormat : GLenum {
	DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
	DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
	DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32,
	DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
};

enum class DepthStencilFormat : GLenum {
	DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
	DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
};

enum class StencilFormat : GLenum {
	STENCIL_INDEX1 = GL_STENCIL_INDEX1,
	STENCIL_INDEX4 = GL_STENCIL_INDEX4,
	STENCIL_INDEX8 = GL_STENCIL_INDEX8,
	STENCIL_INDEX16 = GL_STENCIL_INDEX16,
};

enum class SizedInternalFormat : GLenum {
	R8 = GL_R8,
	R8_SNORM = GL_R8_SNORM,
	R16 = GL_R16,
	R16_SNORM = GL_R16_SNORM,
	RG8 = GL_RG8,
	RG8_SNORM = GL_RG8_SNORM,
	RG16 = GL_RG16,
	RG16_SNORM = GL_RG16_SNORM,
	R3_G3_B2 = GL_R3_G3_B2,
	RGB4 = GL_RGB4,
	RGB5 = GL_RGB5,
	RGB8 = GL_RGB8,
	RGB8_SNORM = GL_RGB8_SNORM,
	RGB10 = GL_RGB10,
	RGB12 = GL_RGB12,
	RGB16_SNORM = GL_RGB16_SNORM,
	RGBA2 = GL_RGBA2,
	RGBA4 = GL_RGBA4,
	RGB5_A1 = GL_RGB5_A1,
	RGBA8 = GL_RGBA8,
	RGBA8_SNORM = GL_RGBA8_SNORM,
	RGB10_A2 = GL_RGB10_A2,
	RGB10_A2UI = GL_RGB10_A2UI,
	RGBA12 = GL_RGBA12,
	RGBA16 = GL_RGBA16,
	SRGB8 = GL_SRGB8,
	SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
	R16F = GL_R16F,
	RG16F = GL_RG16F,
	RGB16F = GL_RGB16F,
	RGBA16F = GL_RGBA16F,
	R32F = GL_R32F,
	RG32F = GL_RG32F,
	RGB32F = GL_RGB32F,
	RGBA32F = GL_RGBA32F,
	R11F_G11F_B10F = GL_R11F_G11F_B10F,
	RGB9_E5 = GL_RGB9_E5,
	R8I = GL_R8I,
	R8UI = GL_R8UI,
	R16I = GL_R16I,
	R16UI = GL_R16UI,
	R32I = GL_R32I,
	R32UI = GL_R32UI,
	RG8I = GL_RG8I,
	RG8UI = GL_RG8UI,
	RG16I = GL_RG16I,
	RG16UI = GL_RG16UI,
	RG32I = GL_RG32I,
	RG32UI = GL_RG32UI,
	RGB8I = GL_RGB8I,
	RGB8UI = GL_RGB8UI,
	RGB16I = GL_RGB16I,
	RGB16UI = GL_RGB16UI,
	RGB32I = GL_RGB32I,
	RGB32UI = GL_RGB32UI,
	RGBA8I = GL_RGBA8I,
	RGBA8UI = GL_RGBA8UI,
	RGBA16I = GL_RGBA16I,
	RGBA16UI = GL_RGBA16UI,
	RGBA32I = GL_RGBA32I,
	RGBA32UI = GL_RGBA32UI,
};

constexpr inline BaseInternalFormat compatibleBaseFormat(DepthFormat format) noexcept {
	switch (format) {
	default:
	case DepthFormat::DEPTH_COMPONENT16:        return BaseInternalFormat::DEPTH_COMPONENT;
	case DepthFormat::DEPTH_COMPONENT24:        return BaseInternalFormat::DEPTH_COMPONENT;
	case DepthFormat::DEPTH_COMPONENT32:        return BaseInternalFormat::DEPTH_COMPONENT;
	case DepthFormat::DEPTH_COMPONENT32F:       return BaseInternalFormat::DEPTH_COMPONENT;
	}
}

constexpr inline BaseInternalFormat compatibleBaseFormat(DepthStencilFormat format) noexcept {
	switch (format) {
	default:
	case DepthStencilFormat::DEPTH24_STENCIL8:  return BaseInternalFormat::DEPTH_STENCIL;
	case DepthStencilFormat::DEPTH32F_STENCIL8: return BaseInternalFormat::DEPTH_STENCIL;
	}
}

constexpr inline BaseInternalFormat compatibleBaseFormat(StencilFormat format) noexcept {
	switch (format) {
	default:
	case StencilFormat::STENCIL_INDEX1:         return BaseInternalFormat::STENCIL_INDEX;
	case StencilFormat::STENCIL_INDEX4:         return BaseInternalFormat::STENCIL_INDEX;
	case StencilFormat::STENCIL_INDEX8:         return BaseInternalFormat::STENCIL_INDEX;
	case StencilFormat::STENCIL_INDEX16:        return BaseInternalFormat::STENCIL_INDEX;
	}
}

constexpr inline BaseInternalFormat compatibleBaseFormat(CompressedFormat format) noexcept {
	switch (format) {
	default:
	case CompressedFormat::RED:                      return BaseInternalFormat::RED;
	case CompressedFormat::RG:                       return BaseInternalFormat::RG;
	case CompressedFormat::RGB:                      return BaseInternalFormat::RGB;
	case CompressedFormat::RGBA:                     return BaseInternalFormat::RGBA;
	case CompressedFormat::SRGB:                     return BaseInternalFormat::RGB;
	case CompressedFormat::SRGB_ALPHA:               return BaseInternalFormat::RGBA;
	case CompressedFormat::RED_RGTC1:                return BaseInternalFormat::RED;
	case CompressedFormat::SIGNED_RED_RGTC1:         return BaseInternalFormat::RED;
	case CompressedFormat::RG_RGTC2:                 return BaseInternalFormat::RG;
	case CompressedFormat::SIGNED_RG_RGTC2:          return BaseInternalFormat::RG;
	case CompressedFormat::RGBA_BPTC_UNORM:          return BaseInternalFormat::RGBA;
	case CompressedFormat::SRGB_ALPHA_BPTC_UNORM:    return BaseInternalFormat::RGBA;
	case CompressedFormat::RGB_BPTC_SIGNED_FLOAT:    return BaseInternalFormat::RGB;
	case CompressedFormat::RGB_BPTC_UNSIGNED_FLOAT:  return BaseInternalFormat::RGB;

	case CompressedFormat::RGB_S3TC_DXT1_EXT:        return BaseInternalFormat::RGB;
	case CompressedFormat::SRGB_S3TC_DXT1_EXT:       return BaseInternalFormat::RGB;
	case CompressedFormat::RGBA_S3TC_DXT1_EXT:       return BaseInternalFormat::RGBA;
	case CompressedFormat::SRGB_ALPHA_S3TC_DXT1_EXT: return BaseInternalFormat::RGBA;
	case CompressedFormat::RGBA_S3TC_DXT3_EXT:       return BaseInternalFormat::RGBA;
	case CompressedFormat::SRGB_ALPHA_S3TC_DXT3_EXT: return BaseInternalFormat::RGBA;
	case CompressedFormat::RGBA_S3TC_DXT5_EXT:       return BaseInternalFormat::RGBA;
	case CompressedFormat::SRGB_ALPHA_S3TC_DXT5_EXT: return BaseInternalFormat::RGBA;
	}
}

constexpr inline BaseInternalFormat compatibleBaseFormat(SizedInternalFormat format) noexcept {
	switch (format) {
	default:
	case SizedInternalFormat::R8:               return BaseInternalFormat::RED;
	case SizedInternalFormat::R8_SNORM:         return BaseInternalFormat::RED;
	case SizedInternalFormat::R16:              return BaseInternalFormat::RED;
	case SizedInternalFormat::R16_SNORM:        return BaseInternalFormat::RED;
	case SizedInternalFormat::RG8:              return BaseInternalFormat::RG;
	case SizedInternalFormat::RG8_SNORM:        return BaseInternalFormat::RG;
	case SizedInternalFormat::RG16:             return BaseInternalFormat::RG;
	case SizedInternalFormat::RG16_SNORM:       return BaseInternalFormat::RG;
	case SizedInternalFormat::R3_G3_B2:         return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB4:             return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB5:             return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB8:             return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB8_SNORM:       return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB10:            return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB12:            return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB16_SNORM:      return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGBA2:            return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGBA4:            return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGB5_A1:          return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGBA8:            return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGBA8_SNORM:      return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGB10_A2:         return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGB10_A2UI:       return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGBA12:           return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGBA16:           return BaseInternalFormat::RGBA;
	case SizedInternalFormat::SRGB8:            return BaseInternalFormat::RGB;
	case SizedInternalFormat::SRGB8_ALPHA8:     return BaseInternalFormat::RGBA;
	case SizedInternalFormat::R16F:             return BaseInternalFormat::RED;
	case SizedInternalFormat::RG16F:            return BaseInternalFormat::RG;
	case SizedInternalFormat::RGB16F:           return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGBA16F:          return BaseInternalFormat::RGBA;
	case SizedInternalFormat::R32F:             return BaseInternalFormat::RED;
	case SizedInternalFormat::RG32F:            return BaseInternalFormat::RG;
	case SizedInternalFormat::RGB32F:           return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGBA32F:          return BaseInternalFormat::RGBA;
	case SizedInternalFormat::R11F_G11F_B10F:   return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB9_E5:          return BaseInternalFormat::RGB;
	case SizedInternalFormat::R8I:              return BaseInternalFormat::RED;
	case SizedInternalFormat::R8UI:             return BaseInternalFormat::RED;
	case SizedInternalFormat::R16I:             return BaseInternalFormat::RED;
	case SizedInternalFormat::R16UI:            return BaseInternalFormat::RED;
	case SizedInternalFormat::R32I:             return BaseInternalFormat::RED;
	case SizedInternalFormat::R32UI:            return BaseInternalFormat::RED;
	case SizedInternalFormat::RG8I:             return BaseInternalFormat::RG;
	case SizedInternalFormat::RG8UI:            return BaseInternalFormat::RG;
	case SizedInternalFormat::RG16I:            return BaseInternalFormat::RG;
	case SizedInternalFormat::RG16UI:           return BaseInternalFormat::RG;
	case SizedInternalFormat::RG32I:            return BaseInternalFormat::RG;
	case SizedInternalFormat::RG32UI:           return BaseInternalFormat::RG;
	case SizedInternalFormat::RGB8I:            return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB8UI:           return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB16I:           return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB16UI:          return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB32I:           return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGB32UI:          return BaseInternalFormat::RGB;
	case SizedInternalFormat::RGBA8I:           return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGBA8UI:          return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGBA16I:          return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGBA16UI:         return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGBA32I:          return BaseInternalFormat::RGBA;
	case SizedInternalFormat::RGBA32UI:         return BaseInternalFormat::RGBA;
	}
}

enum class DataType : GLenum {
	UByte = GL_UNSIGNED_BYTE,
	Byte = GL_BYTE,
	Bitmap = GL_BITMAP,
	UShort = GL_UNSIGNED_SHORT,
	Short = GL_SHORT,
	UInt = GL_UNSIGNED_INT,
	Int = GL_INT,
	Float = GL_FLOAT,
	UByte_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
	UByte_2_3_3_rev = GL_UNSIGNED_BYTE_2_3_3_REV,
	UShort_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
	UShort_5_6_5_rev = GL_UNSIGNED_SHORT_5_6_5_REV,
	UShort_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
	UShort_4_4_4_4_rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
	UShort_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
	UShort_1_5_5_5_rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
	UInt_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
	UInt_8_8_8_8_rev = GL_UNSIGNED_INT_8_8_8_8_REV,
	UInt_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
	UInt_2_10_10_10_rev = GL_UNSIGNED_INT_2_10_10_10_REV,
};

enum class Filter : GLenum {
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
	LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
};

// ShaderType --------------------------------------------------------------------------------------

enum class ShaderType : GLenum {
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	Compute = GL_COMPUTE_SHADER,
	TessControl = GL_TESS_CONTROL_SHADER,
	TessEvaluation = GL_TESS_EVALUATION_SHADER,
};

inline const char* to_string(ShaderType type) {
	switch (type) {
	case ShaderType::Vertex: return "vertex";
	case ShaderType::Geometry: return "geometry";
	case ShaderType::Fragment: return "fragment";
	case ShaderType::Compute: return "compute";
	case ShaderType::TessControl: return "tess-control";
	case ShaderType::TessEvaluation: return "tess-evaluation";
	default: return "unknown";
	}
}

// TextureType -------------------------------------------------------------------------------------

enum class TextureChannel : GLint {
	diffuse = 0,
	normal = 1,
	specular = 2,
	emission = 3,
	environment = 4,
	pass = 5,
	ambient = 6,
	//	_7 = 7,
	sky = 8,
	//	_9 = 9,
	shadow0 = 10,
	shadow1 = 11,
	shadow2 = 12,
	shadow3 = 13,
	shadow4 = 14,
	shadow5 = 15,
	shadow6 = 16,
	shadow7 = 17,
	//	_18 = 18,
	//	_19 = 19,
	//	_20 = 20,
};

// AttributeType --------------------------------------------------------------------------------

enum class AttributeType : GLenum {
	//Acceptable by glVertexAttribPointer:
	//Acceptable by glVertexAttribIPointer:
	BYTE = GL_BYTE,
	UBYTE = GL_UNSIGNED_BYTE,
	SHORT = GL_SHORT,
	USHORT = GL_UNSIGNED_SHORT,
	INT = GL_INT,
	UINT = GL_UNSIGNED_INT,

	//Acceptable by glVertexAttribPointer:
	HFLOAT = GL_HALF_FLOAT,
	FLOAT = GL_FLOAT,
	FIXED = GL_FIXED,
	INT_2_10_10_10_REV = GL_INT_2_10_10_10_REV,
	UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
	UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV,

	//Acceptable by glVertexAttribPointer:
	//Acceptable by glVertexAttribLPointer:
	DOUBLE = GL_DOUBLE,
};

template <typename T>
constexpr inline AttributeType toAttributeType() {
//		if constexpr (std::is_same_v<T, bool>)
//			return to_value(AttributeType::INT);
	// Bool cannot be just mapped for int, what about the sizes?

	if constexpr (std::is_same_v<T, int32_t>)
		return AttributeType::INT;

	if constexpr (std::is_same_v<T, float>)
		return AttributeType::FLOAT;

	if constexpr (std::is_same_v<T, double>)
		return AttributeType::DOUBLE;
}

// -------------------------------------------------------------------------------------------------

enum class CullFace : GLenum {
	Back = GL_BACK,
	Front = GL_FRONT,
};

enum class PolygonMode : GLenum {
	Fill = GL_FILL,
	Line = GL_LINE,
	Point = GL_POINT,
};

enum class FrontFace : GLenum {
	CCW = GL_CCW,
	CW = GL_CW,
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
