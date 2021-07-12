// Project: libv, File: app/space/game_instance.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
//#include <space/fwd.hpp>
// libv
#include <libv/utility/nexus.hpp>
// std
#include <memory>
// pro
#include <space/frame.hpp>
#include <space/game_session.hpp>
#include <space/message_control.hpp>
#include <space/player.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

class GameInstance {
private:
	SpaceFrame frame;
	// ^ controls

public:
	Player player;
	// Settings settings; // Or profile

	libv::Nexus nexus;

	std::shared_ptr<GameSession> game_session;
	std::optional<libv::ui::Component> game_session_ui;

public:
	void execute() {
		CameraBehaviour::register_controls(frame.controls);
		CameraBehaviour::bind_default_controls(frame.controls);
		CanvasBehaviour::register_controls(frame.controls);
		CanvasBehaviour::bind_default_controls(frame.controls);

//		enter_single_player();
		frame.show();
//		game_session->execute();
		frame.join();
	}

//	void update() {
//		game_session->update();
//	}

	void register_nexus() {
		nexus.connect<mc::RequestNameChange>(this, [this](const mc::RequestNameChange& event) {
			player.name = event.name;
			nexus.broadcast<mc::OnNameChange>();
		});

		nexus.connect<mc::RequestCreateClient>(this, [this](const mc::RequestCreateClient& event) {
			enter_multi_player_client(event.server_address, event.server_port);
			nexus.broadcast<mc::OnCreateClient>();
		});

		nexus.connect<mc::RequestCreateServer>(this, [this](const mc::RequestCreateServer& event) {
			enter_multi_player_server(event.server_port);
			nexus.broadcast<mc::OnCreateServer>();
		});

		nexus.connect<mc::RequestDestroyClient>(this, [this] {
			enter_single_player();
			nexus.broadcast<mc::OnDestroyClient>();
		});

		nexus.connect<mc::RequestDestroyServer>(this, [this] {
			enter_single_player();
			nexus.broadcast<mc::OnDestroyServer>();
		});


		// <<< These are game session requests
		nexus.connect<mc::RequestClearFleets>(this, [this] {
			game_session->playout.buffer.queue<app::CommandClearFleets>();
			// <<< Do this in the playout buffer
//			nexus.broadcast<mc::OnClearFleets>();
		});
		nexus.connect<mc::RequestShuffle>(this, [this] {
			game_session->playout.buffer.queue<app::CommandShuffle>();
			// <<< Do this in the playout buffer
//			nexus.broadcast<mc::OnClearFleets>();
		});
	}

	void unregister_nexus() {
		nexus.disconnect_all(this);
	}

public:
	GameInstance() {
		register_nexus();
//		frame.onContextUpdate();
	}

	~GameInstance() {
//		if (game_session_ui)
//			frame.ui.remove(*game_session_ui);
		game_session_ui.reset();
		unregister_nexus();
	}

public:
	void enter_single_player() {
		if (game_session_ui)
			frame.ui.remove(*game_session_ui);
		game_session.reset();
		game_session = createSinglePlayer(*this, nexus, frame.controls);
		game_session_ui = game_session->create_ui();
		frame.ui.add(*game_session_ui);
	}

	void enter_multi_player_client(std::string server_address, uint16_t server_port) {
		if (game_session_ui)
			frame.ui.remove(*game_session_ui);
		game_session.reset();
		game_session = createMultiPlayerClient(*this, nexus, frame.controls, std::move(server_address), server_port);
		game_session_ui = game_session->create_ui();
		frame.ui.add(*game_session_ui);
	}

	void enter_multi_player_server(uint16_t port) {
		if (game_session_ui)
			frame.ui.remove(*game_session_ui);
		game_session.reset();
		game_session = createMultiPlayerServer(*this, nexus, frame.controls, port);
		game_session_ui = game_session->create_ui();
		frame.ui.add(*game_session_ui);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
