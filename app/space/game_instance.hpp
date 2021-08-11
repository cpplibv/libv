// Project: libv, File: app/space/game_instance.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ctrl/controls.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/nexus.hpp>
// std
#include <memory>
#include <optional>
#include <string>
// pro
#include <space/player.hpp>
#include <space/renderer.hpp>
#include <space/view/frame.hpp>


// Not for long ?
#include <libv/ui/component/component.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

class GameInstance {
public:
	SpaceFrame frame;

	libv::Nexus nexus;
	libv::ctrl::Controls controls;

	libv::ui::UI ui;

	Renderer renderer{ui};

	Player player;

	std::shared_ptr<GameSession> game_session;
	std::optional<libv::ui::Component> game_session_ui; // <<<

public:
	GameInstance();
	~GameInstance();

public:
	void execute();
//	void update() {
//		game_session->update();
//	}

	void register_nexus();
	void unregister_nexus();

public:
	void enter_single_player();
	void enter_multi_player_client(std::string server_address, uint16_t server_port);
	void enter_multi_player_server(uint16_t port);
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

} // namespace app
