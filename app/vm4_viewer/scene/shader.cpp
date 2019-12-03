// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/scene/shader.hpp>
// libv
#include <libv/utility/generic_path.hpp>
#include <libv/utility/read_file.hpp>
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

	if (is_vertex)
		this->vertex(file_result.data);
	else
		this->fragment(file_result.data);
}

BaseShader::BaseShader(const std::filesystem::path& vs_path, const std::filesystem::path& fs_path, update_signature func) {
	update_ptr = func;
	update(vs_path, true);
	update(fs_path, false);
	update_ptr(*this);

	// <<< P1: UI based file watcher that swaps threads and broadcasts the event in the appropriate loop stage
	watcher_vs = watcher.subscribe_file(vs_path, [this, vs_path](const libv::fsw::Event& e) {
		log_app.trace("Updating {} shader: {}", "vertex", e);
		this->update(vs_path, true);
		update_ptr(*this);
	});
	watcher_fs = watcher.subscribe_file(fs_path, [this, fs_path](const libv::fsw::Event& e) {
		log_app.trace("Updating {} shader: {}", "fragment", e);
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
