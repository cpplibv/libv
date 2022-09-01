// Project: libv, File: app/space/game/game_session.hpp

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
#include <space/game/player.hpp>
#include <space/sim/playout/playout.hpp>
//#include <space/sim/gen/generation_settings.hpp>
//#include <space/sim/universe.hpp>
//#include <space/sim/simulation.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class GameSession {
public:
	libv::Nexus2& nexus;

	Playout playout;
	Player player;
//	PlayerController controller;
//	FactionController controller;
//	PlayerController playerController;

public:
	explicit GameSession(libv::Nexus2& nexus);
	GameSession(libv::Nexus2& nexus, NetworkClient& network_client);
	GameSession(libv::Nexus2& nexus, NetworkServer& network_server);
	virtual ~GameSession();

private:
	void register_nexus();
	void unregister_nexus();

public:
	void update(libv::time_duration delta_time);
};

std::shared_ptr<GameSession> createSinglePlayer(GameThread& game_thread, libv::Nexus2& nexus);
std::shared_ptr<GameSession> createMultiPlayerClient(GameThread& game_thread, libv::Nexus2& nexus, std::string server_address, uint16_t server_port, User& user);
std::shared_ptr<GameSession> createMultiPlayerServer(GameThread& game_thread, libv::Nexus2& nexus, uint16_t port, User& user);

// -------------------------------------------------------------------------------------------------

} // namespace space
