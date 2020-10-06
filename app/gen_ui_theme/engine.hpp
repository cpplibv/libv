// Project: libv.ui, File: app/gen_ui_theme/engine.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
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
	std::filesystem::path script_file;

	std::vector<std::unique_ptr<app::Effect>> effects;

	libv::lua::State lua = libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::vec);
	std::function<void(std::vector<libv::vec4uc>, libv::vec2i)> callback;
	std::mutex mutex;

	libv::mt::worker_thread worker_thread{"lua-engine-worker"};
	libv::mt::work_cooldown load_cd{std::chrono::milliseconds{100}};

	libv::fsw::FileWatcher file_watcher;

	// -------------------------------------------------------------------------------------------------

public:
	explicit Engine(std::filesystem::path script_file, std::function<void(std::vector<libv::vec4uc>, libv::vec2i)> callback) :
		script_file(std::move(script_file)),
		callback(std::move(callback)) {

		init();

		load_cd.execute_async([this] { load(); }, worker_thread);
		file_watcher.subscribe_file(this->script_file, [this](const auto&) {
			load_cd.execute_async([this] { load(); }, worker_thread);
		});
	}

private:
	void init() {
		lua.set_function("add_glow", [this](float size, float fallout, libv::vec4f color) {
			effects.emplace_back(std::make_unique<app::EffectGlow>(size, fallout, color));
		});
//		lua.set_function("define_flex_point", [](libv::vec2f bl, libv::vec2f tr) {
//
//		});


	}


	void load() {
		std::unique_lock lock(mutex);
		log_app.info("Loading...");

		libv::Timer timer;

		effects.clear();
		effects.emplace_back(std::make_unique<app::EffectRoundedBox>(libv::vec2f(16.f, 16.f), libv::vec2f(96.f, 96.f), 5.5f, 16.f));

		try {
//			lua["tex_size_x"] = texture_size.x;
//			lua["tex_size_y"] = texture_size.y;
//
			const auto script_str = libv::read_file_or_throw(script_file);
			const auto result = lua.safe_script(script_str, sol::script_pass_on_error);

			if (!result.valid()) {
				sol::error err = result;
				return log_app.error("Script execution failed: {}", err.what());
			}

//			if (result.get_type() != sol::type::table) {
//				return log_app.error("Script did not return a table: {} - {}", libv::to_value(result.get_type()), std::string(result));
//			}

			log_app.info("Script execution successful: {:7.3f}ms", timer.timef_ms().count());
		} catch (const std::exception& e) {
			log_app.error("Failed to load texture due to exception: {}", e.what());
		}

		auto texture_size = libv::vec2i{128, 128};

		app::Image image(texture_size.cast<size_t>());
		app::EffectApplyEngine engine(12);
		engine.process(image, effects);

		log_app.info("Work execution successful: {:7.3f}ms", timer.timef_ms().count());

		auto texture_data = image.generate_8bit_channels();

		callback(texture_data, texture_size);
	}

	Atlas export_atlas() {
		AtlasBuilder builder;

//		build.add(...);

		static constexpr int32_t atlas_size_max = 1024;
		for (int32_t i = 64; i <= atlas_size_max; i *= 2) {
			try {
				Atlas result = builder.build_atlas(libv::vec2i{i, i});
				return result;
			} catch (const std::exception& e) {
				if (i != atlas_size_max) {
					log_app.warn("{}", e.what());
				} else {
					log_app.error("{}", e.what());
					throw;
				}
			}
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
