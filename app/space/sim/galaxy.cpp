// Project: libv, File: app/space/sim/galaxy.cpp

// hpp
#include <space/sim/galaxy.hpp>
// libv
#include <libv/algo/linear_find.hpp>
#include <libv/utility/projection.hpp>
// pro
#include <space/sim/engine/memory_store.hpp>
#include <space/sim/faction.hpp>
#include <space/sim/fleet.hpp>
#include <space/sim/planet.hpp>
//#include <space/sim/colony.hpp>
//#include <space/sim/empire.hpp>
//#include <space/sim/trade.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Galaxy::Galaxy(MemoryStore& memory) :
		memory(&memory) {
	factionNeutral_ = memory.faction.create(nextFactionID++, "Neutral", libv::vec4f{0.65f, 0.70f, 0.70f, 0.8f}, libv::vec4f{0.4f, 0.4f, 0.4f, 1.f});
	factions.push_back(factionNeutral_);

//	factions.push_back(memory.faction.create(nextFactionID++, "StoryTeller", aiController, libv::vec4f{0.65f, 0.70f, 0.70f, 0.8f}, libv::vec4f{0.4f, 0.4f, 0.4f, 1.f}));
//	factions.push_back(memory.faction.create(nextFactionID++, "AI", aiController, libv::vec4f{0.65f, 0.70f, 0.70f, 0.8f}, libv::vec4f{0.4f, 0.4f, 0.4f, 1.f}));
}

Galaxy::Galaxy(Galaxy&&) noexcept = default;

Galaxy& Galaxy::operator=(Galaxy&& other) & noexcept = default;
//Galaxy& Galaxy::operator=(Galaxy&& other) & noexcept {
//	kill();
//
//	nextFactionID = std::move(other.nextFactionID);
//	nextPlanetID = std::move(other.nextPlanetID);
//	nextFleetID = std::move(other.nextFleetID);
//
////	nextColonyID = std::move(other.nextColonyID);
////	nextEmpireID = std::move(other.nextEmpireID);
////	nextTradeID = std::move(other.nextTradeID);
//
//	memory = std::move(other.memory);
//	factions = std::move(other.factions);
//	planets = std::move(other.planets);
//	fleets = std::move(other.fleets);
////	colonies = std::move(other.colonies);
////	empires = std::move(other.empires);
////	trades = std::move(other.trades);
//
//	factionNeutral_ = std::move(other.factionNeutral_);
//
//	selectedFleetIDList = std::move(other.selectedFleetIDList);
//
//	return *this;
//}

Galaxy::~Galaxy() = default;

libv::entity_ptr<Faction> Galaxy::faction(std::string_view name) {
	auto cacheHit = libv::linear_find_optional(factions, name, libv::proj_indirect(&Faction::name));
	if (cacheHit)
		return *cacheHit;

	auto result = memory->faction.create(nextFactionID++, std::string(name));
	factions.push_back(result);
	return result;
}

libv::entity_ptr<Faction> Galaxy::factionNeutral() const {
	return factionNeutral_;
}

void Galaxy::kill() {
	factionNeutral_.reset();

	for (const auto& faction : factions)
		faction->kill();

	for (const auto& planet : planets)
		planet->kill();

	for (const auto& fleet : fleets)
		fleet->kill();

//		for (auto& i : colonies)
//			i.kill(dt);
//
//		for (auto& i : empires)
//			i.kill(dt);
//
//		for (auto& i : trades)
//			i.kill(dt);
}

void Galaxy::update(sim_duration delta_time) {
	// TODO P1: If dead, delete, so use erase_if_unstable iteration

	for (auto& faction : factions)
		faction->update(delta_time);

	for (auto& planet : planets)
		planet->update(delta_time);

	for (auto& fleet : fleets)
		fleet->update(delta_time);

//		for (auto& i : colonies)
//			i.update(dt);
//
//		for (auto& i : empires)
//			i.update(dt);
//
//		for (auto& i : trades)
//			i.update(dt);
}

void Galaxy::debug() {

}

// -------------------------------------------------------------------------------------------------

} // namespace space