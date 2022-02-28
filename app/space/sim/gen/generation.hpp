// Project: libv, File: app/space/sim/gen/generation.hpp

#pragma once

// libv
#include <libv/math/vec_fwd.hpp>
#include <libv/utility/entity/entity_ptr_fwd.hpp>
#include <libv/utility/random/xoroshiro128_fwd.hpp>
// std
#include <memory>
// pro
#include <space/fwd.hpp>
#include <space/sim/gen/generation_settings.hpp> // fwdable
#include <space/sim/ids.hpp> // fwdable


namespace space {

// -------------------------------------------------------------------------------------------------

libv::primary_entity_ptr<Planet> generatePlanet(const std::shared_ptr<SimulationContext>& ctx, PlanetID id, libv::vec3f position, libv::xoroshiro128& rng_planet);
Universe generateGalaxy(const std::shared_ptr<SimulationContext>& ctx, UniverseGenerationSettings settings);

//libv::entity_ptr<Planet> generatePlanet(MemoryStore& memory, PlanetID id, libv::vec3f position, libv::xoroshiro128& rng_planet);
//Universe generateUniverse(MemoryStore& memory, UniverseGenerationSettings settings);

// -------------------------------------------------------------------------------------------------

} // namespace space
