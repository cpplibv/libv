// Project: libv, File: app/space/game_session.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/ui/component/fwd.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
#include <memory>
#include <string>
// pro
#include <space/playout.hpp>
#include <space/universe.hpp>
#include <space/session.hpp> // <<< Nope


namespace app {

// -------------------------------------------------------------------------------------------------

struct GameSession {
	Universe universe;
	Playout playout;

	SpaceSession session; // <<< Nope

public:
	virtual ~GameSession() = default;

public:
	virtual void update(libv::ui::time_duration delta_time) = 0;
	[[nodiscard]] virtual libv::ui::Component create_ui() = 0;
};

std::shared_ptr<GameSession> createSinglePlayer(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls);
std::shared_ptr<GameSession> createMultiPlayerClient(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls, std::string server_address, uint16_t server_port);
std::shared_ptr<GameSession> createMultiPlayerServer(GameInstance& game, libv::Nexus& nexus, libv::ctrl::Controls& controls, uint16_t port);

// -------------------------------------------------------------------------------------------------

} // namespace app