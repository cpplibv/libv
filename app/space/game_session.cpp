// Project: libv, File: app/space/game_session.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/game_session.hpp>
// libv
#include <libv/ctrl/controls.hpp>
// pro
#include <space/game_instance.hpp>
#include <space/network_client.hpp>
#include <space/network_server.hpp>
#include <space/playout.hpp>
#include <space/view/scene_main_ui.hpp>
#include <space/universe/universe.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct SinglePlayer : GameSession {
//	Render render;
//	Canvas canvas;
//	Scene scene;
	libv::ctrl::Controls& controls;
//	CameraPlayer camera;

public:
	SinglePlayer(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls) :
		GameSession(std::make_unique<PlayoutSinglePlayer>()),
//		scene(game, *this, nexus),
		controls(controls) {
		game.main_ui_stage.add_game_scene(game, *this, nexus, controls);
	}

	virtual void update(libv::ui::time_duration delta_time) override {
		playout->update(universe);
		universe.update(delta_time);
	}

	virtual ~SinglePlayer() {
	}
};

struct MultiPlayerClient : GameSession {
//	Render render;
//	Canvas canvas;
//	Scene scene;
	libv::ctrl::Controls& controls;
//	CameraPlayer camera;
	NetworkClient client;

	Lobby lobby;

public:
	MultiPlayerClient(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls, std::string server_address, uint16_t server_port) :
		GameSession(std::make_unique<PlayoutMultiPlayerClient>(client)),
		// <<< network client reference is passed before it init ran, (should not be an issue now, but not nice)
//		scene(game, *this, nexus),
		controls(controls),
		client(std::move(server_address), server_port, game.player.name, *playout) {
		game.main_ui_stage.add_game_scene(game, *this, nexus, controls);
	}

	virtual ~MultiPlayerClient() {
	}

	virtual void update(libv::ui::time_duration delta_time) override {
		playout->update(universe, lobby);
		universe.update(delta_time);
	}
};

struct MultiPlayerServer : GameSession {
//	Render render;
//	Canvas canvas;
//	Scene scene;
	libv::ctrl::Controls& controls;
//	CameraPlayer camera;
	NetworkServer server;

	Lobby lobby;

public:
	MultiPlayerServer(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls, uint16_t port) :
		GameSession(std::make_unique<PlayoutMultiPlayerServer>(server)),
		// <<< network client reference is passed before it init ran, (should not be an issue now, but not nice)
//		scene(game, *this, nexus),
		controls(controls),
		server(port, *playout) {
		game.main_ui_stage.add_game_scene(game, *this, nexus, controls);
	}

	virtual ~MultiPlayerServer() {
	}

	virtual void update(libv::ui::time_duration delta_time) override {
		playout->update(universe, lobby);
		universe.update(delta_time);
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
