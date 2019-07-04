// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/meta/reflection_access.hpp>
#include <libv/serialization/archive/json.hpp>
#include <libv/serialization/enable.hpp>
#include <libv/serialization/reflection.hpp>
#include <libv/serialization/serialization.hpp>
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
	std::string last_project;
//	std::string profile;
//	std::string projects_folder;
	int window_height = 800;
	int window_width = 1280;

	LIBV_REFLECTION_ACCESS(last_project);
	LIBV_REFLECTION_ACCESS(window_height);
	LIBV_REFLECTION_ACCESS(window_width);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

struct ConfigProject {
	std::string name;
	std::string location;

	LIBV_REFLECTION_ACCESS(name);
	LIBV_REFLECTION_ACCESS(location);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
};

} // namespace detail

// -------------------------------------------------------------------------------------------------

template <typename T>
class Config : public T {
private:
	libv::Timer timer;
	std::filesystem::path file_path;

private:
	inline T& self() {
		return static_cast<T&>(*this);
	}
	inline const T& self() const {
		return static_cast<const T&>(*this);
	}

	template <typename Archive, typename T2>
	std::string serialize(const char* name, const T2& object) {
		std::ostringstream ss(std::ios::out | std::ios::binary);
		{
			typename Archive::output ar(ss);
			ar(LIBV_NVP_NAMED(name, object));
		}
		return std::move(ss).str();
	}

	template <typename Archive, typename T2>
	auto deserialize(const char* name, std::string data) {
		T2 object;
		std::istringstream ss(std::move(data), std::ios::in | std::ios::binary);
		{
			typename Archive::input ar(ss);
			ar(LIBV_NVP_NAMED(name, object));
		}
		return object;
	}

public:
	// app::ConfigViewer config(config_path, app::Config::save_auto, app::Config::log_everything);
	Config(std::filesystem::path path) : file_path(std::move(path)) {
		const auto config_file = libv::read_file_ec(file_path);

		if (config_file.ec == std::errc::no_such_file_or_directory) {
			app::log_app.warn("Config file is missing. Generating default config file: {}", file_path.generic_string());

			auto ec = libv::write_file_ec(file_path, serialize<libv::archive::JSON>("config", self()));
			app::log_app.error_if(ec, "Failed to save config file: {} {}:{}. Falling back to temporary default config.", file_path.generic_string(), ec, ec.message());
			app::log_app.trace_if(!ec, "Saved config file {} in {:5.1f}ms", file_path.generic_string(), timer.timef_ms().count());

		} else if (config_file.ec) {
			app::log_app.error("Failed to read config file: {} {}:{}. Falling back to temporary default config.", file_path.generic_string(), config_file.ec, config_file.ec.message());

		} else {
			try {
				self() = deserialize<libv::archive::JSON, T>("config", std::move(config_file.data));
				app::log_app.trace("Config loaded in {:5.1f}ms", timer.timef_ms().count());

			} catch (const cereal::Exception& e) {
				app::log_app.error("Failed to deserialize config: {}. Regenerating default config file.", e.what());
			}
		}
	}

	~Config() {
		timer.reset();
		auto ec = libv::write_file_ec(file_path, serialize<libv::archive::JSON>("config", self()));
		app::log_app.error_if(ec, "Failed to save config file: {} {}:{}", file_path.generic_string(), ec, ec.message());
		app::log_app.trace_if(!ec, "Saved config file {} in {:5.1f}ms", file_path.generic_string(), timer.timef_ms().count());
	}

};

using ConfigViewer = Config<detail::ConfigViewer>;
using ConfigProject = Config<detail::ConfigProject>;

// -------------------------------------------------------------------------------------------------

} // namespace app
