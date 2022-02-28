// Project: libv, File: app/space/sim/universe.cpp

// hpp
#include <space/sim/universe.hpp>
// libv
#include <libv/algo/erase_if_unstable.hpp>
#include <libv/algo/linear_find.hpp>
#include <libv/utility/projection.hpp>
//#include <libv/utility/random/uniform_distribution.hpp>
//#include <libv/utility/random/xoroshiro128.hpp>
// std
//#include <algorithm>
// pro
//#include <space/sim/engine/memory_store.hpp>
#include <space/sim/faction.hpp>
#include <space/sim/engine/simulation_context.hpp>
#include <space/log.hpp>
//#include <space/message/cto.hpp>
//#include <space/sim/colony.hpp>
//#include <space/sim/empire.hpp>
//#include <space/sim/fleet.hpp>
//#include <space/sim/gen/generation.hpp>
//#include <space/sim/gen/generation_settings.hpp>
//#include <space/sim/planet.hpp>
//#include <space/sim/trade.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Universe::Universe(const std::shared_ptr<SimulationContext>& ctx) :
		ctx(ctx),
		galaxy(ctx) {
	factionControlled_ = faction("Faction 1"); // <<<

	auto factionNeutral = ctx->memory.faction.create(nextFactionID++, "Neutral", libv::vec4f{0.65f, 0.70f, 0.70f, 0.8f}, libv::vec4f{0.4f, 0.4f, 0.4f, 1.f});
	factionNeutral_ = factionNeutral;
	factions.emplace_back(std::move(factionNeutral));

//	factions.push_back(memory.faction.create(nextFactionID++, "StoryTeller", aiController, libv::vec4f{0.65f, 0.70f, 0.70f, 0.8f}, libv::vec4f{0.4f, 0.4f, 0.4f, 1.f}));
//	factions.push_back(memory.faction.create(nextFactionID++, "AI", aiController, libv::vec4f{0.65f, 0.70f, 0.70f, 0.8f}, libv::vec4f{0.4f, 0.4f, 0.4f, 1.f}));

//	universe_rng(ggs.seed),
//	galaxy(generateGalaxy(memory, std::move(ggs))) {
//	controlledFaction = galaxy.faction("Faction 1"); // <<<
//
//	factionNeutral_ = memory.faction.create(nextFactionID++, "Neutral", libv::vec4f{0.65f, 0.70f, 0.70f, 0.8f}, libv::vec4f{0.4f, 0.4f, 0.4f, 1.f});
//	factions.push_back(factionNeutral_);
//
////	factions.push_back(memory.faction.create(nextFactionID++, "StoryTeller", aiController, libv::vec4f{0.65f, 0.70f, 0.70f, 0.8f}, libv::vec4f{0.4f, 0.4f, 0.4f, 1.f}));
////	factions.push_back(memory.faction.create(nextFactionID++, "AI", aiController, libv::vec4f{0.65f, 0.70f, 0.70f, 0.8f}, libv::vec4f{0.4f, 0.4f, 0.4f, 1.f}));
	log_space.fatal("Universe {}", static_cast<void*>(this));
}

//Universe::Universe(UniverseGenerationSettings ggs) :
//}

//Universe::Universe(Universe&&) noexcept = default;
//
//Universe& Universe::operator=(Universe&& other) & noexcept {
////	galaxy.kill();
////
////	universe_rng = std::move(other.universe_rng);
////	galaxy = std::move(other.galaxy);
////	controlledFaction = std::move(other.controlledFaction);
////
////	// NOTE: Memory has to be moved last to not discard still alive objects
////	memory = std::move(other.memory);
////	// NOTE: having to reassign galaxy's memory address, would be nice to have a better solution
////	galaxy.memory = libv::make_observer_ptr(&memory);
//
//	return *this;
//}

Universe::~Universe() {
//	kill();
	log_space.fatal("~Universe {}", static_cast<void*>(this));
}

// -------------------------------------------------------------------------------------------------

void Universe::kill() {
	for (const auto& faction : factions)
		faction->kill();
	factions.clear();

//	for (const auto& colony : colonies)
//		colony->kill();
//	colonies.clear();
//
//	for (const auto& empire : empires)
//		empire->kill();
//	empires.clear();
//
//	for (const auto& trade : trades)
//		trade->kill();
//	trades.clear();
}

void Universe::update(sim_duration delta_time) {
	//	void update(tick tick_count) {
	//		for (tick i = 0; i < tick_count; ++i) {
	//			const auto tick_time = std::chrono::duration_cast<sim_duration>(std::chrono::seconds{1}) / 60.0;
	//			const auto dt = tick_time * libv::to_underlying(tick_count);
	//
	//			galaxy.update(dt);
	//		}

	libv::erase_if_unstable(factions, [&](libv::entity_ptr<Faction>& faction) {
		faction->update(delta_time);
		return faction->dead();
	});

//	libv::erase_if_unstable(colonies, [&](libv::entity_ptr<Colony>& colony) {
//		colony->update(delta_time);
//		return colony->dead();
//	});
//
//	libv::erase_if_unstable(empires, [&](libv::entity_ptr<Faction>& empire) {
//		empire->update(delta_time);
//		return empire->dead();
//	});
//
//	libv::erase_if_unstable(trades, [&](libv::entity_ptr<Faction>& trade) {
//		trade->update(delta_time);
//		return trade->dead();
//	});

	galaxy.update(delta_time);
}

//void Universe::debug() {
//	galaxy.debug();
//}

// -------------------------------------------------------------------------------------------------

const libv::entity_ptr<Faction>& Universe::faction(std::string_view name) {
	auto cacheHit = libv::linear_find_optional(factions, name, libv::proj_indirect(&Faction::name));
	if (cacheHit)
		return *cacheHit;

	auto result = ctx->memory.faction.create(nextFactionID++, std::string(name));
	return factions.emplace_back(std::move(result));
}

const libv::entity_ptr<Faction>& Universe::factionNeutral() const {
	return factionNeutral_;
}

const libv::entity_ptr<Faction>& Universe::factionControlled() const {
	return factionControlled_;
}

// -------------------------------------------------------------------------------------------------

} // namespace space
