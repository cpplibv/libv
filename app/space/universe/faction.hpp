// Project: libv, File: src/space/universe/force.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_string.hpp>
// std
#include <string>
// pro
#include <space/universe/engine/chrono.hpp>
#include <space/universe/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class Faction {
private:
public: // TODO P5: Make the interface more closed
	FactionID id = invalidFactionID;
	std::string name;
	libv::vec4f colorPrimary;
	libv::vec4f colorSecondary;
//	Controller* controller = passive, ai, player;

public:
	template <typename Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(id);
		ar & LIBV_NVP(name);
		ar & LIBV_NVP(colorPrimary);
		ar & LIBV_NVP(colorSecondary);
	}

public:
	Faction() = default; /// For de-serialization only
	Faction(FactionID id, std::string name);
	Faction(FactionID id, std::string name, libv::vec4f colorPrimary, libv::vec4f colorSecondary);

public:
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
