// Project: libv.re, File: src/libv/re/fwd.hpp

#pragma once

#include <cstdint>
#include <libv/utility/memory/intrusive2_ptr.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

#define LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(TYPE) \
	class TYPE; \
	void increase_ref_count(TYPE* ptr); \
	void decrease_ref_count(TYPE* ptr); \
	using TYPE ## _ptr = libv::intrusive2_ptr<TYPE>; \
	using TYPE ## _cptr = libv::intrusive2_ptr<const TYPE>; \
	using TYPE ## _ref = libv::intrusive2_ref<TYPE>; \
	using TYPE ## _cref = libv::intrusive2_ref<const TYPE>;

// Library
class Settings;
class RenderLibrary;

// Core
class DestroyQueue;
class Engine;
class RenderPass;

// Core - Context
class ContextFrame;
class ContextFrameGL;
class ContextPipeline;
class ContextQueue;
class ContextQueueOverride;
class ContextRender;
class ContextUpdate;

// Core - OpenGL handlers
template <typename T>
class UniformBufferVector;
class UniformBufferMapped;
class ReadbackQueryIndex;
class ReadbackBufferMapped;
class IndirectCommandBuffer;
class IndirectCommandBufferMapped;
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(RenderTarget)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(RenderTargetDefault)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(RenderTargetTextures)

// Resources
class ResourceAccess;
class BasicResource;

LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Shader)
using ShaderID = uint64_t;
class ShaderLoader;
class ImplShaderLoader;
struct ShaderLoadSuccess;
struct ShaderLoadFailure;
struct ShaderUnload;

LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Texture)
using TextureID = uint64_t;
class TextureLoader;
class ImplTextureLoader;
struct TextureLoadSuccess;
struct TextureLoadFailure;
struct TextureUnload;

class MaterialLoader;
class ImplMaterialLoader;
class MaterialScanner;

LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Model)
using ModelID = uint64_t;
class ModelLoader;
class ImplModelLoader;
struct ModelLoadSuccess;
struct ModelLoadFailure;
struct ModelUnload;

// Uniform
struct UniformBlockBufferPlacement;

// Uniform blocks / structs
struct BlockModelMatrices;
struct BlockRenderPass;
struct UniformLight;

// Common
enum class BlockBinding : uint8_t;
enum class DebugCaptureMode : int32_t;
enum class NodeType : uint8_t;
enum class SubObjectIndex : uint32_t;

// Math
struct BoundingSphere;
struct Transform;

// -------------------------------------------------------------------------------------------------

// Passes
class PostProcess;
// LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(PostProcess)

// -------------------------------------------------------------------------------------------------

// Core components
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Canvas)
class SceneInternal;
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Scene)

// Nodes / Objects
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Node)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Proxy)

LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Camera) // TBD if object, currently not one
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(EditorGrid)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(LensFlare)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Light)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(LightDirectional)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(LightPoint)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(LightSpot)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(ModelNode)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(ModelStatic)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Object)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Outline)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Skybox)

// Materials
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Material)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialDebugPassCapture)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialFullscreen)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialJFAOutline)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialJFAStep)
struct UniformBlockLensFlare;
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialLensFlare)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialOutlineMask)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialSkybox)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialSolid)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialSolidModel)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MaterialTest0)

// Meshes
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(Mesh)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MeshCube)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MeshFullscreen)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MeshModelStatic)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MeshPlane)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MeshSkybox)
LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR(MeshSphere)

// -------------------------------------------------------------------------------------------------

// It's a really useful macro
// #undef LIBV_FWD_DECLARE_WITH_INTRUSIVE_PTR

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
