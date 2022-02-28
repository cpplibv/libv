// Project: libv, File: app/space/view/scene_main.cpp

// hpp
#include <space/view/scene_main.hpp>
// libv
#include <libv/ui/ui.hpp>
#include <libv/utility/nexus.hpp>
// pro
#include <space/game/game_session.hpp>
#include <space/message/internal_events.hpp>
#include <space/log.hpp>
#include <space/view/overlay_shader_error.hpp>
#include <space/view/scene_command_bar.hpp>
#include <space/view/scene_menu_bar.hpp>
#include <space/view/scene_mp_status.hpp>

// <<< Will this file have the controls context enter/leave?
#include <libv/ctrl/controls.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

SceneMain::SceneMain(libv::ui::UI& ui, Renderer& renderer, GameThread& game_thread, libv::Nexus& nexus, libv::ctrl::Controls& controls, User& user) :
	nexus(nexus),
	controls(controls),
	renderer(renderer),
	user(user),
	game_thread(game_thread),
	main_layers("main"),
	game_layers("game") {

	main_layers.add(game_layers);
	main_layers.add(SceneMenuBar::create(nexus, user));
	main_layers.add(overlay_shader_error());

	ui.add(main_layers);

	register_nexus();
	controls.context_enter<SceneMain>(this);
}

SceneMain::~SceneMain() {
	controls.context_leave_if_matches<SceneMain>(this);
	unregister_nexus();
}

void SceneMain::register_nexus() {
	nexus.connect<mc::RequestCreateClient>(this, [this](const mc::RequestCreateClient& event) {
		openMPClient(event.server_address, event.server_port);
		nexus.broadcast<mc::OnCreateClient>();
	});

	nexus.connect<mc::RequestCreateServer>(this, [this](const mc::RequestCreateServer& event) {
		openMPServer(event.server_port);
		nexus.broadcast<mc::OnCreateServer>();
	});

	nexus.connect<mc::RequestDestroyClient>(this, [this] {
		openSP();
		nexus.broadcast<mc::OnDestroyClient>();
	});

	nexus.connect<mc::RequestDestroyServer>(this, [this] {
		openSP();
		nexus.broadcast<mc::OnDestroyServer>();
	});

//	nexus.connect<mc::RequestHelpControls>(this, [this] {
//		main_layers.add(OverlayHelpControls::create(nexus, controls));
//	});
}

void SceneMain::unregister_nexus() {
	nexus.disconnect_all(this);
}

// -------------------------------------------------------------------------------------------------

// <<< Whos member is this?
auto createCanvas(const std::shared_ptr<GameSession>& game_session, Renderer& renderer, libv::ctrl::Controls& controls) {
	libv::ui::CanvasAdaptorT<SpaceCanvas> canvas("space.canvas.main", controls, renderer, *game_session, true);
	canvas.z_index_offset(-100);

	canvas.event().focus.connect([&controls](libv::ui::CanvasAdaptorT<SpaceCanvas>& self, const libv::ui::EventFocus& event) {
		log_space.trace("Space Canvas Focus: {}", event.gain());

		if (event.gain())
			self.object().enableControls(controls);
		else
			self.object().disableControls(controls);
	});

	return canvas;
}

// -------------------------------------------------------------------------------------------------

void SceneMain::openSP() {
	game_session.reset();
	game_session = createSinglePlayer(game_thread, nexus);

	game_layers.clear();
	game_layers.add(createCanvas(game_session, renderer, controls));
	game_layers.add(SceneCommandBar::create(nexus));
}

void SceneMain::openMPClient(std::string server_address, uint16_t server_port) {
	game_session.reset();
	game_session = createMultiPlayerClient(game_thread, nexus, std::move(server_address), server_port, user);

	game_layers.clear();
	game_layers.add(createCanvas(game_session, renderer, controls));
	game_layers.add(SceneCommandBar::create(nexus));
	game_layers.add(SceneMPStatus::create(nexus));
}

void SceneMain::openMPServer(uint16_t port) {
	game_session.reset();
	game_session = createMultiPlayerServer(game_thread, nexus, port, user);

	game_layers.clear();
	game_layers.add(createCanvas(game_session, renderer, controls));
	game_layers.add(SceneCommandBar::create(nexus));
	game_layers.add(SceneMPStatus::create(nexus));
}

// -------------------------------------------------------------------------------------------------

} // namespace space
