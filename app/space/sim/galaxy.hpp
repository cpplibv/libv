// Project: libv, File: src/space/universe/galaxy.hpp

#pragma once

// ext
#include <boost/container/flat_set.hpp>
// libv
#include <libv/utility/entity/entity_ptr.hpp>
#include <libv/utility/observer_ptr.hpp>
// std
#include <deque>
#include <memory>
#include <string_view>
#include <vector>
// pro
#include <space/fwd.hpp>
#include <space/sim/engine/chrono.hpp>
#include <space/sim/fwd.hpp>
#include <space/sim/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class Galaxy {
public:
	FactionID nextFactionID = firstFactionID;
	PlanetID nextPlanetID = firstPlanetID;
	FleetID nextFleetID = firstFleetID;
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
	template <typename Archive> void serialize(Archive& ar);

public:
	explicit Galaxy(MemoryStore& memory);
	Galaxy(const Galaxy&) = delete;
	Galaxy& operator=(const Galaxy&) & = delete;
	Galaxy(Galaxy&&) noexcept;
	Galaxy& operator=(Galaxy&&) & noexcept;
	~Galaxy();

public:
	[[nodiscard]] libv::entity_ptr<Faction> faction(std::string_view name);
	[[nodiscard]] libv::entity_ptr<Faction> factionNeutral() const;

public:
	void kill();
	void update(sim_duration delta_time);

	void debug();
};

// -------------------------------------------------------------------------------------------------

} // namespace space
