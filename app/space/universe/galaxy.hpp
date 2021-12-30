// Project: libv, File: src/space/universe/galaxy.hpp

#pragma once

// ext
#include <boost/container/flat_set.hpp>
// libv
#include <libv/serial/serial.hpp>
// std
#include <deque>
#include <memory>
#include <string_view>
#include <vector>
// pro
#include <space/fwd.hpp>
#include <space/universe/engine/chrono.hpp>
#include <space/universe/ids.hpp>
//#include <space/universe/galaxy_serial.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct Galaxy {
public:
//	float time = 0.0f;
//	float test_sin_time = 0.0f;

	FactionID nextFactionID{0};
	PlanetID nextPlanetID{0};
	FleetID nextFleetID{0};
//	uint64_t nextColonyID = 0;
//	uint64_t nextEmpireID = 0;
//	uint64_t nextTradeID = 0;

	// TODO P1: Simulation entity_ptr and entity_store
	std::vector<std::shared_ptr<Faction>> factions;
//	std::vector<Faction> factions;
	std::deque<Planet> planets; // Memory address stability is utilized
	std::deque<Fleet> fleets; // Memory address stability is utilized
//	std::vector<Colony> colonies;
//	std::vector<Empire> empires;
//	std::vector<Trade> trades;

	std::shared_ptr<Faction> factionNeutral_; // "Neutral" faction is references a lot, store a direct lookup for it

	boost::container::flat_set<FleetID> selectedFleetIDList;

public:
	template <typename Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(nextFactionID);
		ar & LIBV_NVP(nextPlanetID);
		ar & LIBV_NVP(nextFleetID);
		ar & LIBV_NVP(factions);
		ar & LIBV_NVP(planets);
		ar & LIBV_NVP(fleets);
		if (ar.isLocal())
			ar & LIBV_NVP(selectedFleetIDList);
		else
			if constexpr (Archive::is_loading::value)
				// TODO P5: Instead of full clear, remove/update only the elements that are no longer available (for seamless snapshot update)
				selectedFleetIDList.clear();
	}

public:
	Galaxy();
//	// TODO P1: Remove copy ctor and copy assignment (they are only required by an captured lambda in a std::function
//	Galaxy(const Galaxy&);
//	Galaxy& operator=(const Galaxy&) &;
	Galaxy(Galaxy&&) noexcept;
	Galaxy& operator=(Galaxy&&) & noexcept;
	~Galaxy();

public:
	[[nodiscard]] std::shared_ptr<Faction> faction(std::string_view name);
	[[nodiscard]] std::shared_ptr<Faction> factionNeutral() const;

public:
	void update(sim_duration delta_time);

	void debug() {

	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
