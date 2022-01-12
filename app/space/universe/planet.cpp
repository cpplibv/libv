// Project: libv, File: app/space/universe/universe.hpp

// hpp
#include <space/universe/planet.hpp>
// pro
#include <space/universe/faction.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Planet::Planet() = default; /// For de-serialization only

Planet::~Planet() = default;

Planet::Planet(PlanetID id, libv::vec3f position) :
	Base(id),
	position(position) {}

void Planet::kill() {
	faction.reset();
}

void Planet::update(sim_duration dt) {
	(void) dt;
//		if (colony)
//			colony->update(dt);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
