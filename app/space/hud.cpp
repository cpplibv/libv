// Project: libv, File: app/space/hud.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/hud.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_float.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/parse/parse_size.hpp>
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

libv::ui::Component create_hud(
		PlayoutDelayBuffer& playout_delay_buffer,
		SpaceSession& space_session,
		SpaceState& space_state,
		std::optional<NetworkClient>& client,
		std::optional<NetworkServer>& server,
		app::CameraPlayer& camera_main,
		app::CameraPlayer& camera_mini,

		std::optional<libv::ui::CanvasAdaptorT<SpaceCanvas>>& out_canvas_main) {

	libv::ui::PanelFloat layers;

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
		libv::ui::PanelLine cmd_buttons;
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
		libv::ui::PanelLine mp_buttons;
		mp_buttons.anchor(libv::ui::Anchor::top_center);
		mp_buttons.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
		mp_buttons.size(libv::ui::parse_size_or_throw("D, D"));
		mp_buttons.spacing(5);
		mp_buttons.margin(5);
		{
			libv::ui::Button btn_host;
			btn_host.align_horizontal(libv::ui::AlignHorizontal::center);
			btn_host.align_vertical(libv::ui::AlignVertical::center);
			btn_host.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			btn_host.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
			btn_host.text("Host");
			btn_host.event().submit.connect([&]() {
				server.emplace(25080, playout_delay_buffer);
			});
			mp_buttons.add(std::move(btn_host));

			libv::ui::Button btn_join;
			btn_join.align_horizontal(libv::ui::AlignHorizontal::center);
			btn_join.align_vertical(libv::ui::AlignVertical::center);
			btn_join.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			btn_join.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
			btn_join.text("Join: rs0.corruptedai.com");
			btn_join.event().submit.connect([&]() {
				client.emplace("rs0.corruptedai.com", 25080, "Name", playout_delay_buffer);
			});
			mp_buttons.add(std::move(btn_join));
		}
		layers.add(mp_buttons);
	}

//	layers.add(pref_graph);
	layers.add(make_shader_error_overlay());

	return layers;
}

// -------------------------------------------------------------------------------------------------

} // namespace app
