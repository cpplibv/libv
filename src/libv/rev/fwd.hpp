// Project: libv, File: app/space/shader.hpp, Author: Császár Mátyás [Vader]

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

class InternalShader;
class InternalShaderLoader;
class InternalShaderStage;
//class InternalIncludedSource;

class ShaderLoader;

class BaseShader;
template <typename T> class Shader;

using ShaderID = uint64_t;

} // namespace rev ---------------------------------------------------------------------------------
} // namespace libv
