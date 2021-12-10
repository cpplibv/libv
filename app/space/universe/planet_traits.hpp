// Project: libv, File: src/space/universe/planet.hpp

#pragma once

// std
//#include <vector>
//#include <string>
// pro
//#include <space/universe/engine/chrono.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

enum class ePlanetTraits {
	Low_atmosphere_pressure,

	Close_to_sun,
	Multiple_sun,

	Geoactive,
	Geodormant,
	Frozen_Core,

	Metal_Poor,
	Metal_Rich,
	Mineral_Rich,
//	Buried_Oil,
//	Irregular_Oil,

//	Large_Glaciers,
	Subsurface_Ocean,
	Volcanoes,



};

struct PlanetTraits {

};

// -------------------------------------------------------------------------------------------------

} // namespace space
