// Project: libv, File: app/space/hud.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/ui/component/component.hpp>
#include <libv/ui/component/fwd.hpp>
// std
#include <optional>


namespace app {

// -------------------------------------------------------------------------------------------------

class CameraPlayer;
class NetworkClient;
class NetworkServer;
class PlayoutDelayBuffer;
class SpaceCanvas;
class SpaceSession;
class SpaceState;

// -------------------------------------------------------------------------------------------------

class ImplConsole;

//struct Console {
//	std::shared_ptr<ImplConsole> self;
//
//	inline Console(const Console&) noexcept = default;
//	inline Console& operator=(const Console&) & noexcept = default;
//	inline Console(Console&&) noexcept = default;
//	inline Console& operator=(Console&&) & noexcept = default;
//
////	void execute(std::string command);
//	void execute(std::string_view command) {
////		if (command == "clear_fleets") {
////			// clear_fleets
////			playout_delay_buffer.queue<app::CommandClearFleets>();
////
////		} else if (command.starts_with("name ")) {
////			// host <name>
////			player.name = arg(1);
////
////		} else if (command.starts_with("host ")) {
////			// host <port>
////			client.reset();
////			server.emplace(arg(1), playout_delay_buffer);
////
////		} else if (command.starts_with("join ")) {
////			// join <address> <port>
////			server.reset();
////			client.emplace(arg(1), arg(2), player.name, playout_delay_buffer);
////		}
//	}
//};

// -------------------------------------------------------------------------------------------------

libv::ui::Component create_hud(
		PlayoutDelayBuffer& playout_delay_buffer,
		SpaceSession& space_session,
		SpaceState& space_state,
		std::optional<NetworkClient>& client,
		std::optional<NetworkServer>& server,
		app::CameraPlayer& camera_main,
		app::CameraPlayer& camera_mini,

		std::optional<libv::ui::CanvasAdaptorT<SpaceCanvas>>& out_canvas_main);

// -------------------------------------------------------------------------------------------------

//struct HUD {
//	libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_main;
//	libv::ui::InputField name_in;
//	libv::ui::LabelImage status_lbl;
//	libv::ui::Button host_btn;
//	libv::ui::Button join_btn;
//
//public:
//	libv::ui::Component make_component();
//};

// -------------------------------------------------------------------------------------------------

} // namespace app
