// Project: libv.rev, File: src/libv/rev/resource/material_scanner.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/vm4/fwd.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/rev/resource/shader.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class MaterialScanner {
private:
	ShaderLoader& shaderLoader;
	TextureLoader& textureLoader;
	const libv::vm4::Material& vm4Material;

public:
	inline MaterialScanner(ShaderLoader& shaderLoader, TextureLoader& textureLoader, const libv::vm4::Material& vm4Material) noexcept :
			shaderLoader(shaderLoader),
			textureLoader(textureLoader),
			vm4Material(vm4Material) {}

public:
	template <typename T>
	[[nodiscard]] inline Shader<T> shader(std::string vs, std::string fs) const;
	template <typename T>
	[[nodiscard]] inline Shader<T> shader(std::string vs, std::string gs, std::string fs) const;

	[[nodiscard]] Texture texture_or(libv::vm4::TextureType type, std::string_view fallback) const;
	[[nodiscard]] Texture texture_or(libv::vm4::TextureType type, Texture fallback) const;

	[[nodiscard]] libv::vec4f vec4f_or(std::string_view name, libv::vec4f fallback) const;

	[[nodiscard]] float float_or(std::string_view name, float fallback) const;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
inline Shader<T> MaterialScanner::shader(std::string vs, std::string fs) const {
	return Shader<T>{shaderLoader, std::move(vs), std::move(fs)};
}

template <typename T>
inline Shader<T> MaterialScanner::shader(std::string vs, std::string gs, std::string fs) const {
	return Shader<T>{shaderLoader, std::move(vs), std::move(gs), std::move(fs)};
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
