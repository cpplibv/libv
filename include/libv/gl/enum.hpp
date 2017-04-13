// File:   enum.hpp Author: Vader Created on 2016. július 27., 4:55

#pragma once

// ext
#include <GL/glew.h>

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

enum class Buffer {
	Color = GL_COLOR_BUFFER_BIT,
	Depth = GL_DEPTH_BUFFER_BIT,
	Stencil = GL_STENCIL_BUFFER_BIT,
};
inline Buffer operator|(Buffer lhs, Buffer rhs) {
	return static_cast<Buffer> (static_cast<int> (lhs) | static_cast<int> (rhs));
}

enum class BufferUsage {
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

enum class BufferTarget {
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

enum class Capability {
	CullFace = GL_CULL_FACE,
	DepthTest = GL_DEPTH_TEST,
	RasterizerDiscard = GL_RASTERIZER_DISCARD,
	ScissorTest = GL_SCISSOR_TEST,
	StencilTest = GL_STENCIL_TEST,
	TextureCubeMapSeamless = GL_TEXTURE_CUBE_MAP_SEAMLESS,
};

enum class Primitive {
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

enum class StencilAction {
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

enum class CubeSide {
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

enum class TextureBindTarget {
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

//constexpr inline TextureBindTarget translateToBindTarget(TextureTarget1D target) {
//	switch (target) {
//	default:
//	case TextureTarget1D::_1D: return TextureBindTarget::_1D;
//	case TextureTarget1D::Proxy1D: return TextureBindTarget::_1D;
//	}
//}
//constexpr inline TextureBindTarget translateToBindTarget(TextureTarget2D target) {
//	switch (target) {
//	default:
//	case TextureTarget2D::_2D: return TextureBindTarget::_2D;
//	case TextureTarget2D::_1DArray: return TextureBindTarget::_1DArray;
//	case TextureTarget2D::Rectangle: return TextureBindTarget::Rectangle;
//	case TextureTarget2D::CubeMapPositiveX: return TextureBindTarget::CubeMap;
//	case TextureTarget2D::CubeMapNegativeX: return TextureBindTarget::CubeMap;
//	case TextureTarget2D::CubeMapPositiveY: return TextureBindTarget::CubeMap;
//	case TextureTarget2D::CubeMapNegativeY: return TextureBindTarget::CubeMap;
//	case TextureTarget2D::CubeMapPositiveZ: return TextureBindTarget::CubeMap;
//	case TextureTarget2D::CubeMapNegativeZ: return TextureBindTarget::CubeMap;
//	case TextureTarget2D::Proxy2D: return TextureBindTarget::_2D;
//	case TextureTarget2D::Proxy1DArray: return TextureBindTarget::_1DArray;
//	case TextureTarget2D::ProxyRectangle: return TextureBindTarget::Rectangle;
//	case TextureTarget2D::ProxyCubeMap: return TextureBindTarget::CubeMap;
//	}
//}
//constexpr inline TextureBindTarget translateToBindTarget(TextureTarget3D target) {
//	switch (target) {
//	default:
//	case TextureTarget3D::_3D: return TextureBindTarget::_3D;
//	case TextureTarget3D::_2DArray: return TextureBindTarget::_2DArray;
//	case TextureTarget3D::Proxy3D: return TextureBindTarget::_3D;
//	case TextureTarget3D::Proxy2DArray: return TextureBindTarget::_2DArray;
//	}
//}

// -------------------------------------------------------------------------------------------------

enum class DepthStencilMode {
	DepthComponent = GL_DEPTH_COMPONENT,
	StencilIndex = GL_STENCIL_INDEX,
};

enum class CompareMode {
	None = GL_NONE,
	CompareRefToTexture = GL_COMPARE_REF_TO_TEXTURE,
};

enum class TestFunction {
	LEqual = GL_LEQUAL,
	GEqual = GL_GEQUAL,
	Less = GL_LESS,
	Greater = GL_GREATER,
	Equal = GL_EQUAL,
	NotEqual = GL_NOTEQUAL,
	Always = GL_ALWAYS,
	Never = GL_NEVER,
};

enum class MagFilter {
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
};

enum class MinFilter {
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
	NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
};

enum class Swizzle {
	Red = GL_RED,
	Green = GL_GREEN,
	Blue = GL_BLUE,
	Alpha = GL_ALPHA,
	Zero = GL_ZERO,
	One = GL_ONE,
};

enum class Wrap {
	ClampToEdge = GL_CLAMP_TO_EDGE,
	Repeat = GL_REPEAT,
	ClampToBorder = GL_CLAMP_TO_BORDER,
	MirroredRepeat = GL_MIRRORED_REPEAT,
	MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE,
};

enum class InternalFormat {
	Alpha = GL_ALPHA,
	Alpha4 = GL_ALPHA4,
	Alpha8 = GL_ALPHA8,
	Alpha12 = GL_ALPHA12,
	Alpha16 = GL_ALPHA16,
	CompressedAlpha = GL_COMPRESSED_ALPHA,
	CompressedLuminance = GL_COMPRESSED_LUMINANCE,
	CompressedLuminanceAlpha = GL_COMPRESSED_LUMINANCE_ALPHA,
	CompressedIntensity = GL_COMPRESSED_INTENSITY,
	CompressedRGB = GL_COMPRESSED_RGB,
	CompressedRGBA = GL_COMPRESSED_RGBA,
	DepthComponent = GL_DEPTH_COMPONENT,
	DepthComponent16 = GL_DEPTH_COMPONENT16,
	DepthComponent24 = GL_DEPTH_COMPONENT24,
	DepthComponent32 = GL_DEPTH_COMPONENT32,
	Luminance = GL_LUMINANCE,
	Luminance4 = GL_LUMINANCE4,
	Luminance8 = GL_LUMINANCE8,
	Luminance12 = GL_LUMINANCE12,
	Luminance16 = GL_LUMINANCE16,
	LuminanceAlpha = GL_LUMINANCE_ALPHA,
	Luminance4Alpha4 = GL_LUMINANCE4_ALPHA4,
	Luminance6Alpha2 = GL_LUMINANCE6_ALPHA2,
	Luminance8Alpha8 = GL_LUMINANCE8_ALPHA8,
	Luminance12Alpha4 = GL_LUMINANCE12_ALPHA4,
	Luminance12Alpha12 = GL_LUMINANCE12_ALPHA12,
	Luminance16Alpha16 = GL_LUMINANCE16_ALPHA16,
	Intensity = GL_INTENSITY,
	Intensity4 = GL_INTENSITY4,
	Intensity8 = GL_INTENSITY8,
	Intensity12 = GL_INTENSITY12,
	Intensity16 = GL_INTENSITY16,
	R3_G3_B2 = GL_R3_G3_B2,
	RGB = GL_RGB,
	RGB4 = GL_RGB4,
	RGB5 = GL_RGB5,
	RGB8 = GL_RGB8,
	RGB10 = GL_RGB10,
	RGB12 = GL_RGB12,
	RGB16 = GL_RGB16,
	RGBA = GL_RGBA,
	RGBA2 = GL_RGBA2,
	RGBA4 = GL_RGBA4,
	RGB5_A1 = GL_RGB5_A1,
	RGBA8 = GL_RGBA8,
	RGB10_A2 = GL_RGB10_A2,
	RGBA12 = GL_RGBA12,
	RGBA16 = GL_RGBA16,
	Sluminance = GL_SLUMINANCE,
	Sluminance8 = GL_SLUMINANCE8,
	SluminanceAlpha = GL_SLUMINANCE_ALPHA,
	Sluminance8Alpha8 = GL_SLUMINANCE8_ALPHA8,
	SRGB = GL_SRGB,
	SRGB8 = GL_SRGB8,
	SRGBAlpha = GL_SRGB_ALPHA,
	SRGB8Alpha8 = GL_SRGB8_ALPHA8,
};

enum class Format {
	RED = GL_RED,
	RG = GL_RG,
	RGB = GL_RGB,
	BGR = GL_BGR,
	RGBA = GL_RGBA,
	BGRA = GL_BGRA,
	REDInteger = GL_RED_INTEGER,
	RGInteger = GL_RG_INTEGER,
	RGBInteger = GL_RGB_INTEGER,
	BGRInteger = GL_BGR_INTEGER,
	RGBAInteger = GL_RGBA_INTEGER,
	BGRAInteger = GL_BGRA_INTEGER,
	StencilIndex = GL_STENCIL_INDEX,
	DepthComponent = GL_DEPTH_COMPONENT,
	DepthStencil = GL_DEPTH_STENCIL,

