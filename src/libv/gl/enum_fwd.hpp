// Project: libv.gl, File: src/libv/gl/enum_fwd.hpp

#pragma once

// std
#include <cstdint>


// -------------------------------------------------------------------------------------------------

using GLenum = uint32_t;
using GLint = int32_t;

// -------------------------------------------------------------------------------------------------

namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

enum class BarrierBit : GLenum;
enum class BufferBit : GLenum;
enum class BufferAccess : GLenum;
enum class BufferAccessFull : GLenum;
enum class BufferUsage : GLenum;
enum class BufferTarget : GLenum;
enum class Capability : GLenum;
enum class Primitive : GLenum;
enum class StencilAction : GLenum;
enum class CubeSide : GLenum;
enum class TextureTarget : GLenum;
enum class DepthStencilMode : GLenum;
enum class CompareMode : GLenum;
enum class TestFunction : GLenum;
enum class BlendEquation : GLenum;
enum class BlendFunction : GLenum;
enum class MagFilter : GLenum;
enum class MinFilter : GLenum;
enum class Swizzle : GLenum;
enum class Wrap : GLenum;
enum class Filter : GLenum;
enum class DataType : GLenum;
enum class ReadFormat : GLenum;
enum class FormatBase : GLenum;
enum class FormatCompressed : GLenum;
enum class FormatDepth : GLenum;
enum class FormatDepthStencil : GLenum;
enum class FormatStencil : GLenum;
enum class FormatSized : GLenum;
enum class ShaderType : GLenum;
enum class TextureUnit : GLint;
enum class AttributeType : GLenum;
enum class CullFace : GLenum;
enum class PolygonMode : GLenum;
enum class FrontFace : GLenum;
enum class Attachment : GLenum;
enum class FramebufferStatus : GLenum;
enum class QueryType : GLenum;

struct Format;

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
