// Project: libv, File: app/space/universe/universe.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// ext
#include <boost/container/flat_set.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/ui/chrono.hpp>
// std
#include <vector>
// pro
#include <space/universe/ids.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

//struct ScreenPickableType {
//	float radius_direct;
//	float radius_indirect;
//
//	constexpr inline ScreenPickable(float radiusDirect, float radiusIndirect) noexcept :
//		radius_direct(radiusDirect),
//		radius_indirect(radiusIndirect) {}
//};

//struct Fleet : ScreenPickable {
struct Fleet {
//	ScreenPickableType* screen_pick_type;

	enum class CommandType {
		movement,
		attack
//		follow,
//		merge,
//		block,
//		land,
//		...,
	};
//	enum class FleetState {
//		idle,
//		selected
//
//	};
//
	struct Command {
		libv::vec3f target;
		CommandType type;
//		int32_t target;
	};

public:
	FleetID id;
	libv::vec3f position;
//	bool selected;
	std::vector<Command> commands;
//	FleetState state;

public:
	explicit Fleet(FleetID id, libv::vec3f position) :
	//		ScreenPickable(50.f, 100.f),
			id(id),
			position(position) {}

public:
//	void queue_command(CommandType type, libv::vec3f target) {
//		commands.emplace_back(type, target);
////		command_arrow.add(target, color);
//	}
//
//	void queue_command(CommandType type, int32_t target) {
//
//	}

	[[nodiscard]] constexpr inline float animation_offset() const noexcept {
		return static_cast<float>(id) * 13;
	}

	void update(libv::ui::time_duration delta_time) {
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
	}
};

// -------------------------------------------------------------------------------------------------

struct Universe {
//	float angle = 0.0f;
//	float time = 0.0f;
//	float test_sin_time = 0.0f;

	FleetID nextFleetID{0};
	boost::container::flat_set<FleetID> selectedFleetIDList;
//	FleetID selectedFleetID{noSelectionID or nullFleetID};
	std::vector<Fleet> fleets;

public:
//	Universe() {
//	}

	void update(libv::ui::time_duration delta_time) {
		for (auto& fleet : fleets)
			fleet.update(delta_time);
	}

public:
	void process(CTO_FleetSpawn&& message);
	void process(CTO_FleetSelect&& message);
	void process(CTO_FleetSelectAdd&& message);
	void process(CTO_FleetBoxSelect&& message);
	void process(CTO_FleetMove&& message);
	void process(CTO_FleetQueueMove&& message);
	void process(CTO_ClearFleets&& message);
	void process(CTO_Shuffle&& message);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
