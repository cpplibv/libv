// Project: libv, File: app/space/shader.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
//#include <boost/container/flat_set.hpp>
//#include <boost/container/small_vector.hpp>
// libv
#include <libv/glr/program.hpp>
#include <libv/glr/uniform.hpp>
// std
#include <string>
#include <libv/fsw/token.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class InternalShader;
class ShaderLoader;

namespace detail { // ------------------------------------------------------------------------------

class BaseUniformContainer {
public:
	virtual void assign(libv::glr::Program& program) = 0;
	virtual ~BaseUniformContainer() = default;
};

template <typename T>
class UniformContainer final : public BaseUniformContainer {
	T uniforms;

public:
	virtual void assign(libv::glr::Program& program) override {
		auto access = [&program](auto&&... args) {
			program.assign(args...);
		};
		uniforms.update_uniforms(access);
	}
};

} // namespace detail ------------------------------------------------------------------------------

class BaseShader {
private:
	std::shared_ptr<InternalShader> internal;

public:
	BaseShader(ShaderLoader& loader, std::unique_ptr<detail::BaseUniformContainer>(*uniformContainerCreator)(), libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1);
	BaseShader(ShaderLoader& loader, std::unique_ptr<detail::BaseUniformContainer>(*uniformContainerCreator)(), libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2);
	~BaseShader();

//public:
//	void add_stage(libv::gl::ShaderType type, std::string path);

public:
	// introspection stuff comes here

//	void stages();
	[[nodiscard]] std::string name() const;

	[[nodiscard]] libv::glr::Program& program();
	[[nodiscard]] const libv::glr::Program& program() const;

//public:
//	void update() = 0;
};

// -------------------------------------------------------------------------------------------------

// This class will most likely be replaces with a 'Material' construct of some sort in libv.rev
template <typename Uniforms>
class Shader final : public BaseShader {

private:
	static std::unique_ptr<detail::BaseUniformContainer> uniform_container_creator() {
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
		BaseShader(loader, uniform_container_creator, type0, std::move(path0), type1, std::move(path1)) {
	}

	Shader(ShaderLoader& loader, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2) :
		BaseShader(loader, uniform_container_creator, type0, std::move(path0), type1, std::move(path1), type2, std::move(path2)) {
	}

	inline Shader(const Shader&) noexcept = default;
	inline Shader& operator=(const Shader&) & noexcept = default;
	inline Shader(Shader&&) noexcept = default;
	inline Shader& operator=(Shader&&) & noexcept = default;

	~Shader() {
	}

public:
	[[nodiscard]] inline Uniforms& uniform() {
		return static_cast<Uniforms&>(base_uniform());
	}

	[[nodiscard]] inline const Uniforms& uniform() const {
		return static_cast<const Uniforms&>(base_uniform());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