	//	Red = GL_RED,
	//	RGB = GL_RGB,
	//	BGR = GL_BGR,
	//	RGBA = GL_RGBA,
	//	BGRA = GL_BGRA
};

enum class DataType {
	UnsignedByte = GL_UNSIGNED_BYTE,
	Byte = GL_BYTE,
	Bitmap = GL_BITMAP,
	UnsignedShort = GL_UNSIGNED_SHORT,
	Short = GL_SHORT,
	UnsignedInt = GL_UNSIGNED_INT,
	Int = GL_INT,
	Float = GL_FLOAT,
	UnsignedByte_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
	UnsignedByte_2_3_3_rev = GL_UNSIGNED_BYTE_2_3_3_REV,
	UnsignedShort_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
	UnsignedShort_5_6_5_rev = GL_UNSIGNED_SHORT_5_6_5_REV,
	UnsignedShort_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
	UnsignedShort_4_4_4_4_rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
	UnsignedShort_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
	UnsignedShort_1_5_5_5_rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
	UnsignedInt_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
	UnsignedInt_8_8_8_8_rev = GL_UNSIGNED_INT_8_8_8_8_REV,
	UnsignedInt_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
	UnsignedInt_2_10_10_10_rev = GL_UNSIGNED_INT_2_10_10_10_REV,
};

enum class Filter {
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
	LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
};

// ShaderType --------------------------------------------------------------------------------------

enum class ShaderType {
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	Compute = GL_COMPUTE_SHADER,
	TessControl = GL_TESS_CONTROL_SHADER,
	TessEvaluation = GL_TESS_EVALUATION_SHADER,
};

constexpr char SHADER_TYPE_VERTEX_NAME[] = "vertex";
constexpr char SHADER_TYPE_GEOMETRY_NAME[] = "geometry";
constexpr char SHADER_TYPE_FRAGMENT_NAME[] = "fragment";
constexpr char SHADER_TYPE_COMPUTE_NAME[] = "compute";
constexpr char SHADER_TYPE_TESS_CONTROL_NAME[] = "tesscontrol";
constexpr char SHADER_TYPE_TESS_EVALUATION_NAME[] = "tessevaluation";

inline const char* to_string(ShaderType type) {
	switch (type) {
	default:
	case ShaderType::Vertex: return SHADER_TYPE_VERTEX_NAME;
	case ShaderType::Geometry: return SHADER_TYPE_GEOMETRY_NAME;
	case ShaderType::Fragment: return SHADER_TYPE_FRAGMENT_NAME;
	case ShaderType::Compute: return SHADER_TYPE_COMPUTE_NAME;
	case ShaderType::TessControl: return SHADER_TYPE_TESS_CONTROL_NAME;
	case ShaderType::TessEvaluation: return SHADER_TYPE_TESS_EVALUATION_NAME;
	}
}

// TextureType -------------------------------------------------------------------------------------

enum class TextureChannel {
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

enum class AttributeType {
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

// -------------------------------------------------------------------------------------------------

enum class Face {
	Back = GL_BACK,
	Front = GL_FRONT,
	FrontAndBack = GL_FRONT_AND_BACK,
};

enum class Mode {
	Fill = GL_FILL,
	Line = GL_LINE,
	Point = GL_POINT,
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv