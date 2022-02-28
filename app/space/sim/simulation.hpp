// Project: libv, File: app/space/sim/simulation.hpp

#pragma once

// fwd
//#include <space/fwd.hpp>
// libv
#include <libv/serial/archive/binary_fwd.hpp>
//#include <libv/serial/codec_message_id.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// std
#include <memory>
//#include <vector>
// pro
#include <space/sim/engine/chrono.hpp>
#include <space/sim/engine/simulation_context.hpp>
#include <space/sim/engine/simulation_snapshot_tag.hpp>
#include <space/sim/engine/snapshot_type.hpp>
#include <space/sim/fwd.hpp>
//#include <space/sim/engine/memory_store.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

using SnapshotHash = uint64_t;

//struct SimulationSnapshot {
//	SnapshotHash hash;
////	std::vector<std::byte> data;
//
////	template <typename Archive> void save(Archive& ar) const;
////	template <typename Archive> void load(Archive& ar);
//};

struct SimulationSnapshotHeader {
	uint64_t version;
	SnapshotType type;

	template <typename Archive> void serialize(Archive& ar) {
//		ar.nvp("version", version);
//		ar.nvp("type", type);
		ar(LIBV_NVP(version));
		ar(LIBV_NVP(type));
	}
};

// -------------------------------------------------------------------------------------------------

class Simulation {
private:
	std::shared_ptr<SimulationContext> ctx = std::make_shared<SimulationContext>();

private:
	libv::xoroshiro128 simulationRNG;

	int32_t tickRate_ = 60;

	int32_t tickCurrent_ = 0;
	int32_t simulationSpeed_ = 1;

public:
	std::unique_ptr<Universe> universe;

public:
	Simulation();
//	Universe(); // For de-serialization only
//	explicit Universe(UniverseGenerationSettings ggs);
//
//	Universe(const Universe&) noexcept = delete;
//	Universe& operator=(const Universe&) & noexcept = delete;
//	Universe(Universe&&) noexcept;
//	Universe& operator=(Universe&&) & noexcept;

	~Simulation();

public:
//	void saveSnapshot(SnapshotType type) const;
//	void loadSnapshot(SimulationSnapshot&&);
//	void saveSnapshot(libv::archive::BinaryOutput& ar, SnapshotType type) const;
//	void loadSnapshot(libv::archive::BinaryInput& ar, SimulationSnapshot&&);
	void saveSnapshot(libv::archive::BinaryOutput& ar, SnapshotType type) const;
	void loadSnapshot(libv::archive::BinaryInput& ar, SnapshotType type);
	[[nodiscard]] std::string debugAsJson() const;
	[[nodiscard]] SnapshotHash hashState() const;

public:
	[[nodiscard]] constexpr inline int32_t tickRate() const noexcept {
		return tickRate_;
	}
	constexpr inline void tickRate(int32_t value) noexcept {
		tickRate_ = value;
	}
	[[nodiscard]] constexpr inline int32_t tickCurrent() const noexcept {
		return tickCurrent_;
	}
	[[nodiscard]] constexpr inline int32_t simulationSpeed() const noexcept {
		return simulationSpeed_;
	}
	constexpr inline void simulationSpeed(int32_t value) noexcept {
		simulationSpeed_ = value;
	}

public:
	void update(sim_duration delta_time);

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

// -------------------------------------------------------------------------------------------------

} // namespace space
