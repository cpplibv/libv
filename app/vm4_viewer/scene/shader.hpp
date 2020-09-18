// Project: libv, File: app/vm4_viewer/scene/shader.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/container/flat_set.hpp>
#include <boost/container/small_vector.hpp>
// libv
#include <libv/glr/program.hpp>
#include <libv/glr/uniform.hpp>
// std
#include <string>


namespace app {

// -------------------------------------------------------------------------------------------------

class ShaderLoader;

struct ShaderStage {
	libv::gl::ShaderType type;
	std::string path;

	inline ShaderStage(libv::gl::ShaderType type, std::string path) :
		type(type),
		path(std::move(path)) { }
};

// ---

class BaseShader {
	template <typename Uniforms>
	friend class Shader;

	using update_signature = void (*)(BaseShader&);

protected:
public: // TODO P0: public
	libv::glr::Program program;

private:
	ShaderLoader& loader;
public: // TODO P0: public
	update_signature update_ptr;

public: // TODO P0: public
	boost::container::small_vector<ShaderStage, 3> stages;
	boost::container::flat_set<std::string> source_files;

public:
	BaseShader(const BaseShader&) = delete;
	BaseShader& operator=(const BaseShader&) & = delete;
	BaseShader(BaseShader&&) = delete;
	BaseShader& operator=(BaseShader&&) & = delete;

	BaseShader(ShaderLoader& loader, update_signature update_ptr);
	~BaseShader();

public:
	void addStage(libv::gl::ShaderType type, std::string path);
	std::string name() const;
};

// ---

template <typename Uniforms>
struct Shader : BaseShader {
	Uniforms uniform;

private:
	template <typename... Args>
	inline void unpackStages(libv::gl::ShaderType type, std::string path, Args&&... args) {
		addStage(type, std::move(path));

		if constexpr (sizeof...(Args) != 0)
			unpackStages(std::forward<Args>(args)...);
	}

private:
	static void updater(BaseShader& self) {
		auto access = [&self](auto&&... args) mutable {
			self.program.assign(args...);
		};
		static_cast<Shader<Uniforms>&>(self).uniform.update_uniforms(access);
	}

public:
	template <typename... Args>
	inline Shader(ShaderLoader& loader, Args&&... args) :
		BaseShader(loader, updater) {
		unpackStages(std::forward<Args>(args)...);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
