// Project: libv, File: app/space/scene.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ui/component/fwd.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
#include <memory>
#include <optional>
// pro
#include <space/render.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Scene {
	GameInstance& game;
	GameSession& game_session;
	libv::Nexus& nexus;

	const void* track_ptr = nullptr;

//	enum class State {
//		idle,
//		client,
//		server,
//	};
//	State active_state = State::idle;
	bool client_active = false;
	bool server_active = false;

public:
	SpaceCanvas* canvas = nullptr;
	CameraPlayer camera;

public:
	Scene(GameInstance& game, GameSession& game_session, libv::Nexus& nexus);

	libv::ui::Component create_ui();

private:
	libv::ui::Component init_canvas_main();
	libv::ui::Component init_canvas_mini_bar();
	libv::ui::Component init_cmd_bar();
	libv::ui::Component init_mp_bar();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
