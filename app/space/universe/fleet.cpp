// Project: libv, File: src/space/universe/fleet.hpp

// hpp
#include <space/universe/fleet.hpp>
// libv
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// pro
#include <space/universe/faction.hpp>
#include <space/universe/planet.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Fleet::Command::Command() = default; /// For de-serialization only

Fleet::Command::Command(FleetCommandType type, libv::vec3f targetPosition) :
		type(type),
		targetPosition(targetPosition) {}

Fleet::Command::Command(FleetCommandType type, libv::entity_ptr<Fleet>&& targetFleet) :
		type(type),
		targetFleet(std::move(targetFleet)) {}

Fleet::Command::Command(FleetCommandType type, libv::entity_ptr<Planet>&& targetPlanet) :
		type(type),
		targetPlanet(std::move(targetPlanet)) {}

Fleet::Command::~Command() = default;

libv::vec3f Fleet::Command::target() const {
	if (targetPlanet)
		return targetPlanet->position;
	else if (targetFleet)
		return targetFleet->position;
	else
		return targetPosition;
}

// -------------------------------------------------------------------------------------------------

Fleet::Fleet() = default; /// For de-serialization only

Fleet::~Fleet() = default;

Fleet::Fleet(FleetID id, libv::vec3f position, libv::entity_ptr<Faction> faction) :
		Base(id),
		position(position),
		faction(std::move(faction)) {

	// <<< Random number of ships
	auto rng = libv::xoroshiro128(+id);
	auto dist = libv::make_uniform_distribution_inclusive(1, 5000);
	number_of_ships = dist(rng);
}

void Fleet::kill() {
	faction.reset();
	commands.clear();
	//dead_ = true;
}

void Fleet::update(libv::time_duration delta_time) {
	if (commands.empty())
		return;

	const auto dt = static_cast<float>(delta_time.count());
	const auto speed = 1.0f;
	const auto movementProgress = dt * speed;

	auto& command = commands.front();

	const auto targetPosition = command.target();
	const auto[len, dir] = (targetPosition - position).length_and_dir();
	distance_travelled += std::min(movementProgress, len);

	switch (command.type) {

	break; case FleetCommandType::movement:
		if (len < movementProgress) {
			position = targetPosition;
			// TODO P3: Do not waste energy, roll the excess 'movement time' into the next command
			commands.erase(commands.begin());
		} else {
			position += dir * dt; //*speed, no?
		}

	break; case FleetCommandType::attack:
		if (len < movementProgress) {
			position = targetPosition;

			if (command.targetFleet)
				command.targetFleet->faction = faction;

			else if (command.targetPlanet)
				command.targetPlanet->faction = faction;

			// TODO P3: Do not waste energy, roll the excess 'movement time' into the next command
			commands.erase(commands.begin());
		} else {
			position += dir * dt;
		}

	}

//		{
//			FleetOrderMovement order;
//			const auto dir = normalize(order.position - position);
//			position = dir * speed * dt;
//		} {
//			FleetOrderStop order;
//		} {
//			FleetOrderAttackFleet order;
//			const auto target = galaxy.getFleet[order.targetFleetID];
//			const auto dir = normalize(target.position - position);
//			position = dir * speed * dt;
//		} {
//			FleetOrderAttackPlanet order;
//			const auto target = galaxy.getPlanet[order.targetPlanetID];
//			const auto dir = normalize(target.position - position);
//			position = dir * speed * dt;
//		} {
//			FleetOrderExploreAuto order;
//		}
}

void Fleet::clearCommandQueue() {
	commands.clear();
}

void Fleet::queueMoveTo(libv::vec3f targetPosition) {
	commands.emplace_back(FleetCommandType::movement, targetPosition);
}

void Fleet::queueAttack(libv::vec3f targetPosition) {
	commands.emplace_back(FleetCommandType::attack, targetPosition);
}

void Fleet::queueAttack(libv::entity_ptr<Planet> target) {
	commands.emplace_back(FleetCommandType::attack, std::move(target));
}

void Fleet::queueAttack(libv::entity_ptr<Fleet> target) {
	commands.emplace_back(FleetCommandType::attack, std::move(target));
}

//	void update_stage_2(sim_time dt) {
//		auto i = for_each_friendly_fleet_in_contact();
//		if (this_intends_to_merge_with_i()) {
//			merge_this_to_i();
//			delete_fleet_i();
//		}
//		if (i_intends_to_merge_with_this()) {
//			merge_i_to_this();
//			delete_fleet_this();
//		}
//
//		auto i = for_each_hostile_fleet_in_contact();
//		if (i_intends_to_attack_this() || this_intends_to_attack_i())
//			fleets_engage(this, i);
//	}

// -------------------------------------------------------------------------------------------------

} // namespace space
