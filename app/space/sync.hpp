// Project: libv, File: app/space/icons.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
//#include <libv/math/vec.hpp>
//#include <libv/ui/chrono.hpp>
// std
#include <memory>
#include <vector>
// pro
#include <space/command.hpp>
#include <space/session.hpp>
#include <space/state.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

// Deterministic lockstep
// article: https://gafferongames.com/post/deterministic_lockstep/
// playout delay buffer
// adaptive playout delay
//

class PlayoutDelayBuffer {
	struct StateChangeEntry {
		using apply_func_t = void(*)(SpaceState&, SpaceSession&, Command*);

//		FrameIndex frameIndex;
		std::unique_ptr<Command> command;
		apply_func_t apply_func;
	};

	std::vector<StateChangeEntry> stateChangeEntries;

public:
	template <typename T, typename... Args>
	void queue(Args&&... args) {
		stateChangeEntries.emplace_back(
//			FrameIndex{0},
			std::make_unique<T>(std::forward<Args>(args)...),
			+[](SpaceState& st, SpaceSession& se, Command* c) {
				apply(st, se, static_cast<T&>(*c));
			}
		);
	}

//	void update_to(FrameIndex nextFrameIndex) {
	void update(SpaceState& state, SpaceSession& session) {
		for (auto& entry : stateChangeEntries)
			entry.apply_func(state, session, entry.command.get());

		stateChangeEntries.clear();
	}
};

// -------------------------------------------------------------------------------------------------

//void apply(SpaceState& state, CommandPlayerKick& command) {
////	state.
//}

void apply(SpaceState& state, SpaceSession& session, CommandChatMessage& command) {
	(void) state;

	// Permission check
	// Identity check
	session.chat_entries.emplace_back(command.sender, command.message);
}

// -------------------------------------------------------------------------------------------------

void apply(SpaceState& state, SpaceSession& session, CommandFleetMove& command) {
	(void) session;

	// Permission check
	// Bound check
	state.fleets[+command.fleetID].target = command.target_position;
}

void apply(SpaceState& state, SpaceSession& session, CommandFleetSpawn& command) {
	(void) session;

	// Permission check
	// Bound check
	state.fleets.emplace_back(command.position);
}

void apply(SpaceState& state, SpaceSession& session, CommandClearFleets& command) {
	(void) session;
	(void) command;

	// Permission check
	// Bound check
	state.fleets.clear();
}

// -------------------------------------------------------------------------------------------------

void apply(SpaceState& state, SpaceSession& session, CommandTrackView& command) {
	(void) state;
	(void) session;
}

void apply(SpaceState& state, SpaceSession& session, CommandCameraWarpTo& command) {
	(void) state;
	(void) session;
}

//void apply(SpaceState& state, CommandCameraMovement& command) {
////	state.
//}
//
//void apply(SpaceState& state, CommandMouseMovement& command) {
////	state.
//}

// -------------------------------------------------------------------------------------------------

} // namespace app
