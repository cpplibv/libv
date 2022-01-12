// Created by Vader on 2021.12.13..

#pragma once

// libv
#include <libv/math/vec_fwd.hpp>
#include <libv/utility/entity/entity_ptr_fwd.hpp>
#include <libv/utility/random/xoroshiro128.hpp> // fwdable
// pro
#include <space/fwd.hpp>
#include <space/sim/gen/generation_settings.hpp> // fwdable
#include <space/sim/ids.hpp> // fwdable


namespace space {

// -------------------------------------------------------------------------------------------------

libv::entity_ptr<Planet> generatePlanet(MemoryStore& memory, PlanetID id, libv::vec3f position, libv::xoroshiro128& rng_planet);
Galaxy generateGalaxy(MemoryStore& memory, GalaxyGenerationSettings settings);

// -------------------------------------------------------------------------------------------------

} // namespace space
