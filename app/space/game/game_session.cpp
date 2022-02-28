// Project: libv, File: app/space/game/game_session.cpp

// hpp
#include <space/game/game_session.hpp>
// libv
#include <libv/ctrl/controls.hpp>
// pro
#include <space/game/game_instance.hpp>
#include <space/network/network_client.hpp>
#include <space/network/network_server.hpp>
#include <space/sim/playout/playout.hpp>
#include <space/sim/simulation.hpp>
#include <space/sim/universe.hpp>
#include <space/sim/faction.hpp>
#include <space/view/scene_main.hpp>

#include <space/message/internal_events.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

GameSession::GameSession(libv::Nexus& nexus) :
		nexus(nexus),
//			universe(UniverseGenerationSettings{}),
//			universe(*simulation.universe),
		playout(),
		player(playout.simulation->universe->faction("Faction 1")) {
	register_nexus();
}

GameSession::GameSession(libv::Nexus& nexus, NetworkClient& network_client) :
		nexus(nexus),
//			universe(UniverseGenerationSettings{}),
//			universe(*simulation.universe),
		playout(network_client),
		player(playout.simulation->universe->faction("Faction 1")) {
	register_nexus();
}

GameSession::GameSession(libv::Nexus& nexus, NetworkServer& network_server) :
		nexus(nexus),
//			universe(UniverseGenerationSettings{}),
//			universe(*simulation.universe),
		playout(network_server),
		player(playout.simulation->universe->faction("Faction 1")) {
	register_nexus();
}

GameSession::~GameSession() {
	player.faction.reset(); // <<< Hazardous ownership
	unregister_nexus();
}

void GameSession::register_nexus() {
	nexus.connect<mc::RequestClearFleets>(this, [this] {
		playout.process<CTO_ClearFleets>();
	});
	nexus.connect<mc::RequestClearPlanets>(this, [this] {
		playout.process<CTO_ClearPlanets>();
	});
	nexus.connect<mc::RequestShuffle>(this, [this] {
		playout.process<CTO_Shuffle>();
	});
//	nexus.connect<mc::ChangeControlledFaction>(this, [this](const auto& e) {
//		e.faction;
	nexus.connect<mc::ChangeControlledFaction>(this, [this] {
		player = Player(playout.simulation->universe->faction("Faction 1"));
	});
	nexus.connect<mc::ClearControlledFaction>(this, [this] {
		player.faction.reset();
	});
}

void GameSession::unregister_nexus() {
	nexus.disconnect_all(this);
}

void GameSession::update(libv::time_duration delta_time) {
	playout.update(delta_time);
//	playout.update(simulation);
//	simulation.update(delta_time);
//	universe.update(delta_time);
}

// =================================================================================================

class SinglePlayer : public GameSession {
public:
	explicit SinglePlayer(GameThread& game_thread, libv::Nexus& nexus) :
		GameSession(nexus) {
		(void) game_thread;
	}

	virtual ~SinglePlayer() {
	}
};

// -------------------------------------------------------------------------------------------------

class MultiPlayerClient : public GameSession {
private:
	NetworkClient client;

public:
	MultiPlayerClient(GameThread& game_thread, libv::Nexus& nexus, std::string server_address, uint16_t server_port, User& user) :
		GameSession(nexus, client),
		// <<< network client reference is passed before its init ran, (should not be an issue now, but not nice)
		//client(std::move(server_address), server_port, game_thread, playout, universe, user) {
		client(std::move(server_address), server_port, game_thread, playout, user) {
	}

	virtual ~MultiPlayerClient() {
	}
};

// -------------------------------------------------------------------------------------------------

class MultiPlayerServer : public GameSession {
private:
	NetworkServer server;

public:
	MultiPlayerServer(GameThread& game_thread, libv::Nexus& nexus, uint16_t port, User& user) :
		GameSession(nexus, server),
		// <<< network server reference is passed before its init ran, (should not be an issue now, but not nice)
		//server(port, game_thread, playout, universe, user) {
		server(port, game_thread, playout, user) {
	}

	virtual ~MultiPlayerServer() {
	}
};

// -------------------------------------------------------------------------------------------------

//class MultiPlayerHeadless : public GameSession {
//	NetworkServer server;
//
//public:
//	MultiPlayerHeadless(GameInstance& game, libv::Nexus& nexus, uint16_t port) :
//		GameSession(nexus, server),
//		// <<< network client reference is passed before it init ran, (should not be an issue now, but not nice)
//		server(port, playout) {
//	}
//
//	virtual ~MultiPlayerHeadless() {
//	}
//};

// -------------------------------------------------------------------------------------------------

///// RemoteChatAdmin
//class HeadlessClient : public GameSession {
//	Playout tracking_only_playout;
//	SpaceSession session;
//	NetworkClient client;
//};

// =================================================================================================

std::shared_ptr<GameSession> createSinglePlayer(GameThread& game_thread, libv::Nexus& nexus) {
	return std::make_shared<SinglePlayer>(game_thread, nexus);
}

std::shared_ptr<GameSession> createMultiPlayerClient(GameThread& game_thread, libv::Nexus& nexus, std::string server_address, uint16_t server_port, User& user) {
	return std::make_shared<MultiPlayerClient>(game_thread, nexus, std::move(server_address), server_port, user);
}

std::shared_ptr<GameSession> createMultiPlayerServer(GameThread& game_thread, libv::Nexus& nexus, uint16_t port, User& user) {
	return std::make_shared<MultiPlayerServer>(game_thread, nexus, port, user);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
