// Project: libv, File: app/space/sim/engine/memory_store.cpp

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
		faction(50),
		planet(200),
		fleet(100) {
	log_space.trace("Faction initial capacity: {}", faction.capacity());
	log_space.trace("Planet initial capacity: {}", planet.capacity());
	log_space.trace("Fleet initial capacity: {}", fleet.capacity());
}

MemoryStore::MemoryStore(MemoryStore&& other) noexcept = default;

MemoryStore& MemoryStore::operator=(MemoryStore&& other) & noexcept {
	checkLeak();

	faction = std::move(other.faction);
	planet = std::move(other.planet);
	fleet = std::move(other.fleet);

	return *this;
}

MemoryStore::~MemoryStore() {
	log_space.trace("Faction terminating capacity: {}", faction.capacity());
	log_space.trace("Planet terminating capacity: {}", planet.capacity());
	log_space.trace("Fleet terminating capacity: {}", fleet.capacity());

	checkLeak();
}

void MemoryStore::checkLeak() const {
	log_space.error_if(faction.size() != 0, "Object leak detected in faction entity_store. Leaked {} entity", faction.size());
	log_space.error_if(planet.size() != 0, "Object leak detected in planet entity_store. Leaked {} entity", planet.size());
	log_space.error_if(fleet.size() != 0, "Object leak detected in fleet entity_store. Leaked {} entity", fleet.size());
}

// -------------------------------------------------------------------------------------------------

} // namespace space
