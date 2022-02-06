// Project: libv, File: app/space/sim/planet.cpp

// hpp
#include <space/sim/planet.hpp>
// pro
#include <space/sim/faction.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Planet::Planet() = default; /// For de-serialization only

Planet::~Planet() = default;

Planet::Planet(PlanetID id, libv::vec3f position) :
	Base(id),
	position(position) {}

void Planet::kill() {
	Base::kill();
	faction.reset();
}

void Planet::update(sim_duration dt) {
	(void) dt;
//		if (colony)
//			colony->update(dt);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
