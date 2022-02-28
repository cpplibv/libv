// Project: libv, File: app/space/sim/gen/generation_settings.hpp

#pragma once

// std
#include <cstdint>


namespace space {

// -------------------------------------------------------------------------------------------------

struct UniverseGenerationSettings {
	uint64_t seed = 0x5EED;
//	int numPlanet = 1000;
	int numPlanet = 250;
	float galaxyRadius = 100.f;
	float galaxyHeight = 2.f;
};

// -------------------------------------------------------------------------------------------------

} // namespace space
