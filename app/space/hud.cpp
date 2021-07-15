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

//#include <space/signal/nexus.hpp>


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

//		Console console,
//		libv::Nexus nexus,

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
		libv::ui::PanelLine cmd_bar("cmd-bar");
		cmd_bar.anchor(libv::ui::Anchor::bottom_right);
		cmd_bar.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
		cmd_bar.size(libv::ui::parse_size_or_throw("D, D"));
		cmd_bar.spacing(5);
		cmd_bar.margin(5);

		{
			libv::ui::Button clear_fleets;
			clear_fleets.align_horizontal(libv::ui::AlignHorizontal::center);
			clear_fleets.align_vertical(libv::ui::AlignVertical::center);
			clear_fleets.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			clear_fleets.size(libv::ui::parse_size_or_throw("10pxD1r, 4pxD"));
			clear_fleets.text("Clear Fleets");
			clear_fleets.event().submit.connect([&playout_delay_buffer]() {
				playout_delay_buffer.queue<app::CommandClearFleets>();
//				console.execute("clear_fleets");
//				nexus.broadcast<ClearFleets>();
			});
			cmd_bar.add(std::move(clear_fleets));

			libv::ui::Button clear_fleets2;
			clear_fleets2.align_horizontal(libv::ui::AlignHorizontal::center);
			clear_fleets2.align_vertical(libv::ui::AlignVertical::center);
			clear_fleets2.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			clear_fleets2.size(libv::ui::parse_size_or_throw("10pxD1r, 4pxD"));
			clear_fleets2.text("Clear Fleets With Longer Label");
			clear_fleets2.event().submit.connect([&playout_delay_buffer]() {
				playout_delay_buffer.queue<app::CommandClearFleets>();
//				console.execute("clear_fleets");
//				nexus.broadcast<ClearFleets>();
			});
			cmd_bar.add(std::move(clear_fleets2));
		}
		layers.add(cmd_bar);
	}

	{
		libv::ui::PanelLine mp_bar("mp-bar");
		mp_bar.anchor(libv::ui::Anchor::top_center);
		mp_bar.orientation(libv::ui::Orientation::LEFT_TO_RIGHT);
		mp_bar.size(libv::ui::parse_size_or_throw("D, D"));
		mp_bar.spacing(5);
		mp_bar.margin(5);
		{
			libv::ui::LabelImage lbl_name("mp-name-lbl");
			lbl_name.align_horizontal(libv::ui::AlignHorizontal::center);
			lbl_name.align_vertical(libv::ui::AlignVertical::center);
			lbl_name.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			lbl_name.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
			lbl_name.text("Name:");
			mp_bar.add(lbl_name);

			libv::ui::InputField in_name("mp-name-in");
			in_name.align_horizontal(libv::ui::AlignHorizontal::center);
			in_name.align_vertical(libv::ui::AlignVertical::center);
			in_name.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			in_name.size(libv::ui::parse_size_or_throw("10pxD, 1r"));
			in_name.text(generate_random_name(4));
			mp_bar.add(in_name);

			libv::ui::LabelImage lbl_state("mp-state");
			lbl_state.align_horizontal(libv::ui::AlignHorizontal::center);
			lbl_state.align_vertical(libv::ui::AlignVertical::center);
			lbl_state.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			lbl_state.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
			lbl_state.text("Status: Idle");
			mp_bar.add(lbl_state);

			libv::ui::Button btn_host("mp-host");
			libv::ui::Button btn_join("mp-join");

			btn_host.align_horizontal(libv::ui::AlignHorizontal::center);
			btn_host.align_vertical(libv::ui::AlignVertical::center);
			btn_host.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			btn_host.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
			btn_host.text("Host");
			btn_host.event().submit.connect([&, lbl_state, in_name, btn_join](libv::ui::Button& self) mutable {
				if (server) {
					lbl_state.text("Status: Idle");
					self.text("Host");
					server.reset();
				} else {
					lbl_state.text("Running as Server [" + in_name.text() + "]");
					self.text("Shutdown");
					server.emplace(25080, playout_delay_buffer);
					client.reset();
//					console.execute_fmt("name {}", in_name.text());
//					console.execute_fmt("host {}", 25080);
//					nexus.broadcast<NameChange>(in_name.text());
//					nexus.broadcast<NetworkHost>(25080);
					btn_join.text("Join: rs0.corruptedai.com");
				}
			});
			mp_bar.add(btn_host);

			btn_join.align_horizontal(libv::ui::AlignHorizontal::center);
			btn_join.align_vertical(libv::ui::AlignVertical::center);
			btn_join.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
			btn_join.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
			btn_join.text("Join: rs0.corruptedai.com");
			btn_join.event().submit.connect([&, lbl_state, in_name, btn_host](libv::ui::Button& self) mutable {
				if (client) {
					lbl_state.text("Status: Idle");
					self.text("Join: rs0.corruptedai.com");
					client.reset();
				} else {
					auto name = in_name.text();
					lbl_state.text("Running as Client [" + name + "]");
					self.text("Disconnect");
					client.emplace("rs0.corruptedai.com", 25080, name, playout_delay_buffer);
					server.reset();
//					console.execute_fmt("name {}", name);
//					console.execute_fmt("join {} {}", "rs0.corruptedai.com", 25080);
//					nexus.broadcast<NameChange>(name);
//					nexus.broadcast<NetworkJoin>("rs0.corruptedai.com", 25080);
					btn_host.text("Host");
				}
			});
			mp_bar.add(btn_join);

//			nexus.listen<NetworkHost>(hud_id, [=] {
//				btn_host.text("Shutdown");
//				btn_join.text("Join: rs0.corruptedai.com");
//			});
//			nexus.listen<NetworkJoin>(hud_id, [=] {
//				btn_host.text("Host");
//				btn_join.text("Shutdown");
//			});
		}
		layers.add(mp_bar);
	}

