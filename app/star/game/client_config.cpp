// Project: libv, File: app/star/game/client_config.cpp

// hpp
#include <star/game/client_config.hpp>
// std
//#include <memory>
#include <sstream>
// libv
//#include <libv/serial/archive/json_any.hpp>
//#include <libv/serial/serial.hpp>
////#include <libv/serial/archive/json.hpp>
//#include <libv/ui/component/component.hpp>
//#include <libv/utility/nexus_fwd.hpp>


//// libv
//#include <libv/meta/reflection.hpp>
//#include <libv/meta/reflection_access.hpp>
#include <libv/serial/archive/json.hpp>
//#include <libv/serial/limit.hpp>
//#include <libv/serial/serial.hpp>
//#include <libv/serial/types/std_string.hpp>
//#include <libv/serial/types/std_vector.hpp>
//#include <libv/utility/generic_path.hpp>
//#include <libv/utility/read_file.hpp>
//#include <libv/utility/timer.hpp>
//#include <libv/utility/write_file.hpp>
//// std
//#include <filesystem>
//#include <string>
//// pro
//#include <vm4_viewer/log.hpp>

namespace star {

// -------------------------------------------------------------------------------------------------

//struct ConfigViewer {
//	int window_height = 800;
//	int window_width = 1280;
//	int save_window_size = false;
//	libv::serial::limit<std::vector<libv::serial::limit<std::string>, 1024>, 1024> recent_projects;
//	libv::serial::limit<std::vector<libv::serial::limit<std::string>, 1024>, 1024> recent_models;
//
//	LIBV_REFLECTION_ACCESS(window_height);
//	LIBV_REFLECTION_ACCESS(window_width);
//	LIBV_REFLECTION_ACCESS(save_window_size);
//	LIBV_REFLECTION_ACCESS(recent_projects);
//	LIBV_REFLECTION_ACCESS(recent_models);
//};
//
//struct ConfigProject {
//	libv::serial::limit<std::string, 1024> project_name = "default";
//	libv::serial::limit<std::string, 1024> project_location;
//	libv::serial::limit<std::vector<libv::serial::limit<std::string>, 1024>, 1024> recent_models;
//
//	LIBV_REFLECTION_ACCESS(project_name);
//	LIBV_REFLECTION_ACCESS(project_location);
//	LIBV_REFLECTION_ACCESS(recent_models);
//};

// -------------------------------------------------------------------------------------------------

//class Config {
//private:
//	std::filesystem::path file_path;
//
//public:
//	// app::ConfigViewer config(config_path, app::Config::save_auto, app::Config::log_everything);
//	Config(std::filesystem::path path) : file_path(std::move(path)) {
//		libv::Timer timer;
//
//		const auto file_path_str = libv::generic_path(file_path);
//		const auto config_file = libv::read_file_ec(file_path);
//
//		if (config_file.ec == std::errc::no_such_file_or_directory) {
//			app::log_app.warn("Missing config file {}. Generating default config.", file_path_str);
//
//			auto ec = libv::write_file_ec(file_path, serialize<libv::archive::JSON>(self()));
//			app::log_app.error_if(ec, "Failed to save config: {} {}:{}. Falling back to temporary default config.", file_path_str, ec, ec.message());
//			app::log_app.trace_if(!ec, "Saved config {} in {:5.1f}ms", file_path_str, timer.timef_ms().count());
//
//		} else if (config_file.ec) {
//			app::log_app.error("Failed to read config: {} {}:{}. Falling back to temporary default config.", file_path_str, config_file.ec, config_file.ec.message());
//
//		} else {
//			try {
//				self() = deserialize<libv::archive::JSON, T>(std::move(config_file.data));
//				app::log_app.trace("Loaded config {} in {:5.1f}ms", file_path_str, timer.timef_ms().count());
//
//			} catch (const cereal::Exception& e) {
//				app::log_app.error("Failed to deserialize config: {}. Regenerating default config.", e.what());
//			}
//		}
//	}
//
//	void save() {
//
//		const auto file_path_str = libv::generic_path(file_path);
//
//		libv::Timer timer;
//		auto ec = libv::write_file_ec(file_path, serialize<libv::archive::JSON>(self()));
//		app::log_app.error_if(ec, "Failed to save config: {} {}:{}", file_path_str, ec, ec.message());
//		app::log_app.trace_if(!ec, "Saved config {} in {:5.1f}ms", file_path_str, timer.timef_ms().count());
//	}
//
//	~Config() {
//		save();
//	}
//};

// =================================================================================================

ClientConfig::~ClientConfig() {
	//save();
}

[[nodiscard]] std::string ClientConfig::saveJSON() const {
	std::ostringstream ss(std::ios::out | std::ios::binary);
	{
		libv::archive::JSONOutput ar(ss);
		ar(*this);
	}
	return std::move(ss).str();
}

void ClientConfig::loadJSON(const std::string& settingsJSONData) {
	std::istringstream ss(std::move(settingsJSONData), std::ios::in | std::ios::binary);
	{
		libv::archive::JSONInput ar(ss);
		ar(*this);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace star
