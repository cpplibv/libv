// Project: libv, File: app/space/sim/engine/simulation_snapshot_tag.hpp

#pragma once

// fwd
//#include <space/fwd.hpp>
// libv
#include <libv/serial/codec_message_id.hpp>
#include <libv/serial/serial.hpp>
//#include <libv/utility/random/xoroshiro128.hpp>
// std
#include <cstdint>
//#include <memory>
//#include <vector>
// pro
#include <space/sim/engine/snapshot_type.hpp>
//#include <space/sim/engine/chrono.hpp>
//#include <space/sim/engine/memory_store.hpp>
//#include <space/sim/engine/simulation_context.hpp>
//#include <space/sim/fwd.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

//using SnapshotHash = uint64_t;

struct SimulationSnapshotTag {
	static constexpr libv::serial::CodecMessageID id{11};

//	uint64_t version;
//	SnapshotType type;

	template <typename Archive> void serialize(Archive& ar) {
//		ar.nvp("version", version);
//		ar.nvp("type", type);
//		ar(LIBV_NVP(version));
//		ar(LIBV_NVP(type));
		(void) ar;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
