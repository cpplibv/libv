// Project: libv, File: app/space/view/scene_main.cpp

// hpp
#include <space/view/scene_main.hpp>
// libv
#include <libv/ui/ui.hpp>
#include <libv/utility/nexus.hpp>
// pro
#include <space/game_session.hpp>
#include <space/internal_events.hpp>
#include <space/log.hpp>
#include <space/view/make_shader_error_overlay.hpp>
#include <space/view/scene_command_bar.hpp>
#include <space/view/scene_menu_bar.hpp>
#include <space/view/scene_mp_status.hpp>


// <<< Will this file have the controls context enter/leave?
#include <libv/ctrl/controls.hpp>
//#include <libv/ctrl/binding.hpp>
//#include <libv/ctrl/feature_register.hpp>


namespace app {

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
	main_layers.add(app::make_shader_error_overlay());

	ui.add(main_layers);

	register_nexus();
}

SceneMain::~SceneMain() {
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
}

void SceneMain::unregister_nexus() {
	nexus.disconnect_all(this);
}

// -------------------------------------------------------------------------------------------------

// <<< Whos member is this?
auto createCanvas(const std::shared_ptr<GameSession>& game_session, Renderer& renderer, libv::ctrl::Controls& controls) {
	libv::ui::CanvasAdaptorT<SpaceCanvas> canvas("space.canvas.main", renderer, *game_session, true);

	canvas.event().focus.connect([&controls](libv::ui::CanvasAdaptorT<SpaceCanvas>& self, const libv::ui::EventFocus& event) {
		log_space.trace("Space Canvas Focus: {}", event.gain());

		if (event.gain()) {
			controls.context_enter<app::SpaceCanvas>(&self.object());
			controls.context_enter<app::BaseCameraOrbit>(&self.object().camera);
		} else {
			controls.context_leave_if_matches<app::BaseCameraOrbit>(&self.object().camera);
			controls.context_leave_if_matches<app::SpaceCanvas>(&self.object());
		}
	});

	return canvas;
}

// =================================================================================================

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
//	game_layers.add(SceneMPStatus::create(nexus, lobby));
}

void SceneMain::openMPServer(uint16_t port) {
	game_session.reset();
	game_session = createMultiPlayerServer(game_thread, nexus, port, user);

	game_layers.clear();
	game_layers.add(createCanvas(game_session, renderer, controls));
	game_layers.add(SceneCommandBar::create(nexus));
	game_layers.add(SceneMPStatus::create(nexus));
//	game_layers.add(SceneMPStatus::create(nexus, lobby));
}

//void SceneMain::add_game_scene(GameInstance& game, GameSession& game_session, libv::Nexus& nexus, libv::ctrl::Controls& controls) {
//	if (game_scene) {
//		game_scene->markRemove();
//		game_scene.reset();
//	}
//	game_scene.emplace(SceneGame::create(game, game_session, nexus, controls));
//	main_layers.add_front(*game_scene);
//	main_layers.add(SceneMPStatus::create(nexus));
////	main_layers.add(SceneMPStatus::create(nexus, lobby));
//}

// -------------------------------------------------------------------------------------------------

} // namespace app

// =================================================================================================

//struct GameBehaviour {
//	void register_controls(libv::ctrl::FeatureRegister controls);
//	void bind_default_controls(libv::ctrl::Controls& controls);
//};
//
//void GameBehaviour::register_controls(libv::ctrl::FeatureRegister controls) {
//	controls.feature_binary<app::SpaceCanvas>("space.show_controls", [&controls = controls.owner()](libv::ctrl::arg_binary& arg, app::SpaceCanvas& ctx) {
//		if (arg.value) {
//			std::ostringstream os;
//			controls.foreach_bindings([&os](const libv::ctrl::Binding& binding) {
//				os << binding.feature_name() << ":" << binding.sequence().to_string() << "\n";
//			});
//			auto text = std::move(os).str();
//			if (!text.empty())
//				text.pop_back(); // Discard the last \n character
//
//			libv::ui::Label label("help-controls");
//			label.text(std::move(text));
//			main_layers.add(label);
//		} else {
//			main_layers.remove(label);
//		}
//	});
//}
//
//void GameBehaviour::bind_default_controls(libv::ctrl::Controls& controls) {
//	controls.bind("space.show_controls", "F1");
//}
