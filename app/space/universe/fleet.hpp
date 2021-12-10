// Project: libv, File: src/space/universe/fleet.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/math/vec_serial.hpp>
#include <libv/serial/serial.hpp>
//#include <libv/meta/reflection_access.hpp>
//#include <libv/serial/enable.hpp>
// std
//#include <cstdint>
#include <string>
#include <vector>
// pro
//#include <space/universe/planet.hpp>
#include <space/universe/engine/screen_pickable_type.hpp>
#include <space/universe/ids.hpp>


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
		libv::vec3f target;
		FleetCommandType type;
//		int32_t target;

		template <class Archive> void serialize(Archive& ar) {
			ar & LIBV_NVP(target);
			ar & LIBV_NVP(type);
		}
	};

	//	struct Order {
	//		uint64_t targetID;
	//		int targetType;
	//		Waypoint targetWaypoint;
	//	};

	// Orders:
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

public:
	FleetID id = invalidFleetID;
	libv::vec3f position;
//	libv::quat orientation;
	std::vector<Command> commands;
	Selection selectionStatus = Selection::notSelected;

//	float speed;
	//soi_type sphere_of_influence;

//	std::vector<Order> order_queue;
//	std::vector<Ship> ships;
//	std::vector<Tank> tanks;

public:
	Fleet() = default; /// For de-serialization only
	Fleet(FleetID id, libv::vec3f position) :
			id(id),
			position(position) {}

public:
	template <class Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(id);
		ar & LIBV_NVP(position);
//		ar & LIBV_NVP(orientation);
		ar & LIBV_NVP(commands);

//		if constexpr (ar.is_local())
//			ar & LIBV_NVP(selectionStatus);
	}

//	void queue_command(FleetCommandType type, libv::vec3f target);
//	void queue_command(FleetCommandType type, int32_t target);

	[[nodiscard]] constexpr inline float animation_offset() const noexcept {
		return static_cast<float>(id) * 13;
	}

	void update(libv::time_duration delta_time) {
		if (commands.empty())
			return;

		const auto dt = static_cast<float>(delta_time.count());
		const auto[len, dir] = (commands.front().target - position).length_and_dir();

		if (len < dt) {
			position = commands.front().target;
			commands.erase(commands.begin());
			// TODO P3: Do not waste energy, roll the excess 'movement time' into the next command
		} else
			position = position + dir * dt;

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
