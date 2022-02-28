// Project: libv, File: app/space/sim/universe.hpp

#pragma once

// fwd
//#include <space/fwd.hpp>
// ext
#include <boost/container/flat_set.hpp>
// libv
#include <libv/serial/codec_message_id.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// std
#include <memory>
// pro
//#include <space/sim/engine/chrono.hpp>
//#include <space/sim/engine/memory_store.hpp>
#include <space/sim/fwd.hpp>
#include <space/sim/galaxy.hpp>
#include <space/sim/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct Universe {
//	static constexpr libv::serial::CodecMessageID id{11};
//
public:
	std::shared_ptr<SimulationContext> ctx;
//	std::shared_ptr<MemoryStore> memory;

public:
	FactionID nextFactionID = firstFactionID;
	PlanetID nextPlanetID = firstPlanetID;
	FleetID nextFleetID = firstFleetID;
//	uint64_t nextColonyID = 0;
//	uint64_t nextEmpireID = 0;
//	uint64_t nextTradeID = 0;

public:
	std::vector<libv::primary_entity_ptr<Faction>> factions;
//	std::vector<libv::entity_ptr<Colony>> colonies;
//	std::vector<libv::entity_ptr<Empire>> empires;
//	std::vector<libv::entity_ptr<Trade>> trades;

//	libv::xoroshiro128 universe_rng;

	Galaxy galaxy;

	libv::entity_ptr<Faction> factionNeutral_; // "Neutral" faction is references a lot, store a direct lookup for it
	libv::entity_ptr<Faction> factionControlled_; // TODO P4: Move to simulation or simulation control

public:
	boost::container::flat_set<FleetID> selectedFleetIDList;
//	boost::container::flat_set<libv::entity_ptr<Fleet>> selectedFleetIDList;

public:
//	template <typename Archive> void save(Archive& ar) const;
//	template <typename Archive> void load(Archive& ar);
	template <typename Archive> void serialize(Archive& ar);

public:
	explicit Universe(const std::shared_ptr<SimulationContext>& ctx);
	//explicit Universe(UniverseGenerationSettings ggs);

	Universe(const Universe&) noexcept = delete;
	Universe& operator=(const Universe&) & noexcept = delete;
	Universe(Universe&&) noexcept;
	Universe& operator=(Universe&&) & noexcept;

	~Universe();

public:
	void kill();
	void update(sim_duration delta_time);

public:
	[[nodiscard]] const libv::entity_ptr<Faction>& faction(std::string_view name);
	[[nodiscard]] const libv::entity_ptr<Faction>& factionNeutral() const;
	[[nodiscard]] const libv::entity_ptr<Faction>& factionControlled() const;

//public:
//	void process(CTO_FleetSpawn&& message);
//	void process(CTO_FleetSelect&& message);
//	void process(CTO_FleetSelectAdd&& message);
//	void process(CTO_FleetSelectBox&& message);
//	void process(CTO_FleetClearSelection&& message);
//	void process(CTO_FleetMove&& message);
//	void process(CTO_FleetMoveQueue&& message);
//	void process(CTO_FleetAttackFleet&& message);
//	void process(CTO_FleetAttackFleetQueue&& message);
//	void process(CTO_FleetAttackPlanet&& message);
//	void process(CTO_FleetAttackPlanetQueue&& message);
//	void process(CTO_ClearFleets&& message);
//	void process(CTO_Shuffle&& message);
//	void process(CTO_PlanetSpawn&& message);
//	void process(CTO_ClearPlanets&& message);
};

using SnapshotUniverse = Universe;

// -------------------------------------------------------------------------------------------------

} // namespace space
