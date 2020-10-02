// Project: libv.ui, File: app/gen_ui_theme/lua_engine.hpp, Author: Császár Mátyás [Vader]

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


namespace app {

// -------------------------------------------------------------------------------------------------

class Engine {
	std::filesystem::path script_file;

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

		load_cd.execute_async([this] { load(); }, worker_thread);
		file_watcher.subscribe_file(this->script_file, [this](const auto&) {
			load_cd.execute_async([this] { load(); }, worker_thread);
		});
	}

private:
	void load() {
		try {
			log_app.info("Updating texture...");

			std::unique_lock guard(mutex);

			libv::Timer load_timer;

//			lua["tex_size_x"] = texture_size.x;
//			lua["tex_size_y"] = texture_size.y;
//
//			const auto script_str = libv::read_file_or_throw(script_file);
//			const auto result = lua.safe_script(script_str, sol::script_pass_on_error);
//
//			if (!result.valid()) {
//				sol::error err = result;
//				return log_app.error("Script execution failed: {}", err.what());
//			}
//
//			if (result.get_type() != sol::type::table) {
//				return log_app.error("Script did not return a table: {} - {}", libv::to_value(result.get_type()), std::string(result));
//			}
//
			log_app.info("Script execution successful: {:7.3f}ms", load_timer.timef_ms().count());
//
//			sol::table result_table = result;

			auto texture_size = libv::vec2i{128, 128};
			auto texture_data = std::vector<libv::vec4uc>(texture_size.x * texture_size.y, libv::vec4uc{});

//			int i = 0;
//			for (const auto v : result_table) {
//				if (!v.second.is<libv::vec4f>()) {
//					log_app.error("Element {} is not a vec4f", i);
//					i++;
//					continue;
//				}
//
//				libv::vec4f c = v.second.as<libv::vec4f>();
//				texture_data[i++] = (libv::vec::clamp(c, 0.0f, 1.0f) * 255.f).cast<uint8_t>();
//			}

			app::Image image(texture_size.cast<size_t>());
			std::vector<std::unique_ptr<app::Effect>> effects;
			effects.emplace_back(std::make_unique<app::EffectRoundedBox>(libv::vec2f(16.f, 16.f), libv::vec2f(96.f, 96.f), 5.5f, 16.f));
			effects.emplace_back(std::make_unique<app::EffectGlow>(6.5f, 3, libv::vec4f(0.2f, 0.2f, 0.5f, 0.5f)));
			effects.emplace_back(std::make_unique<app::EffectGlow>(-1.5f, 1, libv::vec4f(0.8f, 0.8f, 1, 0.8f)));

			app::EffectApplyEngine engine(12);
			engine.process(image, effects);

			log_app.info("Work execution successful: {:7.3f}ms", load_timer.timef_ms().count());

			int i = 0;
			for (int x = 0; x < texture_size.x; ++x) {
				for (int y = 0; y < texture_size.y; ++y) {
					const auto c = image.color(x, y);
					texture_data[i++] = (libv::vec::clamp(c, 0.0f, 1.0f) * 255.f).cast<uint8_t>();
				}
			}

			callback(texture_data, texture_size);

		} catch (const std::exception& e) {
			log_app.error("Failed to load texture due to exception: {}", e.what());
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
