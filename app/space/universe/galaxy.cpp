// Project: libv, File: src/space/universe/galaxy.hpp

// hpp
#include <space/universe/galaxy.hpp>
// libv
#include <libv/algo/linear_find.hpp>
#include <libv/utility/projection.hpp>
// pro
#include <space/universe/engine/memory_store.hpp>
#include <space/universe/faction.hpp>
#include <space/universe/fleet.hpp>
#include <space/universe/planet.hpp>
//#include <space/universe/colony.hpp>
//#include <space/universe/empire.hpp>
//#include <space/universe/trade.hpp>


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

Galaxy& Galaxy::operator=(Galaxy&&) & noexcept = default;

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
