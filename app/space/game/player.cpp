// Project: libv, File: app/space/game/player.cpp

// hpp
#include <space/game/player.hpp>
// std
//#include <string>
//#include <libv/utility/entity/entity_ptr.hpp>
// pro
#include <space/sim/faction.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Player::Player(libv::entity_ptr<Faction> faction) :
		faction(std::move(faction)) {}

Player::~Player() {}

// -------------------------------------------------------------------------------------------------

} // namespace space
