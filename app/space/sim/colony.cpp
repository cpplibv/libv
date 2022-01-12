//// Project: libv, File: src/space/universe/colony.hpp
//
//// hpp
//#include <space/sim/colony.hpp>
//// std
////#include <string>
////#include <vector>
////#include <cstdint>
//// pro
////#include <space/sim/planet.hpp>
////#include <space/sim/building.hpp>
////#include <space/sim/chrono.hpp>
//
////#include <boost/serialization/shared_ptr.hpp>
////
////#include <libv/memory.hpp>
////#include <libv/math/vec.hpp>
////#include <libv/serialization/serialization.hpp>
////#include <libv/serialization/types/vec.hpp>
////
////#include <string>
////
////#include <space/sim/building_resources.hpp>
//
//// pro
//#include <space/log.hpp>
//#include <space/sim/empire.hpp>
//
//
//namespace space {
//
//// -------------------------------------------------------------------------------------------------
//
//building_result Colony::try_building(BuildingType type, libv::vec2i position) {
//	buildings.emplace_back(nextBuildingID++, type, position);
//	resources += type->resources;
//	building_counts[type]++;
//
//	return building_result::success;
//}
//
//void Colony::update(sim_duration dt) {
//	(void) dt;
//
//	// TODO P1: Things that can affect production rate:
//	//      - Lobby base settings (Could be folded under Race)
//	//      - Race / Race Traits
//	//      - Policies (council active traits)
//	//      - Technologies
//	//      - Planet (production multiplier based on planet stats)
//	//      - Planet buildings
//	//      - Planet special buildings
//	//      - Product type (colonization ship / fighter / etc...)
//	//      - Product slot (first slot 1, every else 0, except with tech idea where second slot 0.5)
//
//	// TODO P4: Secondary production tech, tech multiplier, buildings multiplier, planet multiplier, etc...
//
////	log_space.debug("{}", dt.count());
//
////	{
////		auto ship_progress = resources[BuildingResource::ProductionShip] * dt;
////		while (!production_ship_queue.empty()) {
////			auto& prod = production_ship_queue.front();
////			prod.remaining -= ship_progress;
////			if (prod.remaining > 0) {
////				break;
////			} else {
////				ship_progress = -prod.remaining;
////				prod.remaining = 0;
//////					prod.event_complete();
////				production_ship_queue.pop_front();
////			}
////		}
////	}
////
////	{
////		auto tank_progress = resources[BuildingResource::ProductionTank] * dt;
////		while (!production_tank_queue.empty()) {
////			auto& prod = production_tank_queue.front();
////			prod.remaining -= tank_progress;
////			if (prod.remaining > 0) {
////				break;
////			} else {
////				tank_progress = -prod.remaining;
////				prod.remaining = 0;
//////					prod.event_complete();
////				production_tank_queue.pop_front();
////			}
////		}
////	}
////
////	{
////		auto building_progress = resources[BuildingResource::ProductionBuilding] * dt;
////		while (true) {
////			auto building_progress_left = 0;
////			const auto scaled_prod = building_progress / production_building_queue.size();
////			std::erase_if(production_building_queue, [](auto& prod) {
////				prod.remaining -= scaled_prod;
////				if (prod.remaining > 0) {
////					return false;
////				} else {
////					building_progress_left = -prod.remaining;
////					prod.remaining = 0;
//////						prod.event_complete();
////					return true;
////				}
////			});
////			building_progress = building_progress_left;
////		}
////	}
//
////	empire->balance += resources[BuildingResource::Upkeep] * dt.count();
//
////	empire.production -= resources[BuildingResource::Upkeep] * dt;
//////		empire.workforce -=
////
//////		resources[BuildingResource::Workforce];
//////		resources[BuildingResource::Energy];
//////		resources[BuildingResource::Upkeep];
////
//////		buildingResources.clear();
//////		for (const auto& pair : oldState.buildingsStatictics) {
//////			buildingResources += (*pair.first) * pair.second;
//////		}
////
////	formulacolonyPopulation(population, morale, resources[BuildingResource::Morale], resources[BuildingResource::Fertility]);
//}
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace space
