// Project: libv, File: app/space/view/scene_main.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
#include <optional>
#include <memory>


namespace space {

// -------------------------------------------------------------------------------------------------

class SceneMain {
	friend SceneMainControl;

private:
	libv::Nexus& nexus;
	libv::ctrl::Controls& controls;
	Renderer& renderer;
	User& user;
	GameThread& game_thread;

	libv::ui::PanelAnchor main_layers; // libv::ui::Component main_layers;
	libv::ui::PanelAnchor game_layers; // libv::ui::Component game_layers;

	std::shared_ptr<GameSession> game_session;
//	std::optional<libv::ui::Component> game_scene;

public:
	SceneMain(libv::ui::UI& ui, Renderer& renderer, GameThread& game_thread, libv::Nexus& nexus, libv::ctrl::Controls& controls, User& user);
	~SceneMain();

private:
	void register_nexus();
	void unregister_nexus();

public:
	void openSP();
	void openMPClient(std::string server_address, uint16_t server_port);
	void openMPServer(uint16_t port);

//public:
//	void add_game_scene(GameInstance& game, GameSession& game_session, libv::Nexus& nexus, libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
