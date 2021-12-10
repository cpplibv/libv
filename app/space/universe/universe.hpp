// Project: libv, File: app/space/universe/universe.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// ext
#include <boost/container/flat_set.hpp>
// libv
//#include <libv/math/constants.hpp>
#include <libv/math/vec.hpp>
#include <libv/math/vec_serial.hpp>
//#include <libv/math/quat.hpp>
//#include <libv/math/quat_serial.hpp>
#include <libv/serial/archive/binary_fwd.hpp>
#include <libv/serial/codec_message_id.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/boost_flat_map.hpp>
#include <libv/serial/types/boost_flat_set.hpp>
#include <libv/serial/types/std_vector.hpp>
#include <libv/utility/chrono.hpp>
// std
#include <map>
#include <vector>
// pro
#include <space/universe/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct ScreenPickableType {
	float radius_universe;
	float radius_screen;
};

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

public:
	FleetID id = invalidFleetID;
	libv::vec3f position;
//	libv::quat orientation;
	std::vector<Command> commands;
	Selection selectionStatus = Selection::notSelected;

public:
	Fleet() = default; // For de-serialization only
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
	}
};

// -------------------------------------------------------------------------------------------------

struct SnapshotUniverse {
	static constexpr libv::serial::CodecMessageID id{0xC0};

public:
//	float time = 0.0f;
//	float test_sin_time = 0.0f;

	FleetID nextFleetID{0};
	std::vector<Fleet> fleets;

	boost::container::flat_set<FleetID> selectedFleetIDList;

	template <class Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(nextFleetID);
		ar & LIBV_NVP(fleets);
		ar & LIBV_NVP(selectedFleetIDList);
	}
};

struct Universe : SnapshotUniverse {
public:
	void update(libv::time_duration delta_time) {
		for (auto& fleet : fleets)
			fleet.update(delta_time);
	}

public:
	void save(libv::archive::Binary::output& ar) const;
	void load(libv::archive::Binary::input& ar);

public:
	void process(CTO_FleetSpawn&& message);
	void process(CTO_FleetSelect&& message);
	void process(CTO_FleetSelectAdd&& message);
	void process(CTO_FleetClearSelection&& message);
	void process(CTO_FleetBoxSelect&& message);
	void process(CTO_FleetMove&& message);
	void process(CTO_FleetQueueMove&& message);
	void process(CTO_ClearFleets&& message);
	void process(CTO_Shuffle&& message);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
