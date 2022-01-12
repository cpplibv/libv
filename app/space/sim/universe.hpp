// Project: libv, File: app/space/sim/universe.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/serial/codec_message_id.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// pro
#include <space/sim/engine/chrono.hpp>
#include <space/sim/engine/memory_store.hpp>
#include <space/sim/galaxy.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct Universe {
	static constexpr libv::serial::CodecMessageID id{11};

public:
	MemoryStore memory;

public:
	libv::xoroshiro128 universe_rng;
	Galaxy galaxy;

public:
	template <typename Archive> void save(Archive& ar) const;
	template <typename Archive> void load(Archive& ar);

public:
	Universe(); // For de-serialization only
	explicit Universe(GalaxyGenerationSettings ggs);

//	// TODO P1: Remove copy ctor and copy assignment (they are only required by an captured lambda in a std::function
//	Universe(const Universe&) noexcept = default;
//	Universe& operator=(const Universe&) & noexcept = default;
	Universe(Universe&&) noexcept = default;
	Universe& operator=(Universe&&) & noexcept = default;

	~Universe();

public:
	void update(sim_duration delta_time);
	void debug();
	// NOTE: IN SIM THERE IS NO RENDER ((Headless) SERVER WILL RUN THE SIM CODE ONLY)

public:
	void process(CTO_FleetSpawn&& message);
	void process(CTO_FleetSelect&& message);
	void process(CTO_FleetSelectAdd&& message);
	void process(CTO_FleetSelectBox&& message);
	void process(CTO_FleetClearSelection&& message);
	void process(CTO_FleetMove&& message);
	void process(CTO_FleetMoveQueue&& message);
	void process(CTO_FleetAttackFleet&& message);
	void process(CTO_FleetAttackFleetQueue&& message);
	void process(CTO_FleetAttackPlanet&& message);
	void process(CTO_FleetAttackPlanetQueue&& message);
	void process(CTO_ClearFleets&& message);
	void process(CTO_Shuffle&& message);
	void process(CTO_PlanetSpawn&& message);
	void process(CTO_ClearPlanets&& message);
};

using SnapshotUniverse = Universe;

// -------------------------------------------------------------------------------------------------

} // namespace space
