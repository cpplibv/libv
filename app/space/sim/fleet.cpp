// Project: libv, File: app/space/sim/fleet.cpp

// hpp
#include <space/sim/fleet.hpp>
// libv
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// pro
#include <space/sim/faction.hpp>
#include <space/sim/planet.hpp>


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

	auto len = 0.f;
	auto dir = orientation.forward();
	if (targetPosition != position) { // NaN protection
		const auto lenDir = (targetPosition - position).length_and_dir();
		len = lenDir.length;
		dir = lenDir.dir;
	}

	const auto targetOrientation = libv::quatf::look_at(dir, libv::vec3f(0, 0, 1.f));

	const auto angleMaxProgress = libv::deg_to_rad(360.f) * dt;
	orientation = libv::rotate_towards(orientation, targetOrientation, angleMaxProgress);

	distance_travelled += std::min(movementProgress, len);

	switch (command.type) {
	case FleetCommandType::movement:
		if (len < movementProgress) {
			position = targetPosition;
			// TODO P3: Do not waste energy, roll the excess 'movement time' into the next command
			commands.erase(commands.begin());
		} else {
			position += dir * dt;
//			position += orientation.forward() * movementProgress;
		}
		break;

	case FleetCommandType::attack:
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
//			position += orientation.forward() * movementProgress;
		}
		break;
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

//	struct posAndOrient {
//		libv::vec3f pos;
//		libv::quatf orientation;
//	};
//
//	//outer curve
//	posAndOrient trajectoryCalculation(libv::quatf source_orient, libv::vec3f source_pos, libv::vec3f target_pos, float time, float speed, float angle_speed) {
////		source_orient.axi
//		const auto source_dir = source_orient * libv::vec3f{1, 0, 0};
//		const auto[len, target_dir] = (target_pos - source_pos).length_and_dir();
//		const auto angle = std::acos(dot(normalize(source_dir), normalize(target_dir))); //might be normalized
//		const auto iteration = angle / angle_speed;
//		const auto K = speed * libv::tau / angle_speed;
//		const auto radius = speed/angle_speed; //=    K / (libv::tau)
//		const auto circle_dist = K * (angle/libv::tau);
//		const auto angle_to_circle_point = libv::pi/2 - angle/2;
////		const auto center = source_pos + radius * libv::vec3f{source_dir.y, source_dir.x, source_dir.z};
//		const auto center = source_pos + radius * libv::vec3f{source_dir.y, source_dir.x, source_dir.z};
//		const auto up = orientation.up();
//		const auto x2 = x cos θ − y sin θ
//		x sin θ + y cos θ
//		const auto[len_from_circle_point, target_dir_from_circle_point] = (target_pos - circle_point_pos).length_and_dir();
//		const auto movementProgress = speed * time;
//		if(movementProgress > circle_dist){
//			const auto dist = movementProgress - circle_dist;
//			const auto final_pos = circle_point_pos + target_dir_from_circle_point * dist;
//			const auto target_orientation = libv::quatf::angle_axis(libv::radian(std::atan2(target_dir_from_circle_point.y, target_dir_from_circle_point.x)), libv::vec3f(0, 0, 1.f));
//			return posAndOrient{final_pos, target_orientation};
//		}else{
//			const auto final_angle = movementProgress/K * libv::tau;
//		}
//	}

// -------------------------------------------------------------------------------------------------

} // namespace space
