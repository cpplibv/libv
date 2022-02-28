// Project: libv, File: app/space/sim/engine/snapshot_type.hpp

#pragma once

// std
#include <cstdint>


namespace space {

// -------------------------------------------------------------------------------------------------

enum class SnapshotType : uint8_t {
	///	Local Snapshot: Includes everything from the simulation. Meant for quick save / load
	local = 0,
	///	Shared Snapshot: Excludes some local perspective from the simulation (Like selection).
	/// Meant to be shared on the network or in a save file
	shared = 1,
};

// -------------------------------------------------------------------------------------------------

} // namespace space