//	layers.add(pref_graph);
	layers.add(make_shader_error_overlay());

//	layers.event().detach.connect([] {
//		nexus.disconnect(hud_id);
//	});

	return layers;
}

// =================================================================================================

//libv::ui::Component create_hud_styled(
//		PlayoutDelayBuffer& playout_delay_buffer,
//		SpaceSession& space_session,
//		SpaceState& space_state,
//		std::optional<NetworkClient>& client,
//		std::optional<NetworkServer>& server,
//		app::CameraPlayer& camera_main,
//		app::CameraPlayer& camera_mini,
//
//		std::optional<libv::ui::CanvasAdaptorT<SpaceCanvas>>& out_canvas_main) {
//
//	libv::ui::PanelFloat layers("hud");
//
//	{
//		libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_main("canvas-main", space_state, space_session, playout_delay_buffer, camera_main, true);
//		style("space-canvas-main");
//		layers.add(canvas_main);
//
//		out_canvas_main.emplace(canvas_main);
//	}
//
//	{
//		libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_mini("canvas-mini", space_state, space_session, playout_delay_buffer, camera_mini, false);
//		style("space-canvas-mini");
//		layers.add(canvas_mini);
//	}
//
//	{
//		libv::ui::PanelLine cmd_bar("cmd-bar");
//		style("space.hud-bar.cmd");
//
//		{
//			libv::ui::Button clear_fleets;
//			style("space.hud-bar.cmd.button");
//			clear_fleets.text("Clear Fleets");
//			clear_fleets.event().submit.connect([&playout_delay_buffer]() {
//				playout_delay_buffer.queue<app::CommandClearFleets>();
//			});
//			cmd_bar.add(std::move(clear_fleets));
//
////			libv::ui::Button clear_fleets2;
////			style("space.hud-bar.cmd.button");
////			clear_fleets2.text("Clear Fleets Clear Fleets");
////			clear_fleets2.event().submit.connect([&playout_delay_buffer]() {
////				playout_delay_buffer.queue<app::CommandClearFleets>();
////			});
////			cmd_bar.add(std::move(clear_fleets2));
//		}
//		layers.add(cmd_bar);
//	}
//
//	{
//		libv::ui::PanelLine mp_bar("mp-bar");
//		style("space.hud-bar.mp");
//
//		{
//			libv::ui::LabelImage lbl_name("mp-name-lbl");
//			style("space.hud-bar.mp.lbl");
//			lbl_name.text("Name:");
//			mp_bar.add(lbl_name);
//
//			libv::ui::InputField in_name("mp-name-in");
//			style("space.hud-bar.mp.input");
//			in_name.text(generate_random_name(4));
//			mp_bar.add(in_name);
//
//			libv::ui::LabelImage lbl_state("mp-state");
//			style("space.hud-bar.mp.lbl");
//			lbl_state.text("Idle");
//			mp_bar.add(lbl_state);
//
//			libv::ui::Button btn_host("mp-host");
//			style("space.hud-bar.mp.btn");
//			btn_host.text("Host");
//			btn_host.event().submit.connect([&, lbl_state, in_name]() mutable {
//				if (server) {
//					lbl_state.text("Idle");
//					server.reset();
//				} else {
//					lbl_state.text("Running as Server [" + in_name.text() + "]");
//					server.emplace(25080, playout_delay_buffer);
//					client.reset();
//				}
//			});
//			mp_bar.add(btn_host);
//
//			libv::ui::Button btn_join("mp-join");
//			style("space.hud-bar.mp.btn");
//			btn_join.text("Join: rs0.corruptedai.com");
//			btn_join.event().submit.connect([&, lbl_state, in_name]() mutable {
//				if (client) {
//					lbl_state.text("Idle");
//					client.reset();
//				} else {
//					auto name = in_name.text();
//					lbl_state.text("Running as Client [" + name + "]");
//					client.emplace("rs0.corruptedai.com", 25080, name, playout_delay_buffer);
//					server.reset();
//				}
//			});
//			mp_bar.add(btn_join);
//		}
//		layers.add(mp_bar);
//	}
//
////	layers.add(pref_graph);
//	layers.add(make_shader_error_overlay());
//
//	return layers;
//}
//
//// =================================================================================================
//
//libv::ui::Component create_hud_styled_build_chain(
//		PlayoutDelayBuffer& playout_delay_buffer,
//		SpaceSession& space_session,
//		SpaceState& space_state,
//		std::optional<NetworkClient>& client,
//		std::optional<NetworkServer>& server,
//		app::CameraPlayer& camera_main,
//		app::CameraPlayer& camera_mini,
//
//		std::optional<libv::ui::CanvasAdaptorT<SpaceCanvas>>& out_canvas_main) {
//
//	namespace uic = libv::ui::component;
//
//	auto layers = libv::ui::PanelFloat("hud");
//
//	{
//		auto canvas_main = layers.add_ns<uic::CanvasAdaptorT<SpaceCanvas>>(
//				"canvas-main", "space-canvas-main",
//				space_state, space_session, playout_delay_buffer, camera_main, true);
//
//		out_canvas_main.emplace(canvas_main);
//	}
//
//	{
//		layers.add_ns<uic::CanvasAdaptorT<SpaceCanvas>>(
//				"canvas-mini", "space-canvas-mini",
//				space_state, space_session, playout_delay_buffer, camera_main, true);
//	}
//
//	{
//		const auto& cmd_bar = layers.add_ns<libv::ui::PanelLine>("cmd-bar", "space.hud-bar.cmd");
//		{
//			cmd_bar.add_s<libv::ui::Button>("space.hud-bar.cmd.button")
//					.text("Clear Fleets")
//					.event().submit.connect([&playout_delay_buffer]() {
//						playout_delay_buffer.queue<app::CommandClearFleets>();
//					});
//
//			cmd_bar.add_s<libv::ui::Button>("space.hud-bar.cmd.button")
//					.text("Clear Fleets With Longer Label")
//					.event().submit.connect([&playout_delay_buffer]() {
//						playout_delay_buffer.queue<app::CommandClearFleets>();
//					});
//		}
//	}
//
//	{
//		const auto& mp_bar = layers.add_ns<uic::PanelLine>("mp-bar", "space.hud-bar.mp");
//		{
//			auto lbl_state = mp_bar.add_ns<uic::LabelImage>("name-lbl", "space.hud-bar.mp.lbl")
//					.text("Name:");
//
//			auto in_name = mp_bar.add_ns<uic::InputField>("name-in", "space.hud-bar.mp.input")
//					.text(generate_random_name(4));
//
//			mp_bar.add_ns<uic::LabelImage>("state", "space.hud-bar.mp.lbl")
//					.text("Idle");
//
//			mp_bar.add_ns<uic::LabelImage>("host-btn", "space.hud-bar.mp.btn")
//					.text("Host")
//					.event().submit.connect([&, lbl_state, in_name]() mutable {
//						if (server) {
//							lbl_state.text("Idle");
//							server.reset();
//						} else {
//							lbl_state.text("Running as Server [" + in_name.text() + "]");
//							server.emplace(25080, playout_delay_buffer);
//							client.reset();
//						}
//					});
//
//			mp_bar.add_ns<uic::LabelImage>("join-btn", "space.hud-bar.mp.btn")
//					.text("Join: rs0.corruptedai.com")
//					.event().submit.connect([&, lbl_state, in_name]() mutable {
//						if (client) {
//							lbl_state.text("Idle");
//							client.reset();
//						} else {
//							auto name = in_name.text();
//							lbl_state.text("Running as Client [" + name + "]");
//							client.emplace("rs0.corruptedai.com", 25080, name, playout_delay_buffer);
//							server.reset();
//						}
//					});
//		}
//	}
//
//	layers.add(make_shader_error_overlay());
//
//	return layers;
//}

// =================================================================================================

// -------------------------------------------------------------------------------------------------

} // namespace app
