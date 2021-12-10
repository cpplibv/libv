// Project: libv, File: src/space/universe/galaxy.hpp

#pragma once

// ext
#include <boost/container/flat_set.hpp>
//#include <colony/plf_colony.h>
// libv
#include <libv/serial/types/boost_flat_set.hpp>
// std
#include <vector>
#include <string>
// pro
#include <space/universe/engine/chrono.hpp>
#include <space/universe/planet.hpp>
//#include <space/universe/colony.hpp>
//#include <space/universe/faction.hpp>
//#include <space/universe/empire.hpp>
#include <space/universe/fleet.hpp>
#include <space/universe/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct Galaxy {
//	static constexpr libv::serial::CodecMessageID id{C0};

public:
//	float time = 0.0f;
//	float test_sin_time = 0.0f;

//	uint64_t nextColonyID = 0;
//	uint64_t nextEmpireID = 0;
	FleetID nextFleetID{0};
//	uint64_t nextForceID = 0;
	PlanetID nextPlanetID{0};
//	uint64_t nextTradeID = 0;

//	plf::colony<Colony> colonies;
//	plf::colony<Faction> factions;
//	plf::colony<Empire> empires;
//	plf::colony<Fleet> fleets;
//	plf::colony<Planet> planets;
//	plf::colony<Trade> trades;

//	std::vector<Colony> colonies;
//	std::vector<Empire> empires;
	std::vector<Fleet> fleets;
//	std::vector<Force> forces;
	std::vector<Planet> planets;
//	std::vector<Trade> trades;

	boost::container::flat_set<FleetID> selectedFleetIDList;

public:
	template <class Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(nextFleetID);
		ar & LIBV_NVP(nextPlanetID);
		ar & LIBV_NVP(fleets);
		ar & LIBV_NVP(planets);
		ar & LIBV_NVP(selectedFleetIDList);
	}
//};
//
//struct Galaxy : SnapshotGalaxy {
//public:
	void update(sim_duration delta_time) {
//		for (auto& i : colonies)
//			i.update(dt);
//
//		for (auto& i : factions)
//			i.update(dt);
//
//		for (auto& i : empires)
//			i.update(dt);

		for (auto& fleet : fleets)
			fleet.update(delta_time);

//		for (auto& i : fleets)
//			i.update(dt);

		for (auto& planet : planets)
			planet.update(delta_time);

//		for (auto& i : trades)
//			i.update(dt);
	}

	void debug() {

	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
