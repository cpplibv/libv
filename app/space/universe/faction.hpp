// Project: libv, File: src/space/universe/force.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_string.hpp>
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// std
#include <string>
// pro
#include <space/universe/ids.hpp>
#include <space/universe/engine/chrono.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class Faction {
private:
public: // <<<
	FactionID id = invalidFactionID;
	std::string name;
	libv::vec4f colorPrimary;
	libv::vec4f colorSecondary;
//	Controller* controller = passive, ai, player;

public:
	template <class Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(id);
		ar & LIBV_NVP(name);
		ar & LIBV_NVP(colorPrimary);
		ar & LIBV_NVP(colorSecondary);
	}

public:
	Faction() = default; /// For de-serialization only
	Faction(FactionID id, std::string name) :
			id(id),
			name(std::move(name)) {

		auto rng = libv::xoroshiro128(+id + 500000);
		auto dist = libv::make_uniform_distribution_inclusive(0.f, 1.f);
		colorPrimary.x = dist(rng);
		colorPrimary.y = dist(rng);
		colorPrimary.z = dist(rng);
		colorSecondary.x = dist(rng);
		colorSecondary.y = dist(rng);
		colorSecondary.z = dist(rng);
	}

public:
	void update(sim_duration delta_time) {
		(void) delta_time;
	}
};

enum class Relation {
	ally,
	friendly,
	neutral,
	hostile,
	atWar,

	unknown,
};

class FactionRelationMatrix {
//	std::map<std::pair<Faction, Faction>, Relation> relations;
//	std::vector<>
};

// -------------------------------------------------------------------------------------------------

} // namespace space
