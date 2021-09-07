// Project: libv, File: app/space/scene.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/view/scene_main_ui.hpp>
// libv
#include <libv/ui/ui.hpp>
// pro
#include <space/view/make_shader_error_overlay.hpp>
#include <space/view/scene_mp_bar.hpp>
#include <space/view/scene_game.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

SceneMainUI::SceneMainUI(libv::ui::UI& ui, libv::Nexus& nexus, Player& player) :
	nexus(nexus),
	main_layers("main-layers") {

//	main_layers.add(game_layers);
	main_layers.add(SceneMPBar::create(nexus, player));
	main_layers.add(app::make_shader_error_overlay());

	ui.add(main_layers);
}

void SceneMainUI::add_game_scene(GameInstance& game, GameSession& game_session, libv::Nexus& nexus, libv::ctrl::Controls& controls) {
	if (game_scene) {
		game_scene->markRemove();
		game_scene.reset();
	}
	game_scene.emplace(SceneGame::create(game, game_session, nexus, controls));
	main_layers.add_front(*game_scene);
}

// -------------------------------------------------------------------------------------------------

} // namespace app
