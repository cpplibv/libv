// Project: libv, File: app/space/scene.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/view/scene_game.hpp>
// libv
#include <libv/ui/attach_state.hpp>
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/label_image.hpp>
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

//#include <space/network_client.hpp>
//#include <space/network_server.hpp>
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
	libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_main("canvas-main", game.renderer, game_session, game_session.universe, *game_session.playout, camera, true);
//		style("space-canvas-main");

//	out_canvas_main.emplace(canvas_main);
	canvas = &canvas_main.object();

	return canvas_main;
}

libv::ui::Component SceneGame::init_canvas_mini_bar() {
	libv::ui::PanelLine mp_cam_bar("mp-cam-bar");
//		style("space.hud-bar.mp-cam");
	mp_cam_bar.anchor(libv::ui::Anchor::center_right);
	mp_cam_bar.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
	mp_cam_bar.size(libv::ui::parse_size_or_throw("25%, 100%"));
	mp_cam_bar.spacing(5);
	mp_cam_bar.margin(5);

//	{
//		libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_mini("canvas-mini", game, game->session->player[X]->camera, false);
////		style("space-canvas-mini");
//		canvas_mini.size(libv::ui::parse_size_or_throw("1r, 15%"));
//		canvas_mini.margin(10);
//		canvas_mini.anchor(libv::ui::Anchor::center_right);
//		mp_cam_bar.add(canvas_mini);
//	}

	return mp_cam_bar;
}

libv::ui::Component SceneGame::init_cmd_bar() {
	libv::ui::PanelLine cmd_bar("cmd-bar");
//		style("space.hud-bar.cmd");
	cmd_bar.anchor(libv::ui::Anchor::bottom_right);
	cmd_bar.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
	cmd_bar.size(libv::ui::parse_size_or_throw("D, D"));
	cmd_bar.spacing(5);
	cmd_bar.margin(5);

	{
		libv::ui::Button clear_fleets;
//			style("space.hud-bar.cmd.button");
		clear_fleets.align_horizontal(libv::ui::AlignHorizontal::center);
		clear_fleets.align_vertical(libv::ui::AlignVertical::center);
		clear_fleets.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
		clear_fleets.size(libv::ui::parse_size_or_throw("10pxD1r, 4pxD"));
		clear_fleets.text("Clear Fleets");
		clear_fleets.event().submit.connect([this]() {
			nexus.broadcast(mc::RequestClearFleets{});
		});
		cmd_bar.add(std::move(clear_fleets));

		libv::ui::Button clear_fleets_long;
//			style("space.hud-bar.cmd.button");
		clear_fleets_long.align_horizontal(libv::ui::AlignHorizontal::center);
		clear_fleets_long.align_vertical(libv::ui::AlignVertical::center);
		clear_fleets_long.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
		clear_fleets_long.size(libv::ui::parse_size_or_throw("10pxD1r, 4pxD"));
		clear_fleets_long.text("Clear Fleets With Longer Label");
		clear_fleets_long.event().submit.connect([this]() {
			nexus.broadcast(mc::RequestClearFleets{});
		});
		cmd_bar.add(std::move(clear_fleets_long));

		libv::ui::Button shuffle;
//			style("space.hud-bar.cmd.button");
		shuffle.align_horizontal(libv::ui::AlignHorizontal::center);
		shuffle.align_vertical(libv::ui::AlignVertical::center);
		shuffle.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
		shuffle.size(libv::ui::parse_size_or_throw("10pxD1r, 4pxD"));
		shuffle.text("Shuffle");
		shuffle.event().submit.connect([this]() {
			nexus.broadcast(mc::RequestShuffle{});
		});
		cmd_bar.add(std::move(shuffle));
	}

	return cmd_bar;
}

// -------------------------------------------------------------------------------------------------

} // namespace app
