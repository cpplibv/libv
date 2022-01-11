// Project: libv, File: src/space/universe/galaxy.hpp

#pragma once

// ext
#include <boost/container/flat_set.hpp>
// libv
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_vector.hpp>
#include <libv/utility/entity/entity_ptr.hpp>
#include <libv/utility/entity/entity_ptr_serial.hpp>
#include <libv/utility/observer_ptr.hpp>
// std
#include <deque>
#include <memory>
#include <string_view>
#include <vector>
// pro
#include <space/fwd.hpp>
#include <space/universe/engine/chrono.hpp>
#include <space/universe/engine/memory_store.hpp>
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

	libv::observer_ptr<MemoryStore> memory; // Would be nice to not store a pointer to memory in here
	std::vector<libv::entity_ptr<Faction>> factions;
	std::vector<libv::entity_ptr<Planet>> planets;
	std::vector<libv::entity_ptr<Fleet>> fleets;
//	std::vector<libv::entity_ptr<Colony>> colonies;
//	std::vector<libv::entity_ptr<Empire>> empires;
//	std::vector<libv::entity_ptr<Trade>> trades;

	libv::entity_ptr<Faction> factionNeutral_; // "Neutral" faction is references a lot, store a direct lookup for it

	boost::container::flat_set<FleetID> selectedFleetIDList;

public:
	template <typename Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(nextFactionID);
		ar & LIBV_NVP(nextPlanetID);
		ar & LIBV_NVP(nextFleetID);

		ar & LIBV_NVP_NAMED("factions", libv::entity_primary_owner(memory->faction, factions));
		ar & LIBV_NVP_NAMED("planets", libv::entity_primary_owner(memory->planet, planets));
		ar & LIBV_NVP_NAMED("fleets", libv::entity_primary_owner(memory->fleet, fleets));

		if (ar.isLocal())
			ar & LIBV_NVP(selectedFleetIDList);
		else
			if constexpr (Archive::is_loading::value)
				// TODO P5: Instead of full clear, remove/update only the elements that are no longer available (for seamless snapshot update)
				selectedFleetIDList.clear();
	}

public:
	explicit Galaxy(MemoryStore& memory);
//	// TODO P1: Remove copy ctor and copy assignment (they are only required by an captured lambda in a std::function
//	Galaxy(const Galaxy&);
//	Galaxy& operator=(const Galaxy&) &;
	Galaxy(Galaxy&&) noexcept;
	Galaxy& operator=(Galaxy&&) & noexcept;
	~Galaxy();

public:
	[[nodiscard]] libv::entity_ptr<Faction> faction(std::string_view name);
	[[nodiscard]] libv::entity_ptr<Faction> factionNeutral() const;

public:
	void update(sim_duration delta_time);

	void debug() {

	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
