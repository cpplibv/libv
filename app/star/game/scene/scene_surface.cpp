// Project: libv, File: app/star/game/scene/scene_settings.cpp

// hpp
#include <star/game/scene/scenes.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_line.hpp>
// pro
#include <star/game/scene/utility.hpp>
//#include <star/game/config/client_config.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct SceneSurface {
	std::string some_data;

	[[nodiscard]] libv::ui::Component create_scene(libv::Nexus& nexus) {
		auto layers = libv::ui::PanelLine::n("line");

		auto label = layers.add_a<libv::ui::Label>("Surface: " + some_data);
		label.align_horizontal(libv::ui::AlignHorizontal::center);
		label.align_vertical(libv::ui::AlignVertical::center);
		label.font_color({1, 1, 1, 1});

		auto btn = layers.add_a<libv::ui::Button>("Back to Main Menu");
		btn.event().submit.connect([nexus](libv::ui::Button& source) mutable {
			switchParentScene(source, "main", createSceneMainMenu(nexus));
		});

		return layers;
	}
};

libv::ui::Component createSceneSurface(libv::Nexus& nexus) {
	return assembleScene<SceneSurface>("Hi")(nexus);
}

// -------------------------------------------------------------------------------------------------

} // namespace star
