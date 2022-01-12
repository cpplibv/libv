// Project: libv, File: src/space/universe/colony.hpp

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/math/vec.hpp>
//#include <libv/math/vec_serial.hpp>
//#include <libv/serial/serial.hpp>
// std
#include <string>
//#include <vector>
//#include <cstdint>
// pro
//#include <space/sim/planet.hpp>
#include <space/sim/engine/chrono.hpp>
#include <space/sim/ship.hpp>
#include <space/sim/building.hpp>
#include <space/sim/building_resources.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class Empire;

// -------------------------------------------------------------------------------------------------

struct ProductionEntry {
	int32_t remaining;
	ShipTypeDefinition* ship_type;

//	void event_start(const Colony& colony, const ProductionEntry& entry);
//	void event_progress(const Colony& colony, const ProductionEntry& entry, int32_t progress);
//	void event_pause(const Colony& colony, const ProductionEntry& entry);
//	void event_continue(const Colony& colony, const ProductionEntry& entry);
//	void event_complete(const Colony& colony, const ProductionEntry& entry);
//
//	void event_reorder(const Colony& colony, const ProductionEntry& entry, int32_t from, int32_t to);
};

enum class building_result {
	success,
};

// -------------------------------------------------------------------------------------------------

class Colony {
private:
	uint64_t id;
	std::string name; // default := name of the celestial_body

	uint64_t nextBuildingID = 0;
	//	std::shared_ptr<Empire> owner;
	std::vector<Building> buildings;
	//	std::vector<Production> productionQueue;
//	std::map<BuildingType, uint32_t> buildings_statistics;
//	std::vector<std::pair<BuildingType, uint32_t>> buildings_statistics;

	boost::container::flat_map<BuildingType, uint32_t> building_counts;

public:
//private:
	std::vector<ProductionEntry> production_ship_queue;
	std::vector<ProductionEntry> production_tank_queue;
	std::vector<ProductionEntry> production_building_queue;

public:
//private:
//	Planet* planet;
//	Force* force;
	Empire* empire;
//	Surface* surface;

private:
	float tax;

	int64_t population;

	BuildingResources resources;

public:
	building_result try_building(BuildingType type, libv::vec2i position);
	void update(sim_duration dt);

public:
	template <typename Archive> void serialise(Archive& ar) {
//		ar & LIBV_NVP(name);
//		ar & LIBV_NVP(nextBuildingID);
//		ar & LIBV_NVP(buildings);
//		ar & LIBV_NVP(buildingsStatictics);
//		ar & LIBV_NVP(tax);
//		ar & LIBV_NVP(population);
//		ar & LIBV_NVP(buildingResources);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
