// Project: libv, File: app/space/shader.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/glr/program.hpp>
// std
#include <memory>
#include <string>
#include <typeindex>
// pro
#include <libv/rev/base_shader.hpp>


namespace libv {
namespace rev {
namespace detail {

// -------------------------------------------------------------------------------------------------

template <typename Uniforms>
class UniformContainer final : public BaseUniformContainer {
public:
	Uniforms uniforms;

public:
	virtual void bind_assign(libv::glr::Program& program) override {
		auto access = [&program](auto&&... args) {
			program.assign(args...);
		};
		uniforms.access_uniforms(access);
	}
};

} // namespace detail ------------------------------------------------------------------------------

// This class will most likely be replaces with a 'Material' construct of some way in libv.rev
template <typename Uniforms>
class Shader final : public BaseShader {
private:
	static std::unique_ptr<detail::BaseUniformContainer> ucc() {
		return std::make_unique<detail::UniformContainer<Uniforms>>();
	}

public:
	Shader(ShaderLoader& loader, std::string path_vs, std::string path_fs) :
		Shader(loader,
				libv::gl::ShaderType::Vertex, std::move(path_vs),
				libv::gl::ShaderType::Fragment, std::move(path_fs)) {
	}

	Shader(ShaderLoader& loader, std::string path_vs, std::string path_gs, std::string path_fs) :
		Shader(loader,
				libv::gl::ShaderType::Vertex, std::move(path_vs),
				libv::gl::ShaderType::Geometry, std::move(path_gs),
				libv::gl::ShaderType::Fragment, std::move(path_fs)) {
	}

	Shader(ShaderLoader& loader, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1) :
		BaseShader(loader, std::type_index(typeid(Uniforms)), ucc, type0, std::move(path0), type1, std::move(path1)) {
	}

	Shader(ShaderLoader& loader, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2) :
		BaseShader(loader, std::type_index(typeid(Uniforms)), ucc, type0, std::move(path0), type1, std::move(path1), type2, std::move(path2)) {
	}

	inline Shader(const Shader&) noexcept = default;
	inline Shader& operator=(const Shader&) & noexcept = default;
	inline Shader(Shader&&) noexcept = default;
	inline Shader& operator=(Shader&&) & noexcept = default;

	~Shader() {
	}

public:
	[[nodiscard]] inline Uniforms& uniform() noexcept {
		return static_cast<detail::UniformContainer<Uniforms>&>(base_uniform()).uniforms;
	}

	[[nodiscard]] inline const Uniforms& uniform() const noexcept {
		return static_cast<const detail::UniformContainer<Uniforms>&>(base_uniform()).uniforms;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv