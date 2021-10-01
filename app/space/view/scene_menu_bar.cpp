// Project: libv, File: app/space/view/scene_menu_bar.cpp

// hpp
#include <space/view/scene_menu_bar.hpp>
// libv
#include <libv/ui/attach_state.hpp>
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/utility/nexus.hpp>
// pro
#include <space/user.hpp>
#include <space/internal_events.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

libv::ui::Component SceneMenuBar::create(libv::Nexus& nexus, User& user) {
	libv::ui::PanelLine mp_bar("mp-bar");
	libv::ui::attach_state<SceneMenuBar>(mp_bar)(nexus, user).init(mp_bar);
	return mp_bar;
}

libv::ui::Component SceneMenuBar::init(libv::ui::PanelLine& mp_bar_main) {

	mp_bar_main.style("space.hud-bar.mp.main");

	libv::ui::Button title_space("title-space");
	title_space.style("space.title-space");
	title_space.text("SPACE");
	mp_bar_main.add(title_space);

	libv::ui::PanelLine mp_bar;
	mp_bar.style("space.hud-bar.mp.panel");
	mp_bar_main.add(mp_bar);

	{
		libv::ui::Label lbl_name("mp-name-lbl");
		lbl_name.style("space.hud-bar.mp.lbl");
		lbl_name.text("Name:");
		mp_bar.add(lbl_name);

		libv::ui::InputField in_name("mp-name-in");
		in_name.style("space.hud-bar.mp.input");
		in_name.text(user.name);
		mp_bar.add(in_name);

		lbl_state.style("space.hud-bar.mp.lbl");
		lbl_state.text("Status: Idle");
		mp_bar.add(lbl_state);

		btn_host.style("space.hud-bar.mp.btn");
		btn_host.text("Host");
		btn_host.event().submit.connect([this, in_name] {
			if (server_active) {
				nexus.broadcast(mc::RequestDestroyServer{});
			} else {
				nexus.broadcast(mc::RequestNameChange{in_name.text()});
				nexus.broadcast(mc::RequestCreateServer{uint16_t{25080}});
			}
		});
		mp_bar.add(btn_host);

		libv::ui::InputField in_server_address("mp-server-address-in");
		in_server_address.style("space.hud-bar.mp.input");
		in_server_address.text("rs0.corruptedai.com");

		btn_join.style("space.hud-bar.mp.btn");
		btn_join.text("Join");
		btn_join.event().submit.connect([this, in_name, in_server_address] {
			if (client_active) {
				nexus.broadcast(mc::RequestDestroyClient{});
			} else {
				nexus.broadcast(mc::RequestNameChange{in_name.text()});
				nexus.broadcast(mc::RequestCreateClient{in_server_address.text(), uint16_t{25080}});
			}
		});
		mp_bar.add(btn_join);

		mp_bar.add(in_server_address);

//		nexus.connect<mc::OnNameChange>(this, [this] mutable {
//			in_name.text(user.name);
//		});
		nexus.connect<mc::OnCreateClient>(this, [this] mutable {
			client_active = true;
			server_active = false;
			lbl_state.text("Running as Client [" + user.name + "]");
			btn_host.text("Host");
			btn_join.text("Disconnect");
		});
		nexus.connect<mc::OnCreateServer>(this, [this] mutable {
			client_active = false;
			server_active = true;
			lbl_state.text("Running as Server [" + user.name + "]");
			btn_host.text("Shutdown");
			btn_join.text("Join");
		});
		nexus.connect<mc::OnDestroyClient>(this, [this] mutable {
			client_active = false;
			lbl_state.text("Status: Idle");
			btn_host.text("Host");
			btn_join.text("Join");
		});
		nexus.connect<mc::OnDestroyServer>(this, [this] mutable {
			server_active = false;
			lbl_state.text("Status: Idle");
			btn_host.text("Host");
			btn_join.text("Join");
		});
	}

	return mp_bar;
}

SceneMenuBar::~SceneMenuBar() {
	nexus.disconnect_all(this);
}

// =================================================================================================
// =================================================================================================
// =================================================================================================
// <<< Do not forget about the prettier API (auto c = container.add<Component>())



//libv::ui::Component create_hud_styled_build_chain(
//		PlayoutDelayBuffer& playout_delay_buffer,
//		SpaceSession& space_session,
//		World& world,
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
//				world, space_session, playout_delay_buffer, camera_main, true);
//
//		out_canvas_main.emplace(canvas_main);
//	}
//
//	{
//		layers.add_ns<uic::CanvasAdaptorT<SpaceCanvas>>(
//				"canvas-mini", "space-canvas-mini",
//				world, space_session, playout_delay_buffer, camera_main, true);
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
//					.event().submit.connect([&, lbl_state, in_name] mutable {
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
//					.event().submit.connect([&, lbl_state, in_name] mutable {
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
// =================================================================================================
// =================================================================================================

// -------------------------------------------------------------------------------------------------

} // namespace app
