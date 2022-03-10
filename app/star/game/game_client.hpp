// Created by Vader on 2022.03.08..

#pragma once

// fwd
//#include <star/fwd.hpp>
// libv
//#include <libv/ctrl/controls.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/nexus.hpp>
// std
//#include <memory>
//#include <optional>
//#include <string>
// pro
#include <star/game/game_client_frame.hpp>
//#include <star/game/game_thread.hpp>
//#include <star/game/user.hpp>
//#include <star/view/render/renderer.hpp>
//#include <star/view/scene_main.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

class GameClient {
private:
	GameClientFrame frame;

	libv::Nexus nexus;
//	libv::ctrl::Controls controls;

	libv::ui::UI ui;

//	Renderer renderer{ui};
//
//	GameThread game_thread{ui, nexus};
//
//	User user;

public:
	GameClient();
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


