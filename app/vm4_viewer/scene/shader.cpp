// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/scene/shader.hpp>
// libv
#include <libv/utility/generic_path.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/gl/glsl_compiler.hpp>
// pro
#include <vm4_viewer/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

static libv::fsw::FileWatcher watcher; // SIGN-OFF: global variable - Vader

void BaseShader::update(const std::filesystem::path& file_path, bool is_vertex) {
	auto file_result = libv::read_file_ec(file_path);

	if (file_result.ec) {
		log_app.error("Failed to update {} shader: {} - {}: {}", is_vertex ? "vertex" : "fragment", libv::generic_path(file_path), file_result.ec, file_result.ec.message());
		return;
	}

	const auto include_loader = [&](const auto& path) {
		// TODO P2: store token for fsw subscribe_file every included file
//		const auto token = watcher.subscribe_file(path, [this, file_path, is_vertex](const libv::fsw::Event& e) {
//			log_app.trace("Updating {} shader: {}", "vertex", e);
//			this->update(file_path, is_vertex);
//			update_ptr(*this);
//		});

		auto include_result = libv::read_file_ec(path);

		if (include_result.ec) {
			log_app.error("Failed to include file: {} - {}: {}", path, include_result.ec, include_result.ec.message());
			return libv::gl::IncludeResult{false, ""};
		} else {
			return libv::gl::IncludeResult{true, std::move(include_result.data)};
		}
	};

	libv::gl::GLSLCompiler compiler(include_loader);
	const auto source = compiler.compile(file_result.data, libv::generic_path(file_path));

	if (is_vertex)
		this->vertex(source);
	else
		this->fragment(source);
}

BaseShader::BaseShader(const std::filesystem::path& vs_path, const std::filesystem::path& fs_path, update_signature func) {
	update_ptr = func;
	update(vs_path, true);
	update(fs_path, false);
	update_ptr(*this);

	// <<< P5: UI based file watcher that swaps threads and broadcasts the event in the appropriate loop stage
//	watcher_vs = watcher.subscribe_file(vs_path, [this, vs_path](const libv::fsw::Event& e) {
//		log_app.trace("Updating {} shader: {}", "vertex", e);
//		this->update(vs_path, true);
//		update_ptr(*this);
//	});
//	watcher_fs = watcher.subscribe_file(fs_path, [this, fs_path](const libv::fsw::Event& e) {
//		log_app.trace("Updating {} shader: {}", "fragment", e);
//		this->update(fs_path, false);
//		update_ptr(*this);
//	});
	watcher_fs = watcher.subscribe_directory(vs_path.parent_path(), [this, vs_path, fs_path](const libv::fsw::Event& e) {
		log_app.trace("Updating shader {}/{} action: {}", libv::generic_path(vs_path), libv::generic_path(fs_path), e);
		this->update(vs_path, true);
		this->update(fs_path, false);
		update_ptr(*this);
	});
}

BaseShader::~BaseShader() {
//	watcher.unsubscribe(watcher_vs);
//	watcher.unsubscribe(watcher_fs);
}

// -------------------------------------------------------------------------------------------------

} // namespace app
