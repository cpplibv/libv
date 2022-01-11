// Project: libv, File: app/space/universe/universe.hpp

// hpp
#include <space/universe/engine/memory_store.hpp>
// libv
#include <space/log.hpp>
#include <space/universe/faction.hpp>
#include <space/universe/fleet.hpp>
#include <space/universe/planet.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

MemoryStore::MemoryStore() :
		faction(100),
		planet(1000),
		fleet(2000) {
	log_space.info("faction initial capacity: {}", faction.capacity());
	log_space.info("planet initial capacity: {}", planet.capacity());
	log_space.info("fleet initial capacity: {}", fleet.capacity());
}

// -------------------------------------------------------------------------------------------------

} // namespace space
