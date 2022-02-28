// Project: libv, File: app/space/sim/engine/simulation_context.hpp

#pragma once

// fwd
//#include <space/fwd.hpp>
// libv
//#include <libv/serial/codec_message_id.hpp>
//#include <libv/utility/random/xoroshiro128.hpp>
// std
//#include <memory>
//#include <vector>
// pro
//#include <space/sim/engine/chrono.hpp>
#include <space/sim/engine/memory_store.hpp>
//#include <space/sim/engine/snapshot_type.hpp>
//#include <space/sim/fwd.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct SimulationContext {
	MemoryStore memory;
};

// -------------------------------------------------------------------------------------------------

} // namespace space
