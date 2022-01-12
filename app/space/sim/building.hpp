// Project: libv, File: src/space/universe/building.hpp

#pragma once

//#include <boost/serialization/shared_ptr.hpp>

//#include <libv/memory.hpp>
#include <libv/math/vec.hpp>
#include <libv/math/vec_serial.hpp>
#include <libv/serial/serial.hpp>
// std
#include <string>
// pro
#include <space/sim/building_resources.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct TypeDefinition {
	std::string string_id;
	std::string mod_id;


};

// -------------------------------------------------------------------------------------------------

enum class BuildingTag {
//	military,
//	civilian,
};

// -------------------------------------------------------------------------------------------------

struct BuildingTypeDefinition {
	uint32_t id;
//	std::string stringID;
	std::string localeName;
	std::string localeDescription;
//	loc_string localeName;
//	loc_string localeDescription;
	//	Race race;
	std::string renderAsset;

	libv::vec2i size;
	uint32_t maxHealth;

	int32_t colonyLimit;
	int32_t buildProduction;
	int32_t buildMoneyCost;

	std::vector<BuildingTag> tags;
	BuildingResources resources;

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int) {
		ar & LIBV_NVP(id);
		ar & LIBV_NVP(localeName);
		ar & LIBV_NVP(localeDescription);
		ar & LIBV_NVP(renderAsset);

		ar & LIBV_NVP(size);
		ar & LIBV_NVP(maxHealth);

		ar & LIBV_NVP(colonyLimit);
		ar & LIBV_NVP(buildProduction);
		ar & LIBV_NVP(buildMoneyCost);

		ar & LIBV_NVP(resources);
	}
};

// -------------------------------------------------------------------------------------------------

using BuildingType = BuildingTypeDefinition*;

// -------------------------------------------------------------------------------------------------

struct Building {
	uint32_t id;
	const BuildingType type;
	libv::vec2i position;
	int32_t progress;
	int32_t health;
//	bool disabled;
//	bool operational;

//	BOOST_SERIALIZATION_SPLIT_MEMBER()
//	template <typename Archive> void save(Archive& ar, const unsigned int) const {
//		ar & LIBV_NVP(id);
//		ar & LIBV_NVP_NAMED("type", type->id);
//		ar & LIBV_NVP(buildProgress);
//		ar & LIBV_NVP(health);
//		ar & LIBV_NVP(operational);
//	}
//	template <typename Archive> void load(Archive& ar, const unsigned int) {
//		ar & LIBV_NVP(id);
//		uint32_t typeID;
//		ar & LIBV_NVP_NAMED("type", typeID);
//		type = std::make_shared<BuildingType>(); // TODO P2: Cache lookup typeID -> type
//		ar & LIBV_NVP(buildProgress);
//		ar & LIBV_NVP(health);
//		ar & LIBV_NVP(operational);
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
