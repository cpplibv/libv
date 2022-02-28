// Project: libv, File: app/space/game/player.hpp

#pragma once

// std
//#include <string>
#include <libv/utility/entity/entity_ptr.hpp>
// pro
#include <space/sim/fwd.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class Player {
public:
//	std::string name = generate_random_name(5);
	libv::entity_ptr<Faction> faction;

public:
	explicit Player(libv::entity_ptr<Faction> faction);
	~Player();
};

//class PlayerController {
//public:
////	std::string name = generate_random_name(5);
//	libv::entity_ptr<Faction> faction;
//
//public:
//	explicit PlayerController(libv::entity_ptr<Faction> faction);
//	~PlayerController();
//};

// -------------------------------------------------------------------------------------------------

} // namespace space
