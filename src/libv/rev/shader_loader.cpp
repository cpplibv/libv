// Project: libv, File: app/space/shader_loader.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/rev/shader_loader.hpp>
// ext
#include <boost/container/flat_set.hpp>
// libv
#include <libv/fsw/watcher.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/glsl_compiler.hpp>
#include <libv/gl/program.hpp>
#include <libv/utility/concat.hpp>
#include <libv/utility/generic_path.hpp>
#include <libv/utility/read_file.hpp>
// std
#include <filesystem>
#include <map>
#include <mutex>
// pro
#include <libv/rev/shader.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

//struct FileIncludeTrackingLoader {
//};

//struct FileIncludeLoader {
//};

struct FileIncluder {
	std::filesystem::path base_directory = "app/vm4_viewer/res/shader/";
	libv::observer_ptr<boost::container::flat_set<std::string>> current_sources;

public:
	libv::gl::IncludeResult operator()(const std::string_view include_path) {
		const auto file_path = base_directory / include_path;

		// TODO P5: normalized_generic_path
		current_sources->emplace(libv::generic_path(file_path));
		auto read_result = libv::read_file_ec(file_path);

		if (read_result.ec)
			// TODO P5: report failed include information
			// TODO P5: report search path with failed include information
			return {false, libv::concat(read_result.ec, ": ", read_result.ec.message())};
		else
			return {true, std::move(read_result.data)};
	}
};

// -------------------------------------------------------------------------------------------------

class ImplShaderLoader {
	FileIncluder includer;
//	libv::gl::GLSLCompiler compiler;
	libv::fsw::Watcher watcher;

	std::mutex update_queue_m;
	boost::container::flat_set<libv::observer_ref<BaseShader>> update_queue;
	std::map<libv::observer_ref<BaseShader>, std::vector<libv::fsw::WatchToken>> tokens;

//	libv::gl::IncludeResult include(const std::string_view include_path);
//
//	ImplShaderLoader() :
//		compiler(std::ref(includer)) { }
};

// -------------------------------------------------------------------------------------------------

//Shader ShaderLoader::load(std::filesystem::path vs, std::filesystem::path fs) {
//	load(libv::gl::ShaderType::Vertex, std::move(vs), libv::gl::ShaderType::Fragment, std::move(fs));
//}
//
//Shader ShaderLoader::load(std::filesystem::path vs, std::filesystem::path gs, std::filesystem::path fs) {
//	load(libv::gl::ShaderType::Vertex, std::move(vs), libv::gl::ShaderType::Geometry, std::move(gs), libv::gl::ShaderType::Fragment, std::move(fs));
//}

BaseShader ShaderLoader::aux_lookup(std::type_index uniforms_type_index, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1) {

}

BaseShader ShaderLoader::aux_lookup(std::type_index uniforms_type_index, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2) {

}

//void ShaderLoader::subscribe(const libv::observer_ref<BaseShader> shader) {
//	std::unique_lock lock(self->update_queue_m);
//
//	self->update_queue.emplace(shader);
//}
//
//void ShaderLoader::unsubscribe(const libv::observer_ref<BaseShader> shader) {
//	std::unique_lock lock(self->update_queue_m);
//
//	self->update_queue.erase(shader);
//
//	auto& tokens = self->tokens[shader];
//	for (const auto& token : tokens)
//		self->watcher.unsubscribe(token);
//
//	tokens.clear();
//}

void ShaderLoader::update(libv::gl::GL& gl, success_cb_type success_cb, failure_cb_type failure_cb) {
	std::unique_lock lock(self->update_queue_m);

	auto previous_program = gl.bound_program();

	for (const auto& update : self->update_queue) {
		self->includer.current_sources = &update->source_files;

		libv::gl::Program program;
		gl(program).create();

		bool shader_compile_failed = false;
		std::string shader_compile_error;

		for (const auto& stage : update->stages) {
			// TODO P1: Load source "here", but "not here".
			//          Delaying source loading (until update) is a good idea
			//          But FS calls between gl calls are not that sexy
			const auto source = self->compiler.load(stage.path);

			libv::gl::Shader shader;

			gl(shader).create(stage.type);
			gl(shader).compile(source);

			if (!gl(shader).status()) {
				shader_compile_failed = true;
				shader_compile_error = gl(shader).info();
				break;
			}
			gl(program).attach(shader);
			gl(shader).destroy();
		}

		if (!shader_compile_failed)
			gl(program).link();

		if (shader_compile_failed) {
			failure_cb(ShaderLoadFailure{*update, shader_compile_error});
		} else if (!gl(program).status()) {
			failure_cb(ShaderLoadFailure{*update, gl(program).info()});
		} else {
			update->program._native_swap(program);
			(*update->update_ptr)(*update);
			success_cb(ShaderLoadSuccess{*update});
		}

		if (program.id != 0)
			// Destroy the freshly failed or the previously used program object
			gl(program).destroy();

		{
			// TODO P1: Handle watcher changes the moment a file is included (even before the read operation)
			//          And not here
			//          | This also kills requirement to store update->source_files
			// Cleanup Watchers - Add new ones first, remove old one second
			auto& tokens = self->tokens[update];
			const auto old_tokens = std::move(tokens);
			tokens.clear();

			for (const auto& source_file : update->source_files)
				tokens.emplace_back(self->watcher.subscribe_file(source_file, [this, update](libv::fsw::Event) {
					std::unique_lock lock(self->update_queue_m);
					self->update_queue.emplace(update);
				}));

			for (const auto& token : old_tokens)
				self->watcher.unsubscribe(token);
		}
	}

	// Cleanup
	self->update_queue.clear();
	self->includer.current_sources = nullptr;
	gl(previous_program).use();
}

// -------------------------------------------------------------------------------------------------

ShaderLoader::ShaderLoader(std::filesystem::path base_include_directory) :
	self(std::make_unique<ImplShaderLoader>()) {
}

ShaderLoader::~ShaderLoader() {
	// For the sake of forward declared unique_ptr
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
