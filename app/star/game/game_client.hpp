// Project: libv, File: app/star/game/game_client.hpp

#pragma once

// fwd
#include <star/game/fwd.hpp>
// libv
#include <libv/ui/ui.hpp>
#include <libv/utility/nexus.hpp>
//#include <libv/ctrl/controls.hpp>
// std
#include <filesystem>
#include <memory>
//#include <optional>
//#include <string>
// pro
#include <star/game/game_client_frame.hpp>
//#include <star/game/client_settings.hpp>
//#include <star/game/game_thread.hpp>
//#include <star/game/user.hpp>
//#include <star/view/render/renderer.hpp>
//#include <star/view/scene_main.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

//class GameClient {
//private:
//	std::unique_ptr<class ImplGameClient> self;
//
//public:
//	GameClient();
//	~GameClient();
//};
//
//GameClient::GameClient() :
//	self(std::make_unique<ImplGameClient>()) {
//}
//
//GameClient::~GameClient() {
//	// For the sake of forward declared unique_ptr
//}

class GameClient {
private:
	std::shared_ptr<ClientConfig> settings_;

	GameClientFrame frame;

	libv::Nexus nexus_;
//	libv::ctrl::Controls controls;

	libv::ui::UI ui;

//	Renderer renderer{ui};
//
//	GameThread game_thread{ui, nexus};
//
//	User user;

public:
	explicit GameClient(const std::filesystem::path& settingsFilepath);
	~GameClient();

//public:
//	void execute();
//
//public:
//	void enterSinglePlayer();
//	void enterMultiPlayerClient(std::string server_address, uint16_t server_port);
//	void enterMultiPlayerServer(uint16_t port);

private:
	void register_nexus();
	void unregister_nexus();
	void init_ui();

public:
	void run();

public:
	[[nodiscard]] constexpr inline libv::Nexus& nexus() noexcept {
		return nexus_;
	}

	[[nodiscard]] constexpr inline const std::shared_ptr<ClientConfig>& settings() const noexcept {
		return settings_;
	}
};

// ---

//#include <libv/mt/work_cooldown.hpp>
//#include <libv/mt/worker_thread.hpp>

//	libv::mt::worker_thread scheduler_thread;

// GameSettingsManager settings; // Or profile

//class GameSettingsManager {
//	libv::mt::work_warmup_cooldown settings_file_write_cd{std::chrono::seconds(1), std::chrono::seconds(5)};
//	GameSettings settings;
//};

// -------------------------------------------------------------------------------------------------

} // namespace star


