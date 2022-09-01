// Project: libv, File: app/space/view/scene_command_bar.cpp

// hpp
#include <space/view/scene_command_bar.hpp>
// libv
#include <libv/sys/start.hpp>
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/utility/nexus.hpp>
//#include <libv/ui/component/input_field.hpp>
// pro
#include <space/message/internal_events.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

libv::ui::Component SceneCommandBar::create(libv::Nexus2& nexus) {
	libv::ui::PanelLine cmd_bar("cmd-bar");
	cmd_bar.style("space.hud-bar.cmd.panel");

	{
		libv::ui::Button clear_fleets;
		clear_fleets.style("space.hud-bar.cmd.btn");
		clear_fleets.text("Clear Fleets");
		clear_fleets.event().submit.connect([&nexus]() {
			nexus.broadcast_global(mc::RequestClearFleets{});
		});
		cmd_bar.add(std::move(clear_fleets));

		libv::ui::Button clear_planets;
		clear_planets.style("space.hud-bar.cmd.btn");
		clear_planets.text("Clear Planets");
		clear_planets.event().submit.connect([&nexus]() {
			nexus.broadcast_global(mc::RequestClearPlanets{});
		});
		cmd_bar.add(std::move(clear_planets));

		libv::ui::Button clear_fleets_long;
		clear_fleets_long.style("space.hud-bar.cmd.btn");
		clear_fleets_long.text("Clear Fleets With Longer Label");
		clear_fleets_long.event().submit.connect([&nexus]() {
			nexus.broadcast_global(mc::RequestClearFleets{});
		});
		cmd_bar.add(std::move(clear_fleets_long));

		libv::ui::Button shuffle;
		shuffle.style("space.hud-bar.cmd.btn");
		shuffle.text("Shuffle");
		shuffle.event().submit.connect([&nexus]() {
			nexus.broadcast_global(mc::RequestShuffle{});
		});
		cmd_bar.add(std::move(shuffle));

		libv::ui::Button link;
		link.style("space.hud-bar.cmd.btn");
		link.text("Open link: corruptedai.com");
		link.event().submit.connect([]() {
			libv::sys::start_unsafe("https://corruptedai.com");
		});
//		libv::ui::InputField lin;
//		lin.style("space.hud-bar.cmd.btn");
//		lin.text("https://corruptedai.com");
//		cmd_bar.add(lin);
//		link.event().submit.connect([lin]() {
//			libv::sys::start_unsafe(lin.text());
//		});
		cmd_bar.add(std::move(link));
	}

	return cmd_bar;
}

// -------------------------------------------------------------------------------------------------

} // namespace space
