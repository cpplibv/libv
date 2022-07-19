// Project: libv, File: app/star/game/client_config.hpp

#pragma once

// pro
#include <star/game/config/config.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct ConfigGroupProfile : BaseConfigGroup {
	ConfigEntry<std::string> profile_name{config, "Profile0", "Profile Name", "Profile name of the local configurations"};
	ConfigEntry<std::string> player_name {config, "Player0000", "Player Name", "Display name of the player in multiplayer sessions"};

	template <typename Archive> void serialize(Archive& ar) {
		ar.nvp("profile_name", profile_name);
		ar.nvp("player_name", player_name);
	}
};

struct ConfigGroupGameplay : BaseConfigGroup {
	template <typename Archive> void serialize(Archive& ar) {
		(void) ar;
	}
};

struct ConfigGroupGraphics : BaseConfigGroup {
	ConfigEntry<int> window_size_x           {config, 1280, "window_size_x", "window_size_x desc"};
	ConfigEntry<int> window_size_y           {config, 800, "window_size_y", "window_size_y desc"};
	ConfigEntry<bool> persist_window_size    {config, false, "persist_window_size", "persist_window_size desc"};

	ConfigEntry<int> window_position_x       {config, 0, "window_position_x", "window_position_x desc"};
	ConfigEntry<int> window_position_y       {config, 0, "window_position_y", "window_position_y desc"};
	ConfigEntry<bool> persist_window_position{config, false, "persist_window_position", "persist_window_position desc"};

	ConfigEntry<bool> vsync_enable           {config, true, "VSync Enable", "Enable/Disable VSync"};
	ConfigEntry<int> msaa_samples            {config, 4, "MSAA Samples", "Number of MSAA samples"};

	template <typename Archive> void serialize(Archive& ar) {
		ar.nvp("window_size_x", window_size_x);
		ar.nvp("window_size_y", window_size_y);
		ar.nvp("persist_window_size", persist_window_size);

		ar.nvp("window_position_x", window_position_x);
		ar.nvp("window_position_y", window_position_y);
		ar.nvp("persist_window_position", persist_window_position);

		ar.nvp("vsync_enable", vsync_enable);
		ar.nvp("msaa_samples", msaa_samples);
	}
};

struct ConfigGroupControl : BaseConfigGroup {
	template <typename Archive> void serialize(Archive& ar) {
		(void) ar;
	}
};

struct ConfigGroupSounds : BaseConfigGroup {
	template <typename Archive> void serialize(Archive& ar) {
		(void) ar;
	}
};

struct ConfigGroupDevelopment : BaseConfigGroup {

	ConfigEntry<bool> logging_trace_ui{config, false, "Logging: UI Trace", "Enable/Disable UI Trace logging"};
	ConfigEntry<bool> always_on_top{config, false, "Always On Top", "Enable/Disable always on top window setting on start"};

	ConfigEntry<int> test_setting            {config, 1, "test_setting", "test_setting desc"};
	ConfigEntry<std::string> test_setting_str{config, "Test string default content", "test_setting_str", "test_setting str desc"};

	template <typename Archive> void serialize(Archive& ar) {
		ar.nvp("logging_trace_ui", logging_trace_ui);
		ar.nvp("always_on_top", always_on_top);
		ar.nvp("test_setting", test_setting);
		ar.nvp("test_setting_str", test_setting_str);
	}
};

// ---

class ClientConfigT : public BaseConfig {
	int32_t config_version = 2;

public:
	ConfigGroupProfile profile{*this};
	ConfigGroupGameplay gameplay{*this};
	ConfigGroupGraphics graphics{*this};
	ConfigGroupControl control{*this};
	ConfigGroupSounds sound{*this};
	ConfigGroupDevelopment development{*this};

	template <typename Archive> void serialize(Archive& ar) {
		if constexpr (Archive::is_output)
			ar.nvp("config_version", config_version);
		else {
			int32_t _input_version;
			ar.nvp("config_version", _input_version);
		}
		// ar.version_constant("config_version", config_version);
		// ar.constant("config_version", config_version);
//		const auto version_guard = ar.scope_version("config_version", config_version);

		ar.nvp("profile", profile);
		ar.nvp("gameplay", gameplay);
		ar.nvp("graphics", graphics);
		ar.nvp("control", control);
		ar.nvp("sound", sound);
		ar.nvp("development", development);
	}

public:
	using BaseConfig::BaseConfig;

private:
	[[nodiscard]] virtual std::string saveJSON() const override;
	virtual void loadJSON(const std::string& settingsJSONData) override;

public:
	virtual void resetToDefault() override;
};

using ClientConfig = Config<ClientConfigT>;

// -------------------------------------------------------------------------------------------------

} // namespace star
