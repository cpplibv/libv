// File: enum.hpp Author: Vader Created on 2016. július 27., 4:55

#pragma once

// std
#include <cstdint>
// libv
#include <libv/utility/enum.hpp>


// -------------------------------------------------------------------------------------------------

using GLenum = uint32_t;
using GLint = int32_t;

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

enum class BufferBit : GLenum {
	Color = 0x00004000, /// GL_COLOR_BUFFER_BIT
	Depth = 0x00000100, /// GL_DEPTH_BUFFER_BIT
	Stencil = 0x00000400, /// GL_STENCIL_BUFFER_BIT

	ColorDepth = Color | Depth,
	DepthStencil = Depth | Stencil,

	ColorDepthStencil = Color | Depth | Stencil,
};
inline BufferBit operator|(BufferBit lhs, BufferBit rhs) {
	return static_cast<BufferBit>(libv::to_value(lhs) | libv::to_value(rhs));
}

enum class BufferUsage : GLenum {
	StreamDraw = 0x88E0, /// GL_STREAM_DRAW
	StreamRead = 0x88E1, /// GL_STREAM_READ
	StreamCopy = 0x88E2, /// GL_STREAM_COPY
	StaticDraw = 0x88E4, /// GL_STATIC_DRAW
	StaticRead = 0x88E5, /// GL_STATIC_READ
	StaticCopy = 0x88E6, /// GL_STATIC_COPY
	DynamicDraw = 0x88E8, /// GL_DYNAMIC_DRAW
	DynamicRead = 0x88E9, /// GL_DYNAMIC_READ
	DynamicCopy = 0x88EA, /// GL_DYNAMIC_COPY
};

//enum class BufferTarget : GLenum {
//	Array = 0x8892, /// GL_ARRAY_BUFFER
//	AtomicCounter = 0x92C0, /// GL_ATOMIC_COUNTER_BUFFER
//	CopyRead = 0x8F36, /// GL_COPY_READ_BUFFER
//	CopyWrite = 0x8F37, /// GL_COPY_WRITE_BUFFER
//	DispatchIndirect_ = 0x90EE, /// GL_DISPATCH_INDIRECT_BUFFER
//	DrawIndirect = 0x8F3F, /// GL_DRAW_INDIRECT_BUFFER
//	ElementArray = 0x8893, /// GL_ELEMENT_ARRAY_BUFFER
//	PixelPack = 0x88EB, /// GL_PIXEL_PACK_BUFFER
//	PixelUnpack = 0x88EC, /// GL_PIXEL_UNPACK_BUFFER
//	Query = 0x9192, /// GL_QUERY_BUFFER
//	ShaderStorage = 0x90D2, /// GL_SHADER_STORAGE_BUFFER
//	Texture = 0x8C2A, /// GL_TEXTURE_BUFFER
//	TransformFeedback = 0x8C8E, /// GL_TRANSFORM_FEEDBACK_BUFFER
//	Uniform = 0x8A11, /// GL_UNIFORM_BUFFER
//};

enum class Capability : GLenum {
	Blend = 0x0BE2, /// GL_BLEND
	CullFace = 0x0B44, /// GL_CULL_FACE
	DepthTest = 0x0B71, /// GL_DEPTH_TEST
	RasterizerDiscard = 0x8C89, /// GL_RASTERIZER_DISCARD
	ScissorTest = 0x0C11, /// GL_SCISSOR_TEST
	StencilTest = 0x0B90, /// GL_STENCIL_TEST
	TextureCubeMapSeamless = 0x884F, /// GL_TEXTURE_CUBE_MAP_SEAMLESS
};

enum class Primitive : GLenum {
	LineLoop = 0x0002, /// GL_LINE_LOOP
	LineStrip = 0x0003, /// GL_LINE_STRIP
	LineStripAdjacency = 0x000B, /// GL_LINE_STRIP_ADJACENCY
	Lines = 0x0001, /// GL_LINES
	LinesAdjacency = 0x000A, /// GL_LINES_ADJACENCY
	Patches = 0xE, /// GL_PATCHES
	Points = 0x0000, /// GL_POINTS
	TriangleFan = 0x0006, /// GL_TRIANGLE_FAN
	TriangleStrip = 0x0005, /// GL_TRIANGLE_STRIP
	TriangleStripAdjacency = 0x000D, /// GL_TRIANGLE_STRIP_ADJACENCY
	Triangles = 0x0004, /// GL_TRIANGLES
	TrianglesAdjacency = 0x000C, /// GL_TRIANGLES_ADJACENCY
};

