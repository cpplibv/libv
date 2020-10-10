// Project: libv.ui, File: app/gen_ui_theme/engine.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/container/vector_2d.hpp>
#include <libv/fsw/watcher.hpp>
#include <libv/lua/lua.hpp>
#include <libv/thread/work_cooldown.hpp>
#include <libv/thread/worker_thread.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
// std
#include <filesystem>
#include <chrono>
#include <mutex>
// pro
#include <gen_ui_theme/effect.hpp>
#include <gen_ui_theme/theme.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

class Engine {
private:
	std::filesystem::path script_file;

private:
	libv::lua::State lua = libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::vec);
	std::function<void(libv::vector_2D<libv::vec4uc>, libv::vec2i)> callback;

private:
	std::mutex mutex;

private:
	libv::mt::worker_thread worker_thread{"lua-engine-worker"};
	libv::mt::work_cooldown load_cd{std::chrono::milliseconds{100}};
	libv::fsw::FileWatcher file_watcher;

	// -------------------------------------------------------------------------------------------------

public:
	explicit Engine(std::filesystem::path script_file, std::function<void(libv::vector_2D<libv::vec4uc>, libv::vec2i)> callback);

private:
//	void load_recipe(const sol::object& object);
	void init();

private:
	void load();
//	Atlas export_atlas();
};

// -------------------------------------------------------------------------------------------------

} // namespace app



