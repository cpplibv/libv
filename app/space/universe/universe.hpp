// Project: libv, File: app/space/universe/universe.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/serial/archive/binary_fwd.hpp>
#include <libv/serial/archive/json_any_fwd.hpp>
#include <libv/serial/codec_message_id.hpp>
#include <libv/serial/serial.hpp>
// pro
#include <space/universe/engine/chrono.hpp>
#include <space/universe/galaxy.hpp>
#include <space/universe/generation/generation_settings.hpp>
//#include <space/universe/engine/tick.hpp>
//#include <space/universe/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct Universe {
	static constexpr libv::serial::CodecMessageID id{11};

public:
	Galaxy galaxy;

//	uint64_t nextPlayerID = 0;
//	plf::colony<Player> players;
//	std::vector<Player> players;

private:
	template <class Archive> void aux_serialize(Archive& ar) {
		ar & LIBV_NVP(galaxy);
	}

public:
	Universe() = default; // For de-serialization only
	explicit Universe(GalaxyGenerationSettings ggs);

public:
	void update(sim_duration delta_time);
	void debug();
	// NOTE: IN SIM THERE IS NO RENDER ((Headless) SERVER WILL RUN THE SIM CODE ONLY)

public:
	void save(libv::archive::Binary::output& ar) const;
	void save(libv::archive::JSONAny::output& ar) const;
	void load(libv::archive::Binary::input& ar);
	void load(libv::archive::JSONAny::input& ar);

public:
	void process(CTO_FleetSpawn&& message);
	void process(CTO_FleetSelect&& message);
	void process(CTO_FleetSelectAdd&& message);
	void process(CTO_FleetClearSelection&& message);
	void process(CTO_FleetBoxSelect&& message);
	void process(CTO_FleetMove&& message);
	void process(CTO_FleetQueueMove&& message);
	void process(CTO_ClearFleets&& message);
	void process(CTO_Shuffle&& message);
	void process(CTO_PlanetSpawn&& message);
	void process(CTO_ClearPlanets&& message);
};

using SnapshotUniverse = Universe;

// -------------------------------------------------------------------------------------------------

} // namespace space
