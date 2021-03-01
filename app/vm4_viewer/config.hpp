// Project: libv, File: app/vm4_viewer/config.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/meta/reflection.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/serial/archive/json.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_vector.hpp>
#include <libv/utility/generic_path.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
#include <libv/utility/write_file.hpp>
// std
#include <filesystem>
#include <string>
// pro
#include <vm4_viewer/log.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

namespace detail {

struct ConfigViewer {
	int window_height = 800;
	int window_width = 1280;
	int save_window_size = false;
	std::vector<std::string> recent_projects;
	std::vector<std::string> recent_models;

	LIBV_REFLECTION_ACCESS(window_height);
	LIBV_REFLECTION_ACCESS(window_width);
	LIBV_REFLECTION_ACCESS(save_window_size);
	LIBV_REFLECTION_ACCESS(recent_projects);
	LIBV_REFLECTION_ACCESS(recent_models);
};

struct ConfigProject {
	std::string project_name = "default";
	std::string project_location;
	std::vector<std::string> recent_models;

	LIBV_REFLECTION_ACCESS(project_name);
	LIBV_REFLECTION_ACCESS(project_location);
	LIBV_REFLECTION_ACCESS(recent_models);
};

} // namespace detail

// -------------------------------------------------------------------------------------------------

template <typename T>
class Config : public T {
private:
	std::filesystem::path file_path;

private:
	inline T& self() {
		return static_cast<T&>(*this);
	}
	inline const T& self() const {
		return static_cast<const T&>(*this);
	}

	template <typename Archive, typename T2>
	std::string serialize(const T2& object) {
		std::ostringstream ss(std::ios::out | std::ios::binary);
		{
			typename Archive::output ar(ss);
			libv::meta::foreach_member_nrp(object, [&ar](const auto key, const auto& value) {
				ar(LIBV_NVP_NAMED(key, value));
			});
		}
		return std::move(ss).str();
	}

	template <typename Archive, typename T2>
	auto deserialize(std::string data) {
		T2 object;
		std::istringstream ss(std::move(data), std::ios::in | std::ios::binary);
		{
			typename Archive::input ar(ss);
			libv::meta::foreach_member_nrp(object, [&ar](const auto key, auto& value) {
				ar(LIBV_NVP_NAMED(key, value));
			});
		}
		return object;
	}

public:
	// app::ConfigViewer config(config_path, app::Config::save_auto, app::Config::log_everything);
	Config(std::filesystem::path path) : file_path(std::move(path)) {
		libv::Timer timer;

		const auto file_path_str = libv::generic_path(file_path);
		const auto config_file = libv::read_file_ec(file_path);

		if (config_file.ec == std::errc::no_such_file_or_directory) {
			app::log_app.warn("Missing config file {}. Generating default config.", file_path_str);

			auto ec = libv::write_file_ec(file_path, serialize<libv::archive::JSON>(self()));
			app::log_app.error_if(ec, "Failed to save config: {} {}:{}. Falling back to temporary default config.", file_path_str, ec, ec.message());
			app::log_app.trace_if(!ec, "Saved config {} in {:5.1f}ms", file_path_str, timer.timef_ms().count());

		} else if (config_file.ec) {
			app::log_app.error("Failed to read config: {} {}:{}. Falling back to temporary default config.", file_path_str, config_file.ec, config_file.ec.message());

		} else {
			try {
				self() = deserialize<libv::archive::JSON, T>(std::move(config_file.data));
				app::log_app.trace("Loaded config {} in {:5.1f}ms", file_path_str, timer.timef_ms().count());

			} catch (const cereal::Exception& e) {
				app::log_app.error("Failed to deserialize config: {}. Regenerating default config.", e.what());
			}
		}
	}

	void save() {
		libv::Timer timer;

		const auto file_path_str = libv::generic_path(file_path);

		timer.reset();
		auto ec = libv::write_file_ec(file_path, serialize<libv::archive::JSON>(self()));
		app::log_app.error_if(ec, "Failed to save config: {} {}:{}", file_path_str, ec, ec.message());
		app::log_app.trace_if(!ec, "Saved config {} in {:5.1f}ms", file_path_str, timer.timef_ms().count());
	}

	~Config() {
		save();
	}
};

using ConfigViewer = Config<detail::ConfigViewer>;
using ConfigProject = Config<detail::ConfigProject>;

// -------------------------------------------------------------------------------------------------

} // namespace app
