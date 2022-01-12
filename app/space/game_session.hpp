// Project: libv, File: app/space/game_session.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/ui/component/fwd.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
#include <memory>
#include <string>
// pro
#include <space/playout.hpp>
#include <space/sim/gen/generation_settings.hpp>
#include <space/sim/universe.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class GameSession {
private:
	libv::Nexus& nexus;

public:
	Universe universe;
	Playout playout;

public:
	explicit inline GameSession(libv::Nexus& nexus) :
			nexus(nexus),
			universe(GalaxyGenerationSettings{}),
			playout() {
		register_nexus();
	}

	explicit inline GameSession(libv::Nexus& nexus, NetworkClient& network_client) :
			nexus(nexus),
			universe(GalaxyGenerationSettings{}),
			playout(network_client) {
		register_nexus();
	}

	explicit inline GameSession(libv::Nexus& nexus, NetworkServer& network_server) :
			nexus(nexus),
			universe(GalaxyGenerationSettings{}),
			playout(network_server) {
		register_nexus();
	}

	virtual ~GameSession() {
		unregister_nexus();
	}

private:
	void register_nexus();
	void unregister_nexus();

public:
	void update(libv::time_duration delta_time);
};

std::shared_ptr<GameSession> createSinglePlayer(GameThread& game_thread, libv::Nexus& nexus);
std::shared_ptr<GameSession> createMultiPlayerClient(GameThread& game_thread, libv::Nexus& nexus, std::string server_address, uint16_t server_port, User& user);
std::shared_ptr<GameSession> createMultiPlayerServer(GameThread& game_thread, libv::Nexus& nexus, uint16_t port, User& user);

// -------------------------------------------------------------------------------------------------

} // namespace space
