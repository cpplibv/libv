// Project: libv, File: app/star/game/scene/scene_main_menu.cpp

// hpp
#include <star/game/scene/scene_main_menu.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/gap.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/utility/nexus.hpp>
// pro
#include <star/game/control/requests.hpp>
#include <star/game/game_client.hpp>
#include <star/game/scene/scene_controls.hpp>
#include <star/game/scene/scene_root.hpp>
#include <star/game/scene/scene_settings.hpp>
#include <star/version.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

libv::ui::Component createSceneMainMenu(GameClient& gameClient) {
	//libv::ui::PanelAnchor layers{"layers"};
	libv::ui::PanelAnchor layers;

	const auto menuEntry = [&gameClient](std::string name, auto createFn) {
		libv::ui::Button btn;
		btn.style("main-menu.menu.entry");
		btn.text(std::move(name));

		if constexpr (std::is_same_v<decltype(createFn), std::nullptr_t>)
			btn.enable(false);
		else
			btn.event().submit.connect([&gameClient, createFn](libv::ui::Button& source) {
						source.event().global.fire<SwitchPrimaryScene>(createFn(gameClient));
					});

		return btn;
	};

	{
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
					btn.event().submit.connect([&gameClient] {
						gameClient.nexus().broadcast_global(RequestClientExit());
					});
					menu_box.add(std::move(btn));
				}

				golden_box.add(std::move(menu_box));
			}

			golden_line.add(std::move(golden_box));
		}

		layers.add(std::move(golden_line));

	} {
		libv::ui::Label lbl("version_lbl");
		lbl.style("main-menu.version-lbl");
		lbl.text(star::build.version_name);
		layers.add(std::move(lbl));
	}

	return layers;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
