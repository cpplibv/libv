// Project: libv, File: app/space/game/game_instance.hpp

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
#include <space/game/game_thread.hpp>
#include <space/game/user.hpp>
#include <space/view/frame.hpp>
#include <space/view/render/renderer.hpp>
#include <space/view/scene_main.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class GameInstance {
public:
	SpaceFrame frame;

	libv::Nexus nexus;
	libv::ctrl::Controls controls;

	libv::ui::UI ui;

	Renderer renderer{ui};

	GameThread game_thread{ui, nexus};

	User user;

private:
	SceneMain scene_main{ui, renderer, game_thread, nexus, controls, user};

public:
	GameInstance();
	~GameInstance();

private:
	void register_nexus();
	void unregister_nexus();

public:
	void execute();
//	void update() {
//		game_session->update();
//	}

public:
	void enterSinglePlayer();
	void enterMultiPlayerClient(std::string server_address, uint16_t server_port);
	void enterMultiPlayerServer(uint16_t port);
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

} // namespace space
