// Project: libv, File: app/space/playout.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
//#include <libv/math/vec.hpp>
//#include <libv/ui/chrono.hpp>
#include <libv/utility/hex_dump.hpp>
#include <libv/serial/reflection.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/codec.hpp>
// std
#include <memory>
#include <random>
#include <vector>
// pro
#include <space/command.hpp>
#include <space/log.hpp>
#include <space/session.hpp>
#include <space/universe/universe.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

// Deterministic lockstep
// article: https://gafferongames.com/post/deterministic_lockstep/
// snapshot
// delta update
// playout delay buffer
// adaptive playout delay

// =================================================================================================

class PlayoutDelayBuffer {
	struct StateChangeEntry {
		using apply_func_t = void(*)(Universe&, SpaceSession&, Command*);

//		FrameIndex frameIndex;
		std::unique_ptr<Command> command;
		apply_func_t apply_func;
	};

	std::vector<StateChangeEntry> stateChangeEntries;

//	static auto codec = libv::serial::CodecClient<PlayoutDelayBuffer, libv::archive::Binary>{msg_pdb{}};
//	static CommandCodec codec;
//
//	void receive(const auto& m) {
//	}
//	void foo() {
////		codec.
//
//		codec.decode_handle([](){ }, );
//	}
//	void bar() {
//		[](auto& m) {
//
//		};
//	}

public:
	template <typename CommandT, typename... Args>
	void queue(Args&&... args) {
		stateChangeEntries.emplace_back(
//			FrameIndex{0},
			// TODO P1: Temp unique_ptr usage, replace it with a cache local solution (variant_queue)
			std::make_unique<CommandT>(std::forward<Args>(args)...),
			+[](Universe& u, SpaceSession& se, Command* c) {

				// =================================================================================================

				std::string data;
				{
					libv::archive::BinaryOutput os(data);
					os << LIBV_NVP_NAMED("command", static_cast<CommandT&>(*c));
				}
				log_space.info("Command:\n{}", libv::hex_dump_with_ascii(data));

				// =================================================================================================

				apply(u, se, static_cast<CommandT&>(*c));
			}
		);
	}

//	void update_to(FrameIndex nextFrameIndex) {
	void update(Universe& universe, SpaceSession& session) {
		for (auto& entry : stateChangeEntries)
			entry.apply_func(universe, session, entry.command.get());

		stateChangeEntries.clear();
	}
};

// =================================================================================================

struct Playout {
	PlayoutDelayBuffer buffer;
};

// -------------------------------------------------------------------------------------------------

//void apply(Universe& universe, CommandPlayerKick& command) {
////	state.
//}

inline void apply(Universe& universe, SpaceSession& session, CommandChatMessage& command) {
	(void) universe;

	// Permission check
	// Identity check
	session.chat_entries.emplace_back(command.sender, command.message);
}

// -------------------------------------------------------------------------------------------------

inline void apply(Universe& universe, SpaceSession& session, CommandFleetMove& command) {
	(void) session;

	// Permission check
	// Bound check
	universe.fleets[+command.fleetID].target = command.target_position;
}

inline void apply(Universe& universe, SpaceSession& session, CommandFleetSpawn& command) {
	(void) session;

	// Permission check
	// Bound check
	universe.fleets.emplace_back(universe.nextFleetID, command.position);
	// !!! Synchronized FleetID generation
	universe.nextFleetID = FleetID{+universe.nextFleetID + 1};
}

inline void apply(Universe& universe, SpaceSession& session, CommandClearFleets& command) {
	(void) session;
	(void) command;

	// Permission check
	// Bound check
	universe.fleets.clear();
}

inline void apply(Universe& universe, SpaceSession& session, CommandShuffle& command) {
	(void) session;
	(void) command;

	// Permission check
	// Bound check

	auto positions = std::vector<libv::vec3f>{};
	for (const auto& fleet : universe.fleets)
		positions.emplace_back(fleet.target);

	auto rng = std::mt19937_64{command.seed};

	std::ranges::shuffle(positions, rng);

	for (size_t i = 0; i < positions.size(); ++i)
		universe.fleets[i].target = positions[i];
}

