// Project: libv, File: src/space/universe/galaxy.hpp

#pragma once

// ext
#include <boost/container/flat_set.hpp>
//#include <colony/plf_colony.h>
// libv
#include <libv/algo/linear_find.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/boost_flat_set.hpp>
#include <libv/serial/types/std_memory.hpp>
#include <libv/serial/types/std_vector.hpp>
#include <libv/utility/projection.hpp>
// std
#include <vector>
#include <string>
// pro
#include <space/universe/engine/chrono.hpp>
#include <space/universe/faction.hpp>
#include <space/universe/fleet.hpp>
#include <space/universe/ids.hpp>
#include <space/universe/planet.hpp>
//#include <space/universe/colony.hpp>
//#include <space/universe/empire.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct Galaxy {
//	static constexpr libv::serial::CodecMessageID id{C0};

public:
//	float time = 0.0f;
//	float test_sin_time = 0.0f;

	FactionID nextFactionID{0};
	PlanetID nextPlanetID{0};
	FleetID nextFleetID{0};
//	uint64_t nextColonyID = 0;
//	uint64_t nextEmpireID = 0;
//	uint64_t nextTradeID = 0;

//	plf::colony<Colony> colonies;
//	plf::colony<Faction> factions;
//	plf::colony<Empire> empires;
//	plf::colony<Fleet> fleets;
//	plf::colony<Planet> planets;
//	plf::colony<Trade> trades;

	std::vector<std::shared_ptr<Faction>> factions;
//	std::vector<Faction> factions;
	std::vector<Planet> planets;
	std::vector<Fleet> fleets;
//	std::vector<Colony> colonies;
//	std::vector<Empire> empires;
//	std::vector<Trade> trades;

	boost::container::flat_set<FleetID> selectedFleetIDList;

public:
	template <class Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(nextFactionID);
		ar & LIBV_NVP(nextPlanetID);
		ar & LIBV_NVP(nextFleetID);
		ar & LIBV_NVP(factions);
		ar & LIBV_NVP(planets);
		ar & LIBV_NVP(fleets);
		if (ar.isLocal())
			ar & LIBV_NVP(selectedFleetIDList);
//		else
//			clear_and_or_maintain_the_selected_fleets_list();
	}

	Galaxy() {
		// <<< app.space: Faction Maybe a lookup map? (Linear search most likely the fastest)
		//			Maybe not shared_ptr? I dont think factions would be ever removed, at most they are marked as extinct
		// <<< app.space: "Neutral" faction might be references a lot, make a hard coded getter (member) for it
		factions.push_back(std::make_shared<Faction>(nextFactionID++, "Neutral", libv::vec4f{0.65f, 0.70f, 0.70f, 0.8f}, libv::vec4f{0.4f, 0.4f, 0.4f, 1.f}));
//		factions.push_back(std::make_shared<Faction>(nextFactionID++, "StoryTeller", aiController));
//		factions.push_back(std::make_shared<Faction>(nextFactionID++, "AI", aiController));
	}

	std::shared_ptr<Faction> faction(std::string_view name) {
		auto cacheHit = libv::linear_find_optional(factions, name, libv::proj_indirect(&Faction::name));
		if (cacheHit)
			return *cacheHit;

		auto result = std::make_shared<Faction>(nextFactionID++, std::string(name));
		factions.push_back(result);
		return result;
	}

	void update(sim_duration delta_time) {
		for (auto& faction : factions)
			faction->update(delta_time);
//			faction.update(delta_time);

		for (auto& planet : planets)
			planet.update(delta_time);

		for (auto& fleet : fleets)
			fleet.update(delta_time);

//		for (auto& i : colonies)
//			i.update(dt);
//
//		for (auto& i : empires)
//			i.update(dt);
//
//		for (auto& i : fleets)
//			i.update(dt);
//
//		for (auto& i : trades)
//			i.update(dt);
	}

	void debug() {

	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
