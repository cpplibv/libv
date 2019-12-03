// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/glr/program.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/fsw/watcher.hpp>
// std
#include <filesystem>


namespace app {

// -------------------------------------------------------------------------------------------------

class BaseShader : public libv::glr::Program {
	libv::fsw::FileWatcher::token_type watcher_vs;
	libv::fsw::FileWatcher::token_type watcher_fs;

	using update_signature = void (*)(BaseShader&);
	update_signature update_ptr;

public:
	void update(const std::filesystem::path& file_path, bool is_vertex);

public:
	BaseShader(const std::filesystem::path& vs_path, const std::filesystem::path& fs_path, update_signature func);
	~BaseShader();
};

// -------------------------------------------------------------------------------------------------

template <typename Uniforms>
struct Shader : public Uniforms, public BaseShader {
	Shader(const std::filesystem::path& vs_path, const std::filesystem::path& fs_path) :
		BaseShader(vs_path, fs_path, [](auto& self) {
//			auto access = [&self](auto& uniform, const auto& name) mutable {
//				self.assign(uniform, name);
//			};
			auto access = [&self](auto&&... args) mutable {
				self.assign(args...);
			};
			static_cast<Uniforms&>(static_cast<Shader&>(self)).update_uniforms(access);
		})
	{}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
