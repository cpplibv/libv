// Project: libv, File: app/star/game/scene/scene_settings.cpp

// hpp
#include <star/game/scene/scenes.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_board.hpp>
#include <libv/ui/component/panel_line.hpp>
// pro
#include <star/game/scene/controls_layout.hpp>
#include <star/game/scene/utility.hpp>
//#include <star/game/config/client_config.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

libv::ui::Component createSceneControls(libv::Nexus& nexus) {
//	auto& config = requireBean<ClientConfig>(nexus, "Controls", "ClientConfig");

	auto line = libv::ui::PanelLine::ns("controls-line", "settings.main");

	line.add_sa<libv::ui::Label>("settings.title", "Controls");

	auto vis_line = line.add_ns<libv::ui::PanelLine>("visual-line", "controls.visual.line");

	auto board = vis_line.add_ns<libv::ui::PanelBoard>("keyboard-board", "controls.visual.board");

	// 22 column x 6 row + gaps
	Keyboard keyboard(48.f, 6.0f, 2.0f * 6.0f);

	for (const auto& button : keyboard.buttons) {
		auto key = libv::ui::Button::s("controls.visual.key");

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

	auto btn = line.add_sa<libv::ui::Button>("settings.ctrl", "Back");
	btn.event().submit.connect([nexus](libv::ui::Button& source) mutable {
		switchParentScene("main", source, createSceneMainMenu(nexus));
	});

	return line;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
