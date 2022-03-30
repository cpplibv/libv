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
//class InternalModelLoader;
class ModelLoader;
//class InternalModel;
class Model;

// Texture
class InternalTextureLoader;
class TextureLoader;
class InternalTexture;
class Texture;

// Material
class MaterialLoader;
class ModelMaterial;
//class Material;

// Shader
class ShaderLoader;
class InternalShader;
class InternalShaderLoader;
class InternalShaderStage;
//class InternalIncludedSource;
class BaseShader;
template <typename T> class Shader;

using ShaderID = uint64_t;

// Other
class PostProcessing;
class RenderTarget;

class InternalResourceManager;

} // namespace rev ---------------------------------------------------------------------------------
} // namespace libv
