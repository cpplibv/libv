// Project: libv, File: app/star/game/client_config.hpp

#pragma once

// std
#include <memory>
#include <string>
// libv
//#include <libv/ui/component/component.hpp>
//#include <libv/utility/nexus_fwd.hpp>
#include <libv/serial/serial.hpp>


//// =================================================================================================
//
//// libv
//#include <libv/meta/reflection.hpp>
//#include <libv/meta/reflection_access.hpp>
//#include <libv/serial/archive/json.hpp>
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
//
//
//// -------------------------------------------------------------------------------------------------
//
//namespace detail {
//
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
//
//} // namespace detail
//
//// -------------------------------------------------------------------------------------------------
//
//template <typename T>
//class Config : public T {
//private:
//	std::filesystem::path file_path;
//
//private:
//	inline T& self() {
//		return static_cast<T&>(*this);
//	}
//	inline const T& self() const {
//		return static_cast<const T&>(*this);
//	}
//
//	template <typename Archive, typename T2>
//	std::string serialize(const T2& object) {
//		std::ostringstream ss(std::ios::out | std::ios::binary);
//		{
//			typename Archive::output ar(ss);
//			libv::meta::foreach_member_nrp(object, [&ar](const auto key, const auto& value) {
//				ar(LIBV_NVP_NAMED(key, value));
//			});
//		}
//		return std::move(ss).str();
//	}
//
//	template <typename Archive, typename T2>
//	auto deserialize(std::string data) {
//		T2 object;
//		std::istringstream ss(std::move(data), std::ios::in | std::ios::binary);
//		{
//			typename Archive::input ar(ss);
//			libv::meta::foreach_member_nrp(object, [&ar](const auto key, auto& value) {
//				ar(LIBV_NVP_NAMED(key, value));
//			});
//		}
//		return object;
//	}
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
//
//// -------------------------------------------------------------------------------------------------
//
//using ConfigViewer = Config<detail::ConfigViewer>;
//using ConfigProject = Config<detail::ConfigProject>;
//
//// -------------------------------------------------------------------------------------------------
//
//// =================================================================================================

namespace star {

// -------------------------------------------------------------------------------------------------

//#include <libv/mt/work_cooldown.hpp>
//#include <libv/mt/worker_thread.hpp>

//	libv::mt::worker_thread scheduler_thread;

// GameSettingsManager settings; // Or profile

//class GameSettingsManager {
//	libv::mt::work_warmup_cooldown settings_file_write_cd{std::chrono::seconds(1), std::chrono::seconds(5)};
//	GameSettings settings;
//};

// -------------------------------------------------------------------------------------------------

struct BaseConfigEntry {
//	std::string name;
//	std::string path;
//	std::string description;

	virtual ~BaseConfigEntry() = default;
};

template <typename T>
struct ConfigEntry : BaseConfigEntry {
	T value;

	explicit inline ConfigEntry(T value) : value(std::move(value)) {}
//	virtual ~ConfigEntry() {}

	[[nodiscard]] constexpr inline T& operator()() noexcept {
		return value;
	}
	[[nodiscard]] constexpr inline const T& operator()() const noexcept {
		return value;
	}
	constexpr inline void operator()(const T& value_) noexcept {
		value = value_;
	}
	constexpr inline void operator()(T&& value_) noexcept {
		value = std::move(value_);
	}

};

//template <typename T>
//struct ConfigEntryChange {
//	ConfigEntry& entry;
//};

struct ConfigEntryChange {
	BaseConfigEntry& entry;
};

// -------------------------------------------------------------------------------------------------

struct ConfigProfile {
	ConfigEntry<std::string> profile_name{"profile0"};
	ConfigEntry<std::string> player_name{"Player"};

	template <typename Archive> void serialize(Archive& ar) {
		ar(LIBV_NVP_NAMED("profile_name", profile_name.value));
		ar(LIBV_NVP_NAMED("player_name", player_name.value));
	}
};

//struct ConfigGameplay {
//	template <typename Archive> void serialize(Archive& ar);
//};

struct ConfigGraphics {
	ConfigEntry<int> window_size_x{1280};
	ConfigEntry<int> window_size_y{800};
	ConfigEntry<int> save_window_size{false};

	ConfigEntry<int> window_position_x{-1};
	ConfigEntry<int> window_position_y{-1};
	ConfigEntry<int> save_window_position{false};

	ConfigEntry<bool> msaa_enable{true};
	ConfigEntry<int> msaa_sample{4};

	ConfigEntry<int> test_setting{4};

	template <typename Archive> void serialize(Archive& ar) {
		ar(LIBV_NVP_NAMED("window_size_x", window_size_x.value));
		ar(LIBV_NVP_NAMED("window_size_y", window_size_y.value));
		ar(LIBV_NVP_NAMED("save_window_size", save_window_size.value));

		ar(LIBV_NVP_NAMED("window_position_x", window_position_x.value));
		ar(LIBV_NVP_NAMED("window_position_y", window_position_y.value));
		ar(LIBV_NVP_NAMED("save_window_position", save_window_position.value));

		ar(LIBV_NVP_NAMED("msaa_enable", msaa_enable.value));
		ar(LIBV_NVP_NAMED("msaa_sample", msaa_sample.value));

		ar(LIBV_NVP_NAMED("test_setting", test_setting.value));
	}
};

//struct ConfigControl {
//	template <typename Archive> void serialize(Archive& ar);
//};
//
//struct ConfigSounds {
//	template <typename Archive> void serialize(Archive& ar);
//};

// Configuration > Setting > Property

struct ClientConfig {
	int32_t config_version = 0;

	ConfigProfile profile;
//	ConfigGameplay gameplay;
	ConfigGraphics graphics;
//	ConfigControl control;
//	ConfigSounds sound;

	template <typename Archive> void serialize(Archive& ar) {
		ar(LIBV_NVP(config_version));
		ar(LIBV_NVP(profile));
	//	ar(LIBV_NVP(gameplay));
		ar(LIBV_NVP(graphics));
	//	ar(LIBV_NVP(control));
	//	ar(LIBV_NVP(sound));
	}

public:
	~ClientConfig();

public:
	[[nodiscard]] std::string saveJSON() const;
	void loadJSON(const std::string& settingsJSONData);
};

//libv::ui::Component createConfigUI(const std::shared_ptr<ClientConfig>& config) {
//
//	page()
//	{
//		toggle(config->graphics.msaa_enable, "MSAA", "Enable/Disable MSAA");
//		toggle(config->graphics.msaa_sample, "MSAA Sample", [=]{ return config->graphics.msaa_enable(); });
//	}
//}

// -------------------------------------------------------------------------------------------------

} // namespace star