enum class StencilAction : GLenum {
	Keep = 0x1E00, /// GL_KEEP
	Zero = 0, /// GL_ZERO
	Replace = 0x1E01, /// GL_REPLACE
	Increase = 0x1E02, /// GL_INCR
	IncreaseWrap = 0x8507, /// GL_INCR_WRAP
	Decrease = 0x1E03, /// GL_DECR
	DecreaseWrap = 0x8508, /// GL_DECR_WRAP
	Invert = 0x150A, /// GL_INVERT
};

// -------------------------------------------------------------------------------------------------

enum class CubeSide : GLenum {
	PositiveX = 0x8515, /// GL_TEXTURE_CUBE_MAP_POSITIVE_X
	NegativeX = 0x8516, /// GL_TEXTURE_CUBE_MAP_NEGATIVE_X
	PositiveY = 0x8517, /// GL_TEXTURE_CUBE_MAP_POSITIVE_Y
	NegativeY = 0x8518, /// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
	PositiveZ = 0x8519, /// GL_TEXTURE_CUBE_MAP_POSITIVE_Z
	NegativeZ = 0x851A, /// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

enum class TextureTarget : GLenum {
	_1D = 0x0DE0, /// GL_TEXTURE_1D
	_2D = 0x0DE1, /// GL_TEXTURE_2D
	_3D = 0x806F, /// GL_TEXTURE_3D
	_1DArray = 0x8C18, /// GL_TEXTURE_1D_ARRAY
	_2DArray = 0x8C1A, /// GL_TEXTURE_2D_ARRAY
	Rectangle = 0x84F5, /// GL_TEXTURE_RECTANGLE
	CubeMap = 0x8513, /// GL_TEXTURE_CUBE_MAP
	CubeMapArray = 0x9009, /// GL_TEXTURE_CUBE_MAP_ARRAY
	Buffer = 0x8C2A, /// GL_TEXTURE_BUFFER
	_2DMultisample = 0x9100, /// GL_TEXTURE_2D_MULTISAMPLE
	_2DMultisampleArray = 0x9102, /// GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

// -------------------------------------------------------------------------------------------------

enum class DepthStencilMode : GLenum {
	DepthComponent = 0x1902, /// GL_DEPTH_COMPONENT
	StencilIndex = 0x1901, /// GL_STENCIL_INDEX
};

enum class CompareMode : GLenum {
	None = 0, /// GL_NONE
	CompareRefToTexture = 0x884E, /// GL_COMPARE_REF_TO_TEXTURE
};

enum class TestFunction : GLenum {
	LEqual = 0x0203, /// GL_LEQUAL
	GEqual = 0x0206, /// GL_GEQUAL
	Less = 0x0201, /// GL_LESS
	Greater = 0x0204, /// GL_GREATER
	Equal = 0x0202, /// GL_EQUAL
	NotEqual = 0x0205, /// GL_NOTEQUAL
	Always = 0x0207, /// GL_ALWAYS
	Never = 0x0200, /// GL_NEVER
};

enum class BlendFunction : GLenum {
	ConstantAlpha = 0x8003, /// GL_CONSTANT_ALPHA
	ConstantColor = 0x8001, /// GL_CONSTANT_COLOR
	DestinationAlpha = 0x0304, /// GL_DST_ALPHA
	DestinationColor = 0x0306, /// GL_DST_COLOR
	One = 1, /// GL_ONE
	One_Minus_ConstantAlpha = 0x8004, /// GL_ONE_MINUS_CONSTANT_ALPHA
	One_Minus_ConstantColor = 0x8002, /// GL_ONE_MINUS_CONSTANT_COLOR
	One_Minus_DestinationAlpha = 0x0305, /// GL_ONE_MINUS_DST_ALPHA
	One_Minus_DestinationColor = 0x0307, /// GL_ONE_MINUS_DST_COLOR
	One_Minus_Source1Alpha = 0x88FB, /// GL_ONE_MINUS_SRC1_ALPHA
	One_Minus_Source1Color = 0x88FA, /// GL_ONE_MINUS_SRC1_COLOR
	One_Minus_SourceAlpha = 0x0303, /// GL_ONE_MINUS_SRC_ALPHA
	One_Minus_SourceColor = 0x0301, /// GL_ONE_MINUS_SRC_COLOR
	Source1Alpha = 0x8589, /// GL_SRC1_ALPHA
	Source1Color = 0x88F9, /// GL_SRC1_COLOR
	SourceAlpha = 0x0302, /// GL_SRC_ALPHA
	SourceAlphaSaturate = 0x0308, /// GL_SRC_ALPHA_SATURATE
	SourceColor = 0x0300, /// GL_SRC_COLOR
	Zero = 0, /// GL_ZERO
};

enum class MagFilter : GLenum {
	Nearest = 0x2600, /// GL_NEAREST
	Linear = 0x2601, /// GL_LINEAR
};

enum class MinFilter : GLenum {
	Nearest = 0x2600, /// GL_NEAREST
	Linear = 0x2601, /// GL_LINEAR
	NearestMipmapNearest = 0x2700, /// GL_NEAREST_MIPMAP_NEAREST
	NearestMipmapLinear = 0x2702, /// GL_NEAREST_MIPMAP_LINEAR
	LinearMipmapNearest = 0x2701, /// GL_LINEAR_MIPMAP_NEAREST
	LinearMipmapLinear = 0x2703, /// GL_LINEAR_MIPMAP_LINEAR
};

enum class Swizzle : GLenum {
	Red = 0x1903, /// GL_RED
	Green = 0x1904, /// GL_GREEN
	Blue = 0x1905, /// GL_BLUE
	Alpha = 0x1906, /// GL_ALPHA
	Zero = 0, /// GL_ZERO
	One = 1, /// GL_ONE
};

enum class Wrap : GLenum {
	ClampToEdge = 0x812F, /// GL_CLAMP_TO_EDGE
	Repeat = 0x2901, /// GL_REPEAT
	ClampToBorder = 0x812D, /// GL_CLAMP_TO_BORDER
	MirroredRepeat = 0x8370, /// GL_MIRRORED_REPEAT
	MirrorClampToEdge = 0x8743, /// GL_MIRROR_CLAMP_TO_EDGE
};

enum class Filter : GLenum {
	Nearest = 0x2600, /// GL_NEAREST
	Linear = 0x2601, /// GL_LINEAR
	NearestMipmapNearest = 0x2700, /// GL_NEAREST_MIPMAP_NEAREST
	LinearMipmapNearest = 0x2701, /// GL_LINEAR_MIPMAP_NEAREST
	NearestMipmapLinear = 0x2702, /// GL_NEAREST_MIPMAP_LINEAR
	LinearMipmapLinear = 0x2703, /// GL_LINEAR_MIPMAP_LINEAR
};

enum class DataType : GLenum {
	U8 = 0x1401, /// GL_UNSIGNED_BYTE
	I8 = 0x1400, /// GL_BYTE
	U16 = 0x1403, /// GL_UNSIGNED_SHORT
	I16 = 0x1402, /// GL_SHORT
	U32 = 0x1405, /// GL_UNSIGNED_INT
	I32 = 0x1404, /// GL_INT
	F32 = 0x1406, /// GL_FLOAT
	U8_R3_G3_B2 = 0x8032, /// GL_UNSIGNED_BYTE_3_3_2
	U8_B3_G3_R2 = 0x8362, /// GL_UNSIGNED_BYTE_2_3_3_REV
	U16_R5_G6_B5 = 0x8363, /// GL_UNSIGNED_SHORT_5_6_5
	U16_B5_G6_R5 = 0x8364, /// GL_UNSIGNED_SHORT_5_6_5_REV
	U16_R4_G4_B4_A4 = 0x8033, /// GL_UNSIGNED_SHORT_4_4_4_4
	U16_A4_B4_G4_R4 = 0x8365, /// GL_UNSIGNED_SHORT_4_4_4_4_REV
	U16_R5_G5_B5_A1 = 0x8034, /// GL_UNSIGNED_SHORT_5_5_5_1
	U16_A1_B5_G5_R5 = 0x8366, /// GL_UNSIGNED_SHORT_1_5_5_5_REV
	U32_R8_G8_B8_A8 = 0x8035, /// GL_UNSIGNED_INT_8_8_8_8
	U32_A8_B8_G8_R8 = 0x8367, /// GL_UNSIGNED_INT_8_8_8_8_REV
	U32_R10_G10_B10_A2 = 0x8036, /// GL_UNSIGNED_INT_10_10_10_2
	U32_A2_B10_G10_R10 = 0x8368, /// GL_UNSIGNED_INT_2_10_10_10_REV
};

constexpr inline auto sizeInfo(DataType type) noexcept {
	struct Result {
		int32_t size;
		int32_t pack;
	};

	switch (type) {
	case DataType::U8:                 return Result{1, 1};
	case DataType::I8:                 return Result{1, 1};
	case DataType::U16:                return Result{2, 1};
	case DataType::I16:                return Result{2, 1};
	case DataType::U32:                return Result{4, 1};
	case DataType::I32:                return Result{4, 1};
	case DataType::F32:                return Result{4, 1};
	case DataType::U8_R3_G3_B2:        return Result{1, 3};
	case DataType::U8_B3_G3_R2:        return Result{1, 3};
	case DataType::U16_R5_G6_B5:       return Result{2, 3};
	case DataType::U16_B5_G6_R5:       return Result{2, 3};
	case DataType::U16_R4_G4_B4_A4:    return Result{2, 4};
	case DataType::U16_A4_B4_G4_R4:    return Result{2, 4};
	case DataType::U16_R5_G5_B5_A1:    return Result{2, 4};
	case DataType::U16_A1_B5_G5_R5:    return Result{2, 4};
	case DataType::U32_R8_G8_B8_A8:    return Result{4, 4};
	case DataType::U32_A8_B8_G8_R8:    return Result{4, 4};
	case DataType::U32_R10_G10_B10_A2: return Result{4, 4};
	case DataType::U32_A2_B10_G10_R10: return Result{4, 4};
	}
	return Result{1, 1}; // [[unreachable]]
}

// -------------------------------------------------------------------------------------------------

enum class FormatBase : GLenum {
	R = 0x1903, /// GL_RED
	RG = 0x8227, /// GL_RG
	RGB = 0x1907, /// GL_RGB
	RGBA = 0x1908, /// GL_RGBA
	DEPTH = 0x1902, /// GL_DEPTH_COMPONENT
	DEPTH_STENCIL = 0x84F9, /// GL_DEPTH_STENCIL
	STENCIL = 0x1901, /// GL_STENCIL_INDEX
};

enum class FormatCompressed : GLenum {
	R = 0x8225, /// GL_COMPRESSED_RED
	RG = 0x8226, /// GL_COMPRESSED_RG
	RGB = 0x84ED, /// GL_COMPRESSED_RGB
	RGBA = 0x84EE, /// GL_COMPRESSED_RGBA
	SRGB = 0x8C48, /// GL_COMPRESSED_SRGB
	SRGBA = 0x8C49, /// GL_COMPRESSED_SRGB_ALPHA
	R_RGTC1 = 0x8DBB, /// GL_COMPRESSED_RED_RGTC1
	SIGNED_R_RGTC1 = 0x8DBC, /// GL_COMPRESSED_SIGNED_RED_RGTC1
	RG_RGTC2 = 0x8DBD, /// GL_COMPRESSED_RG_RGTC2
	SIGNED_RG_RGTC2 = 0x8DBE, /// GL_COMPRESSED_SIGNED_RG_RGTC2
	RGBA_BPTC_UNORM = 0x8E8C, /// GL_COMPRESSED_RGBA_BPTC_UNORM
	SRGBA_BPTC_UNORM = 0x8E8D, /// GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM
	RGB_BPTC_SIGNED_FLOAT = 0x8E8E, /// GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT
	RGB_BPTC_UNSIGNED_FLOAT = 0x8E8F, /// GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT

