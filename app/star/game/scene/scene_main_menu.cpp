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

	libv::ui::Label lbl("version_lbl");
	lbl.style("main-menu.version-lbl");
	lbl.text(star::build.version_name);
	layers.add(std::move(lbl));

	return layers;
}

libv::ui::Component createSceneMainMenu(libv::Nexus& nexus) {
	const auto menuEntry = [&](std::string name, auto createFn) {
		libv::ui::Button btn;
		btn.style("main-menu.menu.entry");
		btn.text(std::move(name));

		if constexpr (std::is_same_v<decltype(createFn), std::nullptr_t>)
			btn.enable(false);
		else
			btn.event().submit.connect([nexus, createFn](libv::ui::Button& source) mutable {
				switchParentScene(source, "main", createFn(nexus));
			});

		return btn;
	};

	libv::ui::PanelLine golden_line{"golden-line"};
	golden_line.style("main-menu.golden-line");

	{
		libv::ui::PanelLine golden_box{"golden-box"};
		golden_box.style("main-menu.golden-box");

		{
			libv::ui::Gap golden_gap{"golden-gap"};
			golden_gap.style("main-menu.golden-gap");
			golden_line.add(std::move(golden_gap));

		} {
			libv::ui::PanelLine menu_box{"menu_box"};
			menu_box.style("main-menu.menu.panel");

			{
				libv::ui::Label lbl("title");
				lbl.style("main-menu.menu.title");
				lbl.text("IRIS STAR");
				menu_box.add(std::move(lbl));
			}

			menu_box.add(menuEntry("Single player", nullptr));
			menu_box.add(menuEntry("Multiplayer", nullptr));
			menu_box.add(menuEntry("Load", nullptr));
			menu_box.add(menuEntry("Mods", nullptr));
			menu_box.add(menuEntry("Settings", createSceneSettings));
			menu_box.add(menuEntry("Controls", createSceneControls));
			menu_box.add(menuEntry("Credits", nullptr));

			{
				libv::ui::Button btn;
				btn.style("main-menu.menu.entry");
				btn.text("Exit");
				btn.event().submit.connect([nexus] {
					nexus.broadcast_global(RequestClientExit());
				});
				menu_box.add(std::move(btn));
			}

			golden_box.add(std::move(menu_box));
		}

		golden_line.add(std::move(golden_box));
	}

	return layoutSceneMainMenu(std::move(golden_line));
}

// -------------------------------------------------------------------------------------------------

} // namespace star
