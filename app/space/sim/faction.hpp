// Project: libv, File: src/space/universe/force.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <string>
// pro
#include <space/sim/engine/chrono.hpp>
#include <space/sim/engine/entity.hpp>
#include <space/sim/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class Faction : public Entity<Faction, FactionID, invalidFactionID> {
public:
	std::string name;
	libv::vec4f colorPrimary;
	libv::vec4f colorSecondary;
//	Controller* controller = passive, ai, player;

public:
	template <typename Archive> void serialize(Archive& ar);

public:
	Faction() = default; /// For de-serialization only
	Faction(FactionID id, std::string name);
	Faction(FactionID id, std::string name, libv::vec4f colorPrimary, libv::vec4f colorSecondary);

public:
	void kill();
	void update(sim_duration delta_time);

public:
	[[nodiscard]] bool canAttack(const Faction& other) const;
};

//enum class Relation {
//	ally,
//	friendly,
//	neutral,
//	hostile,
//	atWar,
//
//	unknown,
//};
//
//class FactionRelationMatrix {
////	std::map<std::pair<Faction, Faction>, Relation> relations;
////	std::vector<>
//};

// -------------------------------------------------------------------------------------------------

} // namespace space