	// S3TC formats
	RGB_S3TC_DXT1_EXT = 0x83F0, /// GL_COMPRESSED_RGB_S3TC_DXT1_EXT
	RGBA_S3TC_DXT1_EXT = 0x83F1, /// GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
	RGBA_S3TC_DXT3_EXT = 0x83F2, /// GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
	RGBA_S3TC_DXT5_EXT = 0x83F3, /// GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
	SRGB_S3TC_DXT1_EXT = 0x8C4C, /// GL_COMPRESSED_SRGB_S3TC_DXT1_EXT
	SRGBA_S3TC_DXT1_EXT = 0x8C4D, /// GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT
	SRGBA_S3TC_DXT3_EXT = 0x8C4E, /// GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT
	SRGBA_S3TC_DXT5_EXT = 0x8C4F, /// GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT
};

enum class FormatDepth : GLenum {
	DEPTH_COMPONENT16 = 0x81A5, /// GL_DEPTH_COMPONENT16
	DEPTH_COMPONENT24 = 0x81A6, /// GL_DEPTH_COMPONENT24
	DEPTH_COMPONENT32 = 0x81A7, /// GL_DEPTH_COMPONENT32
	DEPTH_COMPONENT32F = 0x8CAC, /// GL_DEPTH_COMPONENT32F
};

enum class FormatDepthStencil : GLenum {
	DEPTH32F_STENCIL8 = 0x8CAD, /// GL_DEPTH32F_STENCIL8
	DEPTH24_STENCIL8 = 0x88F0, /// GL_DEPTH24_STENCIL8
};

enum class FormatStencil : GLenum {
	STENCIL_INDEX1 = 0x8D46, /// GL_STENCIL_INDEX1
	STENCIL_INDEX4 = 0x8D47, /// GL_STENCIL_INDEX4
	STENCIL_INDEX8 = 0x8D48, /// GL_STENCIL_INDEX8
	STENCIL_INDEX16 = 0x8D49, /// GL_STENCIL_INDEX16
};

enum class FormatSized : GLenum {
	R8 = 0x8229, /// GL_R8
	R8_SNORM = 0x8F94, /// GL_R8_SNORM
	R16 = 0x822A, /// GL_R16
	R16_SNORM = 0x8F98, /// GL_R16_SNORM
	RG8 = 0x822B, /// GL_RG8
	RG8_SNORM = 0x8F95, /// GL_RG8_SNORM
	RG16 = 0x822C, /// GL_RG16
	RG16_SNORM = 0x8F99, /// GL_RG16_SNORM
	R3_G3_B2 = 0x2A10, /// GL_R3_G3_B2
	RGB4 = 0x804F, /// GL_RGB4
	RGB5 = 0x8050, /// GL_RGB5
	RGB8 = 0x8051, /// GL_RGB8
	RGB8_SNORM = 0x8F96, /// GL_RGB8_SNORM
	RGB10 = 0x8052, /// GL_RGB10
	RGB12 = 0x8053, /// GL_RGB12
	RGB16_SNORM = 0x8F9A, /// GL_RGB16_SNORM
	RGBA2 = 0x8055, /// GL_RGBA2
	RGBA4 = 0x8056, /// GL_RGBA4
	RGB5_A1 = 0x8057, /// GL_RGB5_A1
	RGBA8 = 0x8058, /// GL_RGBA8
	RGBA8_SNORM = 0x8F97, /// GL_RGBA8_SNORM
	RGB10_A2 = 0x8059, /// GL_RGB10_A2
	RGB10_A2UI = 0x906F, /// GL_RGB10_A2UI
	RGBA12 = 0x805A, /// GL_RGBA12
	RGBA16 = 0x805B, /// GL_RGBA16
	SRGB8 = 0x8C41, /// GL_SRGB8
	SRGB8_ALPHA8 = 0x8C43, /// GL_SRGB8_ALPHA8
	R16F = 0x822D, /// GL_R16F
	RG16F = 0x822F, /// GL_RG16F
	RGB16F = 0x881B, /// GL_RGB16F
	RGBA16F = 0x881A, /// GL_RGBA16F
	R32F = 0x822E, /// GL_R32F
	RG32F = 0x8230, /// GL_RG32F
	RGB32F = 0x8815, /// GL_RGB32F
	RGBA32F = 0x8814, /// GL_RGBA32F
	R11F_G11F_B10F = 0x8C3A, /// GL_R11F_G11F_B10F
	RGB9_E5 = 0x8C3D, /// GL_RGB9_E5
	R8I = 0x8231, /// GL_R8I
	R8UI = 0x8232, /// GL_R8UI
	R16I = 0x8233, /// GL_R16I
	R16UI = 0x8234, /// GL_R16UI
	R32I = 0x8235, /// GL_R32I
	R32UI = 0x8236, /// GL_R32UI
	RG8I = 0x8237, /// GL_RG8I
	RG8UI = 0x8238, /// GL_RG8UI
	RG16I = 0x8239, /// GL_RG16I
	RG16UI = 0x823A, /// GL_RG16UI
	RG32I = 0x823B, /// GL_RG32I
	RG32UI = 0x823C, /// GL_RG32UI
	RGB8I = 0x8D8F, /// GL_RGB8I
	RGB8UI = 0x8D7D, /// GL_RGB8UI
	RGB16I = 0x8D89, /// GL_RGB16I
	RGB16UI = 0x8D77, /// GL_RGB16UI
	RGB32I = 0x8D83, /// GL_RGB32I
	RGB32UI = 0x8D71, /// GL_RGB32UI
	RGBA8I = 0x8D8E, /// GL_RGBA8I
	RGBA8UI = 0x8D7C, /// GL_RGBA8UI
	RGBA16I = 0x8D88, /// GL_RGBA16I
	RGBA16UI = 0x8D76, /// GL_RGBA16UI
	RGBA32I = 0x8D82, /// GL_RGBA32I
	RGBA32UI = 0x8D70, /// GL_RGBA32UI
};

constexpr inline int32_t dimInfo(FormatBase format) noexcept {
	switch (format) {
	case FormatBase::R: return 1;
	case FormatBase::RG: return 2;
	case FormatBase::RGB: return 3;
	case FormatBase::RGBA: return 4;
	case FormatBase::DEPTH: return 1;
	case FormatBase::DEPTH_STENCIL: return 2;
	case FormatBase::STENCIL: return 1;
	}
	return 1; // [[unreachable]]
}

constexpr inline FormatBase compatibleFormatBase(FormatDepth format) noexcept {
	switch (format) {
	case FormatDepth::DEPTH_COMPONENT16:  return FormatBase::DEPTH;
	case FormatDepth::DEPTH_COMPONENT24:  return FormatBase::DEPTH;
	case FormatDepth::DEPTH_COMPONENT32:  return FormatBase::DEPTH;
	case FormatDepth::DEPTH_COMPONENT32F: return FormatBase::DEPTH;
	}
	return FormatBase::DEPTH; // [[unreachable]]
}

constexpr inline FormatBase compatibleFormatBase(FormatDepthStencil format) noexcept {
	switch (format) {
	case FormatDepthStencil::DEPTH24_STENCIL8:  return FormatBase::DEPTH_STENCIL;
	case FormatDepthStencil::DEPTH32F_STENCIL8: return FormatBase::DEPTH_STENCIL;
	}
	return FormatBase::DEPTH_STENCIL; // [[unreachable]]
}

constexpr inline FormatBase compatibleFormatBase(FormatStencil format) noexcept {
	switch (format) {
	case FormatStencil::STENCIL_INDEX1:  return FormatBase::STENCIL;
	case FormatStencil::STENCIL_INDEX4:  return FormatBase::STENCIL;
	case FormatStencil::STENCIL_INDEX8:  return FormatBase::STENCIL;
	case FormatStencil::STENCIL_INDEX16: return FormatBase::STENCIL;
	}
	return FormatBase::STENCIL; // [[unreachable]]
}

constexpr inline FormatBase compatibleFormatBase(FormatCompressed format) noexcept {
	switch (format) {
	case FormatCompressed::R:                       return FormatBase::R;
	case FormatCompressed::RG:                      return FormatBase::RG;
	case FormatCompressed::RGB:                     return FormatBase::RGB;
	case FormatCompressed::RGBA:                    return FormatBase::RGBA;
	case FormatCompressed::SRGB:                    return FormatBase::RGB;
	case FormatCompressed::SRGBA:                   return FormatBase::RGBA;
	case FormatCompressed::R_RGTC1:                 return FormatBase::R;
	case FormatCompressed::SIGNED_R_RGTC1:          return FormatBase::R;
	case FormatCompressed::RG_RGTC2:                return FormatBase::RG;
	case FormatCompressed::SIGNED_RG_RGTC2:         return FormatBase::RG;
	case FormatCompressed::RGBA_BPTC_UNORM:         return FormatBase::RGBA;
	case FormatCompressed::SRGBA_BPTC_UNORM:        return FormatBase::RGBA;
	case FormatCompressed::RGB_BPTC_SIGNED_FLOAT:   return FormatBase::RGB;
	case FormatCompressed::RGB_BPTC_UNSIGNED_FLOAT: return FormatBase::RGB;

	case FormatCompressed::RGB_S3TC_DXT1_EXT:       return FormatBase::RGB;
	case FormatCompressed::RGBA_S3TC_DXT1_EXT:      return FormatBase::RGBA;
	case FormatCompressed::RGBA_S3TC_DXT3_EXT:      return FormatBase::RGBA;
	case FormatCompressed::RGBA_S3TC_DXT5_EXT:      return FormatBase::RGBA;
	case FormatCompressed::SRGB_S3TC_DXT1_EXT:      return FormatBase::RGB;
	case FormatCompressed::SRGBA_S3TC_DXT1_EXT:     return FormatBase::RGBA;
	case FormatCompressed::SRGBA_S3TC_DXT3_EXT:     return FormatBase::RGBA;
	case FormatCompressed::SRGBA_S3TC_DXT5_EXT:     return FormatBase::RGBA;
	}
	return FormatBase::R; // [[unreachable]]
}

constexpr inline FormatBase compatibleFormatBase(FormatSized format) noexcept {
	switch (format) {
	case FormatSized::R8:             return FormatBase::R;
	case FormatSized::R8_SNORM:       return FormatBase::R;
	case FormatSized::R16:            return FormatBase::R;
	case FormatSized::R16_SNORM:      return FormatBase::R;
	case FormatSized::RG8:            return FormatBase::RG;
	case FormatSized::RG8_SNORM:      return FormatBase::RG;
	case FormatSized::RG16:           return FormatBase::RG;
	case FormatSized::RG16_SNORM:     return FormatBase::RG;
	case FormatSized::R3_G3_B2:       return FormatBase::RGB;
	case FormatSized::RGB4:           return FormatBase::RGB;
	case FormatSized::RGB5:           return FormatBase::RGB;
	case FormatSized::RGB8:           return FormatBase::RGB;
	case FormatSized::RGB8_SNORM:     return FormatBase::RGB;
	case FormatSized::RGB10:          return FormatBase::RGB;
	case FormatSized::RGB12:          return FormatBase::RGB;
	case FormatSized::RGB16_SNORM:    return FormatBase::RGB;
	case FormatSized::RGBA2:          return FormatBase::RGBA;
	case FormatSized::RGBA4:          return FormatBase::RGBA;
	case FormatSized::RGB5_A1:        return FormatBase::RGBA;
	case FormatSized::RGBA8:          return FormatBase::RGBA;
	case FormatSized::RGBA8_SNORM:    return FormatBase::RGBA;
	case FormatSized::RGB10_A2:       return FormatBase::RGBA;
	case FormatSized::RGB10_A2UI:     return FormatBase::RGBA;
	case FormatSized::RGBA12:         return FormatBase::RGBA;
	case FormatSized::RGBA16:         return FormatBase::RGBA;
	case FormatSized::SRGB8:          return FormatBase::RGB;
	case FormatSized::SRGB8_ALPHA8:   return FormatBase::RGBA;
	case FormatSized::R16F:           return FormatBase::R;
	case FormatSized::RG16F:          return FormatBase::RG;
	case FormatSized::RGB16F:         return FormatBase::RGB;
	case FormatSized::RGBA16F:        return FormatBase::RGBA;
	case FormatSized::R32F:           return FormatBase::R;
	case FormatSized::RG32F:          return FormatBase::RG;
	case FormatSized::RGB32F:         return FormatBase::RGB;
	case FormatSized::RGBA32F:        return FormatBase::RGBA;
	case FormatSized::R11F_G11F_B10F: return FormatBase::RGB;
	case FormatSized::RGB9_E5:        return FormatBase::RGB;
	case FormatSized::R8I:            return FormatBase::R;
	case FormatSized::R8UI:           return FormatBase::R;
	case FormatSized::R16I:           return FormatBase::R;
	case FormatSized::R16UI:          return FormatBase::R;
	case FormatSized::R32I:           return FormatBase::R;
	case FormatSized::R32UI:          return FormatBase::R;
	case FormatSized::RG8I:           return FormatBase::RG;
	case FormatSized::RG8UI:          return FormatBase::RG;
	case FormatSized::RG16I:          return FormatBase::RG;
	case FormatSized::RG16UI:         return FormatBase::RG;
	case FormatSized::RG32I:          return FormatBase::RG;
	case FormatSized::RG32UI:         return FormatBase::RG;
	case FormatSized::RGB8I:          return FormatBase::RGB;
	case FormatSized::RGB8UI:         return FormatBase::RGB;
	case FormatSized::RGB16I:         return FormatBase::RGB;
	case FormatSized::RGB16UI:        return FormatBase::RGB;
	case FormatSized::RGB32I:         return FormatBase::RGB;
	case FormatSized::RGB32UI:        return FormatBase::RGB;
	case FormatSized::RGBA8I:         return FormatBase::RGBA;
	case FormatSized::RGBA8UI:        return FormatBase::RGBA;
	case FormatSized::RGBA16I:        return FormatBase::RGBA;
	case FormatSized::RGBA16UI:       return FormatBase::RGBA;
	case FormatSized::RGBA32I:        return FormatBase::RGBA;
	case FormatSized::RGBA32UI:       return FormatBase::RGBA;
	}
	return FormatBase::R; // [[unreachable]]
}

struct Format {
	GLenum format = to_value(FormatSized::RGBA8);
	FormatBase base = FormatBase::RGBA;

