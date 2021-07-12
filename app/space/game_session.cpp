// Project: libv, File: app/space/game_session.hpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/game_session.hpp>
// libv
#include <libv/ctrl/controls.hpp>
// pro
#include <space/game_instance.hpp>
#include <space/network_client.hpp>
#include <space/network_server.hpp>
#include <space/playout.hpp>
#include <space/scene.hpp>
#include <space/universe.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

// TODO P1: Enter / leave on canvas based on focus-unfocus
//controls.context_enter<app::BaseCameraOrbit>(&__camera__);
//controls.context_enter<app::SpaceCanvas>(&__canvas__);

struct SinglePlayer : GameSession {
//	Render render;
//	Canvas canvas;
	Scene scene;
	libv::ctrl::Controls& controls;
//	CameraPlayer camera;

	SinglePlayer(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls) :
		scene(game, *this, nexus),
		controls(controls) {
		controls.context_enter<app::BaseCameraOrbit>(&scene.camera);
//		controls.context_enter<app::SpaceCanvas>(scene.canvas);
	}

	virtual void update(libv::ui::time_duration delta_time) override {
		playout.buffer.update(universe, session);
		universe.update(delta_time);
	}

	[[nodiscard]] virtual libv::ui::Component create_ui() override {
		auto c = scene.create_ui();
		controls.context_enter<app::SpaceCanvas>(scene.canvas); // <<<
		return c;
	}

	virtual ~SinglePlayer() {
		controls.context_leave<app::BaseCameraOrbit>();
		controls.context_leave<app::SpaceCanvas>();
//		controls.context_leave_if_matches<app::SpaceCanvas>(scene.canvas); // <<<
	}
};

struct MultiPlayerClient : GameSession {
//	Render render;
//	Canvas canvas;
	Scene scene;
	libv::ctrl::Controls& controls;
//	CameraPlayer camera;
	NetworkClient client;

	MultiPlayerClient(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls, std::string server_address, uint16_t server_port) :
		scene(game, *this, nexus),
		controls(controls),
		client(std::move(server_address), server_port, game.player.name, playout) {
		controls.context_enter<app::BaseCameraOrbit>(&scene.camera);
		controls.context_enter<app::SpaceCanvas>(scene.canvas);
	}

	virtual ~MultiPlayerClient() {
		controls.context_leave<app::BaseCameraOrbit>();
		controls.context_leave<app::SpaceCanvas>();
	}

	virtual void update(libv::ui::time_duration delta_time) override {
		playout.buffer.update(universe, session);
		universe.update(delta_time);
	}

	[[nodiscard]] virtual libv::ui::Component create_ui() override {
		return scene.create_ui();
	}
};

struct MultiPlayerServer : GameSession {
//	Render render;
//	Canvas canvas;
	Scene scene;
	libv::ctrl::Controls& controls;
//	CameraPlayer camera;
	NetworkServer server;

	MultiPlayerServer(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls, uint16_t port) :
		scene(game, *this, nexus),
		controls(controls),
		server(port, playout) {
		controls.context_enter<app::BaseCameraOrbit>(&scene.camera);
		controls.context_enter<app::SpaceCanvas>(scene.canvas);
	}

	virtual ~MultiPlayerServer() {
		controls.context_leave<app::BaseCameraOrbit>();
		controls.context_leave<app::SpaceCanvas>();
	}

	virtual void update(libv::ui::time_duration delta_time) override {
		playout.buffer.update(universe, session);
		universe.update(delta_time);
	}

	[[nodiscard]] virtual libv::ui::Component create_ui() override {
		return scene.create_ui();
	}
};

//struct MultiPlayerHeadless : GameSession {
//	NetworkServer server;
//
//	MultiPlayerHeadless(GameInstance& game, libv::Nexus& nexus, uint16_t port) {}
//};

//struct RemoteChatAdmin : GameSession {
//	Playout tracking_only_playout;
//	SpaceSession session;
//	NetworkClient client;
//};

// -------------------------------------------------------------------------------------------------

std::shared_ptr<GameSession> createSinglePlayer(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls) {
	return std::make_shared<SinglePlayer>(game, nexus, controls);
}

std::shared_ptr<GameSession> createMultiPlayerClient(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls, std::string server_address, uint16_t server_port) {
	return std::make_shared<MultiPlayerClient>(game, nexus, controls, std::move(server_address), server_port);
}

std::shared_ptr<GameSession> createMultiPlayerServer(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls, uint16_t port) {
	return std::make_shared<MultiPlayerServer>(game, nexus, controls, port);
}

// -------------------------------------------------------------------------------------------------

} // namespace app
