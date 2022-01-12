// Project: libv, File: src/space/universe/fleet.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/entity/entity_ptr.hpp>
// std
#include <string>
#include <vector>
// pro
#include <space/sim/engine/chrono.hpp>
#include <space/sim/engine/entity.hpp>
#include <space/sim/engine/screen_pickable_type.hpp>
#include <space/sim/fwd.hpp>
#include <space/sim/ids.hpp>


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

class Fleet : public Entity<Fleet, FleetID, invalidFleetID> {
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
		libv::entity_ptr<Planet> targetPlanet{nullptr};
		libv::entity_ptr<Fleet> targetFleet{nullptr};

		Command(); /// For de-serialization only
		Command(FleetCommandType type, libv::vec3f targetPosition);
		Command(FleetCommandType type, libv::entity_ptr<Fleet>&& targetFleet);
		Command(FleetCommandType type, libv::entity_ptr<Planet>&& targetPlanet);
		~Command();

		[[nodiscard]] libv::vec3f target() const;

		template <typename Archive> void serialize(Archive& ar);
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

public:
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
	Fleet(); /// For de-serialization only
	Fleet(FleetID id, libv::vec3f position, libv::entity_ptr<Faction> faction);
	~Fleet();

public:
	template <typename Archive> void serialize(Archive& ar);

public:
	[[nodiscard]] constexpr inline float animation_offset() const noexcept {
		return static_cast<float>(id) * 13;
	}

	void kill();
	void update(libv::time_duration delta_time);
//	void update_stage_2(sim_time dt);

	void clearCommandQueue();
	void queueMoveTo(libv::vec3f targetPosition);
	void queueAttack(libv::vec3f targetPosition);
	void queueAttack(libv::entity_ptr<Planet> target);
	void queueAttack(libv::entity_ptr<Fleet> target);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
