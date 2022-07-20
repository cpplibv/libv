// Project: libv, File: app/star/game/scene/scene_settings.cpp

// hpp
#include <star/game/scene/scene_controls.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/ui/component/button.hpp>
//#include <libv/ui/component/gap.hpp>
//#include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
//#include <libv/ui/component/panel_grid.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/panel_board.hpp>
// pro
#include <star/game/scene/controls_layout.hpp>
// Only for scene switching and manipulation, there should be a better way:
#include <star/game/game_client.hpp>
#include <star/game/scene/scene_main_menu.hpp>
#include <star/game/scene/scene_root.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

libv::ui::Component createSceneControls(GameClient& gameClient) {
	auto config = gameClient.config();

	libv::ui::PanelLine line{"controls-line"};
	line.style("settings.main");

	libv::ui::Label title;
	title.text("Controls");
	title.style("settings.title");
	line.add(std::move(title));

	{
		libv::ui::PanelLine vis_line{"visual-line"};
		vis_line.style("controls.visual.line");

		{
			libv::ui::PanelBoard board{"keyboard-board"};
			board.style("controls.visual.board");

			// 22 column x 6 row + gaps
			Keyboard keyboard(48.f, 6.0f, 2.0f * 6.0f);

			for (const auto& button : keyboard.buttons) {
				libv::ui::Button key;
				key.style("controls.visual.key");

				if (button.input == libv::ctrl::Keycode::PrintScreen)
					key.text("Print\nScreen");

				else if (button.input == libv::ctrl::Keycode::ScrollLock)
					key.text("Scroll\nLock");
				else if (button.input == libv::ctrl::Keycode::NumLock)
					key.text("Num\nLock");
				else if (button.input == libv::ctrl::Keycode::CapsLock)
					key.text("Caps Lock");

				else if (button.input == libv::ctrl::Keycode::PageUp)
					key.text("Page\nUp");
				else if (button.input == libv::ctrl::Keycode::PageDown)
					key.text("Page\nDown");

				else if (button.input == libv::ctrl::Keycode::KPEnter)
					key.text("KP\nEnter");

				else if (button.input == libv::ctrl::Keycode::AltLeft)
					key.text("Left\nAlt");
				else if (button.input == libv::ctrl::Keycode::AltRight)
					key.text("Right\nAlt");
				else if (button.input == libv::ctrl::Keycode::ShiftLeft)
					key.text("Left\nShift");
				else if (button.input == libv::ctrl::Keycode::ShiftRight)
					key.text("Right\nShift");
				else if (button.input == libv::ctrl::Keycode::ControlLeft)
					key.text("Left\nControl");
				else if (button.input == libv::ctrl::Keycode::ControlRight)
					key.text("Right\nControl");
				else if (button.input == libv::ctrl::Keycode::SuperLeft)
					key.text("Left\nSuper");
				else if (button.input == libv::ctrl::Keycode::SuperRight)
					key.text("Right\nSuper");

				else
					key.text(button.input.to_string_symbol());

				board.add(std::move(key), button.position, button.size);
			}
			vis_line.add(std::move(board));
		}

		line.add(std::move(vis_line));
	}

	libv::ui::Button btn;
	btn.style("settings.ctrl");
	btn.text("Back");
	btn.event().submit.connect_system([gameClient = &gameClient](libv::ui::Button& source) {
		source.event().global.fire<SwitchPrimaryScene>(createSceneMainMenu(*gameClient));
	});
	line.add(std::move(btn));

	return line;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
