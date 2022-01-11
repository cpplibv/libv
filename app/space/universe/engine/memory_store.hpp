// Project: libv, File: app/space/universe/universe.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/utility/entity/entity_ptr.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct MemoryStore {
	libv::entity_store<Faction> faction;
	libv::entity_store<Planet> planet;
	libv::entity_store<Fleet> fleet;

	MemoryStore();
};

// -------------------------------------------------------------------------------------------------

} // namespace space
