// Created by Vader on 2021.12.13..

#pragma once

// libv
#include <libv/math/vec_fwd.hpp>
#include <libv/utility/random/xoroshiro128.hpp> // fwdable
// pro
#include <space/fwd.hpp>
#include <space/universe/generation/generation_settings.hpp> // fwdable
#include <space/universe/ids.hpp> // fwdable


namespace space {

// -------------------------------------------------------------------------------------------------

Planet generatePlanet(PlanetID id, libv::vec3f position, libv::xoroshiro128& rng_planet);
Galaxy generateGalaxy(GalaxyGenerationSettings settings);

// -------------------------------------------------------------------------------------------------

} // namespace space
