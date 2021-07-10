// Project: libv, File: app/space/hud.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/hud.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/label_image.hpp>
#include <libv/ui/component/panel_float.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/utility/uniform_distribution.hpp>
//#include <libv/ui/component/label.hpp>
// std
#include <random>
// pro
#include <space/command.hpp>
#include <space/make_shader_error_overlay.hpp>
#include <space/playout.hpp>
#include <space/network_client.hpp>
#include <space/network_server.hpp>
#include <space/render.hpp>
//#include <space/camera.hpp>
//#include <space/state.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

std::string generate_random_name(uint64_t seed, size_t length) {
	static constexpr std::string_view char_set =
			"0123456789"
			"abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	auto rng = std::mt19937_64{seed};
	auto dist = libv::make_uniform_distribution_exclusive(char_set.size());

	std::string str;
	str.resize(length);

	for (auto& c : str)
		c = char_set[dist(rng)];

	return str;
}

std::string generate_random_name(size_t length) {
	return generate_random_name(std::random_device{}(), length);
}

// -------------------------------------------------------------------------------------------------

libv::ui::Component create_hud(
		PlayoutDelayBuffer& playout_delay_buffer,
		SpaceSession& space_session,
		SpaceState& space_state,
		std::optional<NetworkClient>& client,
		std::optional<NetworkServer>& server,
		app::CameraPlayer& camera_main,
		app::CameraPlayer& camera_mini,

		std::optional<libv::ui::CanvasAdaptorT<SpaceCanvas>>& out_canvas_main) {

	libv::ui::PanelFloat layers("hud");

	{
		libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_main("canvas-main", space_state, space_session, playout_delay_buffer, camera_main, true);
		layers.add(canvas_main);

		out_canvas_main.emplace(canvas_main);
	}

	{
		libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_mini("canvas-mini", space_state, space_session, playout_delay_buffer, camera_mini, false);
		canvas_mini.size(libv::ui::parse_size_or_throw("25%, 15%"));
		canvas_mini.margin(10);
		canvas_mini.anchor(libv::ui::Anchor::center_right);
		layers.add(canvas_mini);
	}

	{
		libv::ui::PanelLine cmd_buttons("cmd-bar");
		cmd_buttons.anchor(libv::ui::Anchor::bottom_right);
		cmd_buttons.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
		cmd_buttons.size(libv::ui::parse_size_or_throw("D, D"));
		cmd_buttons.spacing(5);
		cmd_buttons.margin(5);

		{
			libv::ui::Button clear_fleets;
			clear_fleets.align_horizontal(libv::ui::AlignHorizontal::center);
			clear_fleets.align_vertical(libv::ui::AlignVertical::center);
			clear_fleets.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			clear_fleets.size(libv::ui::parse_size_or_throw("10pxD1r, 4pxD"));
			clear_fleets.text("Clear Fleets");
			clear_fleets.event().submit.connect([&playout_delay_buffer]() {
				playout_delay_buffer.queue<app::CommandClearFleets>();
			});
			cmd_buttons.add(std::move(clear_fleets));

//			libv::ui::Button clear_fleets2;
//			clear_fleets2.align_horizontal(libv::ui::AlignHorizontal::center);
//			clear_fleets2.align_vertical(libv::ui::AlignVertical::center);
//			clear_fleets2.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
//			clear_fleets2.size(libv::ui::parse_size_or_throw("10pxD1r, 4pxD"));
//			clear_fleets2.text("Clear Fleets Clear Fleets");
//			clear_fleets2.event().submit.connect([&playout_delay_buffer]() {
//				playout_delay_buffer.queue<app::CommandClearFleets>();
//			});
//			cmd_buttons.add(std::move(clear_fleets2));
		}
		layers.add(cmd_buttons);
	}

	{
		libv::ui::PanelLine mp_buttons("mp-bar");
		mp_buttons.anchor(libv::ui::Anchor::top_center);
		mp_buttons.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
		mp_buttons.size(libv::ui::parse_size_or_throw("D, D"));
		mp_buttons.spacing(5);
		mp_buttons.margin(5);
		{
			libv::ui::LabelImage lbl_name("mp-name-lbl");
			lbl_name.align_horizontal(libv::ui::AlignHorizontal::center);
			lbl_name.align_vertical(libv::ui::AlignVertical::center);
			lbl_name.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			lbl_name.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
			lbl_name.text("Name:");
			mp_buttons.add(lbl_name);

			libv::ui::InputField in_name("mp-name-in");
			in_name.align_horizontal(libv::ui::AlignHorizontal::center);
			in_name.align_vertical(libv::ui::AlignVertical::center);
			in_name.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			in_name.size(libv::ui::parse_size_or_throw("10pxD, 1r"));
			in_name.text(generate_random_name(4));
			mp_buttons.add(in_name);

			libv::ui::LabelImage lbl_state("mp-state");
			lbl_state.align_horizontal(libv::ui::AlignHorizontal::center);
			lbl_state.align_vertical(libv::ui::AlignVertical::center);
			lbl_state.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			lbl_state.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
			lbl_state.text("Idle");
			mp_buttons.add(lbl_state);

			libv::ui::Button btn_host("mp-host");
			btn_host.align_horizontal(libv::ui::AlignHorizontal::center);
			btn_host.align_vertical(libv::ui::AlignVertical::center);
			btn_host.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			btn_host.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
			btn_host.text("Host");
			btn_host.event().submit.connect([&, lbl_state, in_name]() mutable {
				if (server) {
					lbl_state.text("Idle");
					server.reset();
				} else {
					lbl_state.text("Running as Server [" + in_name.text() + "]");
					server.emplace(25080, playout_delay_buffer);
					client.reset();
				}
			});
			mp_buttons.add(btn_host);

			libv::ui::Button btn_join("mp-join");;
			btn_join.align_horizontal(libv::ui::AlignHorizontal::center);
			btn_join.align_vertical(libv::ui::AlignVertical::center);
			btn_join.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			btn_join.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
			btn_join.text("Join: rs0.corruptedai.com");
			btn_join.event().submit.connect([&, lbl_state, in_name]() mutable {
				if (client) {
					lbl_state.text("Idle");
					client.reset();
				} else {
					auto name = in_name.text();
					lbl_state.text("Running as Client [" + name + "]");
					client.emplace("rs0.corruptedai.com", 25080, name, playout_delay_buffer);
					server.reset();
				}
			});
			mp_buttons.add(btn_join);
		}
		layers.add(mp_buttons);
	}

//	layers.add(pref_graph);
	layers.add(make_shader_error_overlay());

	return layers;
}

// -------------------------------------------------------------------------------------------------

} // namespace app
