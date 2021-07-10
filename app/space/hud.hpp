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

} // namespace app
