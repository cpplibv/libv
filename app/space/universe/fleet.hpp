// Project: libv, File: src/space/universe/fleet.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/math/vec_serial.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_memory.hpp>
#include <libv/serial/types/std_string.hpp>
#include <libv/utility/entity/entity_ptr_fwd.hpp>
//#include <libv/meta/reflection_access.hpp>
//#include <libv/serial/enable.hpp>
// std
//#include <cstdint>
#include <string>
#include <vector>
// pro
#include <space/universe/engine/screen_pickable_type.hpp>
#include <space/universe/engine/serial_id.hpp>
#include <space/universe/faction.hpp>
#include <space/universe/ids.hpp>
#include <space/universe/planet.hpp>


#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

//struct FleetOrder {
//	static constexpr bool auto_override_with_next = false;
//
//	uint64_t fleetID;
//	LIBV_REFLECTION_ACCESS(fleetID);
//	LIBV_SERIALIAZTION_ENABLE_REFLECTION();
//};
//
//// ---
//
//struct FleetOrderMovement : FleetOrder {
//	world_position position;
//	LIBV_REFLECTION_ACCESS(position);
//};
//
//struct FleetOrderStop : FleetOrder {
//	static constexpr bool auto_override_with_next = true;
//};
//
//struct FleetOrderAttackFleet : FleetOrder {
//	uint64_t targetFleetID;
//	LIBV_REFLECTION_ACCESS(targetFleetID);
//};
//
//struct FleetOrderAttackPlanet : FleetOrder {
//	uint64_t targetPlanetID;
//	LIBV_REFLECTION_ACCESS(targetPlanetID);
//};
//
//struct FleetOrderExploreAuto : FleetOrder {
//	static constexpr bool auto_override_with_next = true;
//};

// -------------------------------------------------------------------------------------------------

enum class FleetCommandType {
	movement,
	attack
//		follow,
//		merge,
//		block,
//		land,
//		repair,
//		upgrade,
//		...,
};

[[nodiscard]] constexpr inline auto operator+(FleetCommandType e) noexcept { return libv::to_underlying(e); }

// -------------------------------------------------------------------------------------------------

struct Fleet {
public:
	static constexpr inline ScreenPickableType pickingType{0.75f, 80.f};

public:
	enum class Selection : int32_t {
		notSelected = 0,
		hoverSingle = 1,
		hoverBox = 2,
		selected = 3,
		selectedHoverSingle = 4,
		selectedHoverBox = 5,
	};

private:
	struct Command {
		FleetCommandType type;
		libv::vec3f targetPosition;
		// <<< Variant?
		libv::entity_ptr<Planet> targetPlanet = nullptr;
		libv::entity_ptr<Fleet> targetFleet = nullptr;

		Command() = default; /// For de-serialization only

		Command(FleetCommandType type, libv::vec3f targetPosition) : type(type), targetPosition(targetPosition) {}

		Command(FleetCommandType type, libv::entity_ptr<Fleet> targetFleet) : type(type), targetFleet(std::move(targetFleet)) {}

		Command(FleetCommandType type, libv::entity_ptr<Planet> targetPlanet) : type(type), targetPlanet(std::move(targetPlanet)) {}

		[[nodiscard]] libv::vec3f target() const {
			if (targetPlanet)
				return targetPlanet->position;
			else if (targetFleet)
				return targetFleet->position;
			else
				return targetPosition;
		}

		template <typename Archive> void serialize(Archive& ar) {
			ar & LIBV_NVP(type);
			ar & LIBV_NVP(targetPosition);

			ar & LIBV_NVP_NAMED("targetPlanet", SerialID{targetPlanet});
			ar & LIBV_NVP_NAMED("targetFleet", SerialID{targetFleet});
		}
	};

	// Commands:
	//
	// - Stop                   -
	// - (Hold position)        -
	//
	// - Shield space           waypoint
	// - Shield planet          friendly planet
	// - Shield fleet           friendly fleet
	//
	// - Merge fleet            own fleet
	// - Movement               own planet
	// - Movement               waypoint
	// - Explore auto           -
	// - Explore region         waypoint
	//
	// - Attack fleet           hostile fleet
	// - Attack planet          hostile planet
	// - Blockade planet        hostile planet
	//
	// - Split fleet            -
	// - Follow fleet           -
	// - Track/observe fleet    foreign fleet
	//
	// - Colonize               inhabited planet

private:
	friend libv::entity_access;
	uint32_t ref_count = 0;

public:
	FleetID id = invalidFleetID;
	libv::vec3f position;
	std::string name;
//	libv::quat orientation;

	std::vector<Command> commands;
	Selection selectionStatus = Selection::notSelected;

	libv::entity_ptr<Faction> faction;

//	float speed;
	//soi_type sphere_of_influence;

//	std::vector<Order> order_queue;
	int32_t number_of_ships;
//	int32_t number_of_big_ships = 3;
//	int32_t number_of_medium_ships = 3;
//	int32_t number_of_small_ships = 3;
	float distance_travelled = 0;

//	std::vector<Ship> ships;
//	std::vector<Tank> tanks;

public:
	Fleet() = default; /// For de-serialization only
	Fleet(FleetID id, libv::vec3f position, libv::entity_ptr<Faction> faction) :
			id(id),
			position(position),
			faction(std::move(faction)) {

		// <<< Random number of ships
		auto rng = libv::xoroshiro128(+id);
		auto dist = libv::make_uniform_distribution_inclusive(1, 5000);
		number_of_ships = dist(rng);
	}

public:
	template <typename Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(id);
		ar & LIBV_NVP(position);
		ar & LIBV_NVP(name);
//		ar & LIBV_NVP(orientation);
		ar & LIBV_NVP(commands);

		if (ar.isLocal())
			ar & LIBV_NVP(selectionStatus);

		ar & LIBV_NVP_NAMED("faction", SerialID{faction});

		ar & LIBV_NVP(number_of_ships);
		ar & LIBV_NVP(distance_travelled);
	}

public:
	[[nodiscard]] constexpr inline float animation_offset() const noexcept {
		return static_cast<float>(id) * 13;
	}

	void update(libv::time_duration delta_time) {
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

	void clearCommandQueue() {
		commands.clear();
	}

	void queueMoveTo(libv::vec3f targetPosition) {
		commands.emplace_back(FleetCommandType::movement, targetPosition);
	}

	void queueAttack(libv::vec3f targetPosition) {
		commands.emplace_back(FleetCommandType::attack, targetPosition);
	}

	void queueAttack(libv::entity_ptr<Planet> target) {
		commands.emplace_back(FleetCommandType::attack, std::move(target));
	}

	void queueAttack(libv::entity_ptr<Fleet> target) {
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
};

// -------------------------------------------------------------------------------------------------

} // namespace space
