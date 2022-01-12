// Project: libv, File: app/space/universe/universe.hpp

// hpp
#include <space/sim/engine/memory_store.hpp>
// libv
#include <space/log.hpp>
#include <space/sim/faction.hpp>
#include <space/sim/fleet.hpp>
#include <space/sim/planet.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

MemoryStore::MemoryStore() :
		faction(100),
		planet(1000),
		fleet(2000) {
	log_space.trace("Faction initial capacity: {}", faction.capacity());
	log_space.trace("Planet initial capacity: {}", planet.capacity());
	log_space.trace("Fleet initial capacity: {}", fleet.capacity());
}

MemoryStore::MemoryStore(MemoryStore&&) noexcept = default;

MemoryStore& MemoryStore::operator=(MemoryStore&&) & noexcept = default;

MemoryStore::~MemoryStore() {
	log_space.trace("Faction terminating capacity: {}", faction.capacity());
	log_space.error_if(faction.size() != 0, "Object leak detected in faction entity_store. Leaked {} entity", faction.size());

	log_space.trace("Planet terminating capacity: {}", planet.capacity());
	log_space.error_if(planet.size() != 0, "Object leak detected in planet entity_store. Leaked {} entity", planet.size());

	log_space.trace("Fleet terminating capacity: {}", fleet.capacity());
	log_space.error_if(fleet.size() != 0, "Object leak detected in fleet entity_store. Leaked {} entity", fleet.size());
}

// -------------------------------------------------------------------------------------------------

} // namespace space
