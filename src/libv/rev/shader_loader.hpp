// Project: libv, File: app/space/shader_loader.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/gl/gl_fwd.hpp>
#include <libv/glr/program.hpp>
#include <libv/utility/function_ref.hpp>
//#include <libv/utility/observer_ref.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
#include <typeindex>
#include <vector>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class BaseShader;
template <typename T>
class Shader;

// -------------------------------------------------------------------------------------------------

struct ShaderLoadSuccess {
	Shader& shader;
	// What triggered the (re)load (first reason)
};

struct ShaderLoadFailure {
	Shader& shader;
	std::string reason;
	// What triggered the (re)load (first reason)
};

// -------------------------------------------------------------------------------------------------

class ShaderLoader {
public:
	using success_cb_type = libv::function_ref<void(const ShaderLoadSuccess&)>;
	using failure_cb_type = libv::function_ref<void(const ShaderLoadFailure&)>;
	using foreach_cb_type = libv::function_ref<void(BaseShader&)>;

private:
	std::unique_ptr<class ImplShaderLoader> self;

public:
	explicit ShaderLoader(std::filesystem::path base_include_directory);
	~ShaderLoader();

private:
	[[nodiscard]] BaseShader aux_lookup(std::type_index uniforms_type_index, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1);
	[[nodiscard]] BaseShader aux_lookup(std::type_index uniforms_type_index, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2);

public:
	template <typename T>
	[[nodiscard]] Shader<T> load(std::string path_vs, std::string path_fs);
	template <typename T>
	[[nodiscard]] Shader<T> load(std::string path_vs, std::string path_gs, std::string path_fs);
	template <typename T>
	[[nodiscard]] Shader<T> load(libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1);
	template <typename T>
	[[nodiscard]] Shader<T> load(libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2);

public:
	void foreach_shaders(foreach_cb_type func);

public:
	void update(libv::gl::GL& gl, success_cb_type success_cb, failure_cb_type failure_cb);
};

// -------------------------------------------------------------------------------------------------

template <typename T>
Shader<T> ShaderLoader::load(std::string path_vs, std::string path_fs) {
	return load<T>(
			libv::gl::ShaderType::Vertex, std::move(path_vs),
			libv::gl::ShaderType::Fragment, std::move(path_fs));
}

template <typename T>
Shader<T> ShaderLoader::load(std::string path_vs, std::string path_gs, std::string path_fs) {
	return load<T>(
			libv::gl::ShaderType::Vertex, std::move(path_vs),
			libv::gl::ShaderType::Geometry, std::move(path_gs),
			libv::gl::ShaderType::Fragment, std::move(path_fs));
}

template <typename T>
Shader<T> ShaderLoader::load(libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1) {
	static constexpr auto uniforms_type_index = std::type_index(typeid(T));
	BaseShader shader = aux_lookup(uniforms_type_index, type0, std::move(path0), type1, std::move(path1));

	if (!shader)
		shader.init_uniforms<T>();

	return Shader<T>(shader);
}

template <typename T>
Shader<T> ShaderLoader::load(libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2) {
	static constexpr auto uniforms_type_index = std::type_index(typeid(T));
	BaseShader shader = aux_lookup(uniforms_type_index, type0, std::move(path0), type1, std::move(path1), type2, std::move(path2));

	if (!shader)
		shader.init_uniforms<T>();

	return Shader<T>(shader);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
