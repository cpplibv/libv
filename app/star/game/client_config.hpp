// Project: libv, File: app/star/game/client_config.hpp

#pragma once

// std
#include <memory>
#include <string>
// libv
//#include <libv/ui/component/component.hpp>
//#include <libv/utility/nexus_fwd.hpp>
#include <libv/serial/serial.hpp>


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
