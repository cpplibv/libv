// Project: libv, File: app/space/view/scene_game.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/ui/component/fwd.hpp>
//#include <libv/ui/component/button.hpp>
//#include <libv/ui/component/label_image.hpp>
//#include <libv/ui/component/panel_float.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
//#include <memory>
//#include <optional>
// pro
//#include <space/canvas.hpp>
#include <space/camera.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct SceneGame {
	GameInstance& game;
	GameSession& game_session;
	libv::Nexus& nexus;
	libv::ctrl::Controls& controls;

public:
	SpaceCanvas* canvas = nullptr;
	CameraPlayer camera;

public:
	static libv::ui::Component create(GameInstance& game, GameSession& game_session, libv::Nexus& nexus, libv::ctrl::Controls& controls);
	SceneGame(libv::ui::PanelFloat& layers, GameInstance& game, GameSession& game_session, libv::Nexus& nexus, libv::ctrl::Controls& controls);
	~SceneGame();

private:
	libv::ui::Component init_canvas_main();
	libv::ui::Component init_canvas_mini_bar();
	libv::ui::Component init_cmd_bar();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