// -------------------------------------------------------------------------------------------------

inline void apply(Universe& universe, SpaceSession& session, CommandTrackView& command) {
	(void) universe;
	(void) session;
}

inline void apply(Universe& universe, SpaceSession& session, CommandCameraWarpTo& command) {
	(void) universe;
	(void) session;
}

//inline void apply(Universe& universe, CommandCameraMovement& command) {
////	universe.
//}
//
//inline void apply(Universe& universe, CommandMouseMovement& command) {
////	universe.
//}

// -------------------------------------------------------------------------------------------------

// =================================================================================================
// =================================================================================================
// =================================================================================================

struct msg_pdb {
	template <typename Codec>
	static constexpr inline void message_types(Codec& codec) {
		codec.template register_type<10, CommandChatMessage>();

		codec.template register_type<10, CommandFleetSpawn>();
		codec.template register_type<11, CommandFleetMove>();
		codec.template register_type<12, CommandClearFleets>();

		codec.template register_type<30, CommandTrackView>();
		codec.template register_type<31, CommandCameraWarpTo>();
		//		codec.template register_type<32, CommandCameraMovement>();
		//		codec.template register_type<33, CommandMouseMovement>();
	}
};


// =================================================================================================
// =================================================================================================
// =================================================================================================

} // namespace app


//// =================================================================================================
//// =================================================================================================
//// =================================================================================================
//
//template <typename CRTP>
//struct NewCodec {
//	template <typename Handler>
//	struct DecodeHandleAccessType {
//		template <typename T, size_t N>
//		using result_type = std::array<T, N>;
//
//		template <uint8_t Index, typename Object>
//		auto entry() {
//			return +[](void* handler, void* object) {
//				static_cast<Handler*>(handler)->receive(*static_cast<Object*>(object));
//			};
//		}
//	};
//
//	template <typename Handler>
//	void decode_handle(Handler&& handler) {
//		// IDEA: Instantiate a access_types with decode_handle(); so the handler type is not burned into the Codec type
//		//			This could even be a bases for a state machine
//
//		static constexpr DecodeHandleAccessType<Handler> access;
//		static constexpr auto dispatch_table = CRTP::access_types(access);
//
//		dispatch(handler, dispatch_table);
////		dispatch_table[]
////		handler(types());
//	}
//};
//
//struct CommandCodec : NewCodec<CommandCodec> {
//	template <typename Access>
//	static constexpr inline auto access_types(Access& access) {
//		return Access::result_type{
//				access.template entry<10, app::CommandChatMessage>(),
//
//				access.template entry<20, app::CommandFleetSpawn>(),
//				access.template entry<21, app::CommandFleetMove>(),
//				access.template entry<22, app::CommandClearFleets>(),
//
//				access.template entry<30, app::CommandTrackView>(),
//				access.template entry<31, app::CommandCameraWarpTo>()
//				//		access.template entry<32, app::CommandCameraMovement>(),
//				//		access.template entry<33, app::CommandMouseMovement>(),
//		};
//	}
//};
//
//// =================================================================================================
//// =================================================================================================
//// =================================================================================================
//
//template <typename CRTP>
//struct NewCodec2 {
//	struct Registry {
//		NewCodec2& owner;
//	};
//};
//
//struct CommandCodec2 : NewCodec2<CommandCodec2> {
//	static void register_types(Registry& registry) {
//		registry.entry<10, app::CommandChatMessage>([](CommandCodec2& c, app::CommandChatMessage& m) {
//			c.session.message(m);
//		});
//
//		registry.entry<20, app::CommandFleetSpawn>();
//		registry.entry<21, app::CommandFleetMove>();
//		registry.entry<22, app::CommandClearFleets>();
//
//		registry.entry<30, app::CommandTrackView>();
//		registry.entry<31, app::CommandCameraWarpTo>();
//		registry.entry<32, app::CommandCameraMovement>();
//		registry.entry<33, app::CommandMouseMovement>();
//	}
//};
//
//// =================================================================================================
//// =================================================================================================
//// =================================================================================================
