// Project: libv, File: app/star/game/scene/scene_main_menu.cpp

// hpp
#include <star/game/scene/scenes.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/gap.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/utility/nexus.hpp>
// pro
#include <star/game/control/requests.hpp>
#include <star/game/scene/utility.hpp>
#include <star/version.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

libv::ui::Component layoutSceneMainMenu(libv::ui::Component&& content) {
	libv::ui::PanelAnchor layers{"layers"};
	layers.add(std::move(content));

	auto lbl = libv::ui::Label::nsa("version_lbl", "main-menu.version-lbl", star::build.version_name);
	layers.add(std::move(lbl));

	return layers;
}

libv::ui::Component createSceneMainMenu(libv::Nexus& nexus) {
	const auto menuEntry = [&](std::string name, auto createFn) {
		auto btn = libv::ui::Button::sa("main-menu.menu.entry", std::move(name));

		if constexpr (std::is_same_v<decltype(createFn), std::nullptr_t>)
			btn.enable(false);
		else
			btn.event().submit.connect([nexus, createFn](libv::ui::Button& source) mutable {
				switchParentScene("main", source, createFn(nexus));
			});

		return btn;
	};

	auto golden_line = libv::ui::PanelLine::ns("golden-line", "main-menu.golden-line");

	golden_line.add_ns<libv::ui::Gap>("golden-gap", "main-menu.golden-gap");
	auto golden_box = golden_line.add_ns<libv::ui::PanelLine>("golden-box", "main-menu.golden-box");

	auto menu_box = golden_box.add_ns<libv::ui::PanelLine>("menu_box", "main-menu.menu.panel");

	menu_box.add_nsa<libv::ui::Label>("title", "main-menu.menu.title", "IRIS STAR");
	menu_box.add(menuEntry("Surface", createSceneSurface));
//	menu_box.add(menuEntry("Space", nullptr));
//	menu_box.add(menuEntry("VM4 Viewer", nullptr));
	menu_box.add(menuEntry("Single player", nullptr));
	menu_box.add(menuEntry("Multiplayer", nullptr));
	menu_box.add(menuEntry("Load", nullptr));
	menu_box.add(menuEntry("Mods", nullptr));
	menu_box.add(menuEntry("Settings", createSceneSettings));
	menu_box.add(menuEntry("Controls", createSceneControls));
	menu_box.add(menuEntry("Credits", nullptr));

	auto btn_exit = menu_box.add_sa<libv::ui::Button>("main-menu.menu.entry", "Exit");
	btn_exit.event().submit.connect([nexus] {
		nexus.broadcast_global(RequestClientExit());
	});

	return layoutSceneMainMenu(std::move(golden_line));
}

// -------------------------------------------------------------------------------------------------

} // namespace star