	constexpr inline Format() noexcept = default;
	constexpr inline Format(FormatBase format) noexcept :
		format(to_value(format)),
		base(format) { }
	constexpr inline Format(FormatCompressed format) noexcept :
		format(to_value(format)),
		base(compatibleFormatBase(format)) { }
	constexpr inline Format(FormatSized format) noexcept :
		format(to_value(format)),
		base(compatibleFormatBase(format)) { }
	constexpr inline Format(FormatDepth format) noexcept :
		format(to_value(format)),
		base(compatibleFormatBase(format)) { }
	constexpr inline Format(FormatDepthStencil format) noexcept :
		format(to_value(format)),
		base(compatibleFormatBase(format)) { }
	constexpr inline Format(FormatStencil format) noexcept :
		format(to_value(format)),
		base(compatibleFormatBase(format)) { }
};

// ShaderType --------------------------------------------------------------------------------------

enum class ShaderType : GLenum {
	Vertex = 0x8B31, /// GL_VERTEX_SHADER
	Fragment = 0x8B30, /// GL_FRAGMENT_SHADER
	Geometry = 0x8DD9, /// GL_GEOMETRY_SHADER
	Compute = 0x91B9, /// GL_COMPUTE_SHADER
	TessControl = 0x8E88, /// GL_TESS_CONTROL_SHADER
	TessEvaluation = 0x8E87, /// GL_TESS_EVALUATION_SHADER
};

inline const char* to_string(ShaderType type) {
	switch (type) {
	case ShaderType::Vertex: return "vertex";
	case ShaderType::Geometry: return "geometry";
	case ShaderType::Fragment: return "fragment";
	case ShaderType::Compute: return "compute";
	case ShaderType::TessControl: return "tess-control";
	case ShaderType::TessEvaluation: return "tess-evaluation";
	}
	return "unknown";
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
	BYTE = 0x1400, /// GL_BYTE
	UBYTE = 0x1401, /// GL_UNSIGNED_BYTE
	SHORT = 0x1402, /// GL_SHORT
	USHORT = 0x1403, /// GL_UNSIGNED_SHORT
	INT = 0x1404, /// GL_INT
	UINT = 0x1405, /// GL_UNSIGNED_INT

	//Acceptable by glVertexAttribPointer:
	HFLOAT = 0x140B, /// GL_HALF_FLOAT
	FLOAT = 0x1406, /// GL_FLOAT
	FIXED = 0x140C, /// GL_FIXED
	INT_2_10_10_10_REV = 0x8D9F, /// GL_INT_2_10_10_10_REV
	UNSIGNED_INT_2_10_10_10_REV = 0x8368, /// GL_UNSIGNED_INT_2_10_10_10_REV
	UNSIGNED_INT_10F_11F_11F_REV = 0x8C3B, /// GL_UNSIGNED_INT_10F_11F_11F_REV

	//Acceptable by glVertexAttribPointer:
	//Acceptable by glVertexAttribLPointer:
	DOUBLE = 0x140A, /// GL_DOUBLE
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
	Back = 0x0405, /// GL_BACK
	Front = 0x0404, /// GL_FRONT
};

enum class PolygonMode : GLenum {
	Fill = 0x1B02, /// GL_FILL
	Line = 0x1B01, /// GL_LINE
	Point = 0x1B00, /// GL_POINT
};

enum class FrontFace : GLenum {
	CCW = 0x0901, /// GL_CCW
	CW = 0x0900, /// GL_CW
};

// -------------------------------------------------------------------------------------------------

enum class Attachment : GLenum {
	Color0 = 0x8CE0, /// GL_COLOR_ATTACHMENT0
	Color1 = 0x8CE1, /// GL_COLOR_ATTACHMENT1
	Color2 = 0x8CE2, /// GL_COLOR_ATTACHMENT2
	Color3 = 0x8CE3, /// GL_COLOR_ATTACHMENT3
	Color4 = 0x8CE4, /// GL_COLOR_ATTACHMENT4
	Color5 = 0x8CE5, /// GL_COLOR_ATTACHMENT5
	Color6 = 0x8CE6, /// GL_COLOR_ATTACHMENT6
	Color7 = 0x8CE7, /// GL_COLOR_ATTACHMENT7
	Color8 = 0x8CE8, /// GL_COLOR_ATTACHMENT8
	Color9 = 0x8CE9, /// GL_COLOR_ATTACHMENT9
	Color10 = 0x8CEA, /// GL_COLOR_ATTACHMENT10
	Color11 = 0x8CEB, /// GL_COLOR_ATTACHMENT11
	Color12 = 0x8CEC, /// GL_COLOR_ATTACHMENT12
	Color13 = 0x8CED, /// GL_COLOR_ATTACHMENT13
	Color14 = 0x8CEE, /// GL_COLOR_ATTACHMENT14
	Color15 = 0x8CEF, /// GL_COLOR_ATTACHMENT15
	Depth = 0x8D00, /// GL_DEPTH_ATTACHMENT
	DepthStencil = 0x821A, /// GL_DEPTH_STENCIL_ATTACHMENT
	Stencil = 0x8D20, /// GL_STENCIL_ATTACHMENT
};

enum class FramebufferStatus : GLenum {
	Complete = 0x8CD5, /// GL_FRAMEBUFFER_COMPLETE

	IncompleteAttachment = 0x8CD6, /// GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
	IncompleteDraw_buffer = 0x8CDB, /// GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
	IncompleteLayerTargets = 0x8DA8, /// GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
	IncompleteMissingAttachment = 0x8CD7, /// GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
	IncompleteMultisample = 0x8D56, /// GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
	IncompleteReadBuffer = 0x8CDC, /// GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
	Undefined = 0x8219, /// GL_FRAMEBUFFER_UNDEFINED
	Unsupported = 0x8CDD, /// GL_FRAMEBUFFER_UNSUPPORTED
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
