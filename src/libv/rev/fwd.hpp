// Project: libv.rev, File: src/libv/rev/fwd.hpp

#pragma once

// std
#include <cstdint>


namespace libv {
namespace rev {

namespace detail { // ------------------------------------------------------------------------------

class BaseUniformContainer;

//template <typename T>
//class UniformContainer;

} // namespace detail ------------------------------------------------------------------------------

// Model
class InternalModel;
class InternalModelLoader;
class Model;
class ModelLoader;

// Texture
class InternalTexture;
class InternalTextureLoader;
class Texture;
class TextureLoader;

// Material
class Material;
class MaterialLoader;
class MaterialScanner;

// Shader
class ShaderLoader;
class InternalShader;
class InternalShaderLoader;
class InternalShaderStage;
//class InternalIncludedSource;
class BaseShader;
template <typename T> class Shader;

using ShaderID = uint64_t;

// // Engine
// class Engine;
//
// // RenderPass
// class RenderPass;

// Other
class PostProcessing;
class RenderTarget;

// ResourceManager
class Settings;
class ResourceManager;
class InternalResourceManager;

} // namespace rev ---------------------------------------------------------------------------------
} // namespace libv
