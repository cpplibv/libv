// Project: libv, File: app/space/sim/ship.hpp

#pragma once

// libv
//#include <libv/meta/reflection_access.hpp>
//#include <libv/serialization/enable.hpp>
// std
//#include <cstdint>
#include <string>
#include <vector>
// pro
//#include <space/sim/planet.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

//enum class ShipClass {
//	class_fighter,
//	class_commando,
//	class_bomber,
//
//	class_corvette,
//	class_frigate,
//	class_destroyer,
//	class_cruiser,
//	class_battleship,
//	class_flagship,
//	class_carrier,
//	class_titan,
//
//	class_support_ship,
//
//	class_colonization,
//	class_terraformer,
//	class_cargo,
////	class_spy,
//};

enum class ShipTag {
	military,
	civilian,
	escort, // small_ship
	support,

	// Small military class
	class_fighter,
//	class_commando,
	class_bomber,

	// Large military class
	class_corvette,
	class_frigate,
	class_destroyer,
	class_cruiser,
	class_battleship,
	class_flagship,
	class_carrier,
	class_titan,

	// Support class
	class_support_ship,

	// Civil class
	class_colonization,
	class_terraformer,
	class_cargo,
//	class_spy,
};

struct ShipTypeDefinition {
//	std::string something;
};

struct Ship {
	std::vector<ShipTag> tags;
//	std::vector<Tank> tanks;
};

// -------------------------------------------------------------------------------------------------

} // namespace space
