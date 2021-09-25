// Project: libv, File: app/space/view/scene_game.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/view/scene_game.hpp>
// libv
#include <libv/sys/start.hpp>
#include <libv/ui/attach_state.hpp>
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/input_field.hpp>
//#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_float.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/utility/nexus.hpp>
// pro
#include <space/command.hpp>
#include <space/game_instance.hpp>
#include <space/game_session.hpp>
#include <space/message_control.hpp>
#include <space/playout.hpp>
#include <space/canvas.hpp>

//#include <space/view/scene_mp_bar.hpp>

//#include <space/network/network_client.hpp>
//#include <space/network/network_server.hpp>
//#include <space/user.hpp>
//#include <space/camera.hpp>
//#include <space/state.hpp>
//#include <space/signal/nexus.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

libv::ui::Component SceneGame::create(GameInstance& game, GameSession& game_session, libv::Nexus& nexus, libv::ctrl::Controls& controls) {
	libv::ui::PanelFloat layers("hud");
	libv::ui::attach_state<SceneGame>(layers)(layers, game, game_session, nexus, controls);
	return layers;
}

SceneGame::SceneGame(libv::ui::PanelFloat& layers, GameInstance& game, GameSession& game_session, libv::Nexus& nexus, libv::ctrl::Controls& controls) :
	game(game),
	game_session(game_session),
	nexus(nexus),
	controls(controls) {

	camera.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});

	layers.add(init_canvas_main());
	layers.add(init_canvas_mini_bar());
	layers.add(init_cmd_bar());

	// TODO P1: Enter / leave on canvas based on focus-unfocus
	//controls.context_enter<app::BaseCameraOrbit>(&__camera__);
	//controls.context_enter<app::SpaceCanvas>(&__canvas__);

	controls.context_enter<app::BaseCameraOrbit>(&camera);
	controls.context_enter<app::SpaceCanvas>(canvas);
}

SceneGame::~SceneGame() {
	// !!!, currently the new game scene is created before the previous is destroyed
//	controls.context_leave<app::BaseCameraOrbit>();
//	controls.context_leave<app::SpaceCanvas>();

//	controls.context_leave_if_matches<app::SpaceCanvas>(canvas); // <<<

	nexus.disconnect_all(this);
}

libv::ui::Component SceneGame::init_canvas_main() {
	libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_main("space.canvas.main", game.renderer, game_session, game_session.universe, *game_session.playout, camera, true);
//		style("space-canvas-main");

//	out_canvas_main.emplace(canvas_main);
	canvas = &canvas_main.object();

	return canvas_main;
}

libv::ui::Component SceneGame::init_canvas_mini_bar() {
	libv::ui::PanelLine mp_cam_bar("mp-cam-bar");
	mp_cam_bar.style("space.hud-bar.mp-cam.panel");

//	{
//		libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_mini("space.canvas.mini", game, game->session->player[X]->camera, false);
//		canvas_mini.style("space.canvas.mini");
//		mp_cam_bar.add(canvas_mini);
//	}

	return mp_cam_bar;
}

libv::ui::Component SceneGame::init_cmd_bar() {
	libv::ui::PanelLine cmd_bar("cmd-bar");
	cmd_bar.style("space.hud-bar.cmd.panel");

	{
		libv::ui::Button clear_fleets;
		clear_fleets.style("space.hud-bar.cmd.btn");
		clear_fleets.text("Clear Fleets");
		clear_fleets.event().submit.connect([this]() {
			nexus.broadcast(mc::RequestClearFleets{});
		});
		cmd_bar.add(std::move(clear_fleets));

		libv::ui::Button clear_fleets_long;
		clear_fleets_long.style("space.hud-bar.cmd.btn");
		clear_fleets_long.text("Clear Fleets With Longer Label");
		clear_fleets_long.event().submit.connect([this]() {
			nexus.broadcast(mc::RequestClearFleets{});
		});
		cmd_bar.add(std::move(clear_fleets_long));

		libv::ui::Button shuffle;
		shuffle.style("space.hud-bar.cmd.btn");
		shuffle.text("Shuffle");
		shuffle.event().submit.connect([this]() {
			nexus.broadcast(mc::RequestShuffle{});
		});
		cmd_bar.add(std::move(shuffle));

		libv::ui::Button link;
		link.style("space.hud-bar.cmd.btn");
		link.text("Open link: corruptedai.com");
		link.event().submit.connect([]() {
			libv::sys::start_unsafe("https://corruptedai.com");
		});
//		libv::ui::InputField lin;
//		lin.style("space.hud-bar.cmd.btn");
//		lin.text("https://corruptedai.com");
//		cmd_bar.add(lin);
//		link.event().submit.connect([lin]() {
//			libv::sys::start_unsafe(lin.text());
//		});
		cmd_bar.add(std::move(link));
	}

	return cmd_bar;
}

// -------------------------------------------------------------------------------------------------

} // namespace app
