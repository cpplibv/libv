// Project: libv, File: app/space/sim/galaxy.cpp

// hpp
#include <space/sim/galaxy.hpp>
// libv
#include <libv/algo/erase_if_unstable.hpp>
// pro
#include <space/sim/fleet.hpp>
#include <space/sim/planet.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Galaxy::Galaxy(const std::shared_ptr<SimulationContext>& ctx) :
	ctx(ctx) {
}

//Galaxy::Galaxy(const Galaxy&) = default;
//Galaxy& Galaxy::operator=(const Galaxy&) & = default;
Galaxy::Galaxy(Galaxy&&) noexcept = default;
Galaxy& Galaxy::operator=(Galaxy&& other) & noexcept = default;
Galaxy::~Galaxy() = default;

void Galaxy::kill() {
	for (const auto& planet : planets)
		planet->kill();
	planets.clear();

	for (const auto& fleet : fleets)
		fleet->kill();
	fleets.clear();
}

void Galaxy::update(sim_duration delta_time) {
	libv::erase_if_unstable(planets, [&](libv::entity_ptr<Planet>& planet) {
		planet->update(delta_time);
		return planet->dead();
	});

	libv::erase_if_unstable(fleets, [&](libv::entity_ptr<Fleet>& fleet) {
		fleet->update(delta_time);
		return fleet->dead();
	});
}

void Galaxy::debug() {

}

// -------------------------------------------------------------------------------------------------

} // namespace space
