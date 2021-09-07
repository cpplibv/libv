// Project: libv, File: app/space/scene.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/panel_float.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
#include <optional>


namespace app {

// -------------------------------------------------------------------------------------------------

class SceneMainUI {
private:
	libv::Nexus& nexus;

	libv::ui::PanelFloat main_layers; // libv::ui::Component main_layers;
	std::optional<libv::ui::Component> game_scene;

public:
	SceneMainUI(libv::ui::UI& ui, libv::Nexus& nexus, Player& player);

public:
	void add_game_scene(GameInstance& game, GameSession& game_session, libv::Nexus& nexus, libv::ctrl::Controls& controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
