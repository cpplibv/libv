// Project: libv, File: app/space/playout.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
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
#include <space/lobby.hpp>
#include <space/log.hpp>
#include <space/universe/universe.hpp>

#include <space/network_client.hpp>
#include <space/network_server.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

// Some random buzz words:
//	Deterministic lockstep
//	article: https://gafferongames.com/post/deterministic_lockstep/
//	snapshot
//	delta update
//	playout delay buffer
//	adaptive playout delay

// -------------------------------------------------------------------------------------------------

struct msg_pdb {
	template <typename Codec>
	static constexpr inline void message_types(Codec& codec) {
		codec.template register_type<0x10, CommandChatMessage>();

		codec.template register_type<0x21, CommandFleetSpawn>();
		codec.template register_type<0x22, CommandFleetMove>();
		codec.template register_type<0x23, CommandClearFleets>();
		codec.template register_type<0x24, CommandShuffle>();

		codec.template register_type<0x30, CommandTrackView>();
		codec.template register_type<0x31, CommandCameraWarpTo>();
		//		codec.template register_type<0x32, CommandCameraMovement>();
		//		codec.template register_type<0x33, CommandMouseMovement>();
	}
};

// =================================================================================================

struct Playout {
	using encode_func_t = std::vector<std::byte>(*)(Command&);
	using apply_func_t = void(*)(Universe&, Lobby&, Command&);

	struct StateChangeEntry {
		//		TickIndex frameIndex;
		// TODO P1: Temp unique_ptr usage, replace it with a cache local solution (packed variant_queue)
		std::unique_ptr<Command> command;
		apply_func_t apply_func;
	};

protected:
	//TickIndex currentTickIndex;
	std::vector<StateChangeEntry> stateChangeEntries; // TODO P1: Use some kind of ring buffer (or deque like)

public:
	template <typename CommandT, typename... Args>
	void queue(Args&&... args) {
		aux_queue(
				std::make_unique<CommandT>(std::forward<Args>(args)...),
				+[](Universe& u, Lobby& se, Command& c) {
					apply(u, se, static_cast<CommandT&>(c));
				},
				+[](Command& c) {
//					std::string data;
//					{
//						libv::archive::BinaryOutput os(data);
//						os << LIBV_NVP_NAMED("command", static_cast<CommandT&>(c));
//					}
//					log_space.info("Encoded:\n{}", libv::hex_dump_with_ascii(data));
//					return data;
					return codec.encode(static_cast<CommandT&>(c));
				});
	}

public:
	static inline libv::serial::CodecCommon<Playout, libv::archive::Binary> codec{msg_pdb{}};

	template <typename CommandT>
	void receive(CommandT&& command) {
		stateChangeEntries.emplace_back(
				std::make_unique<CommandT>(std::move(command)),
				+[](Universe& u, Lobby& se, Command& c) {
					apply(u, se, static_cast<CommandT&>(c));
				});
	}

	void queue_from_network(const std::string_view message) {
		log_space.info("Decoding:\n{}", libv::hex_dump_with_ascii(message));
		codec.decode(*this, message);
	}

public:
	//void update_to(TickIndex nextTickIndex) {
	void update(Universe& universe) {
		for (auto& entry : stateChangeEntries) {
			Lobby* lobby = nullptr; // <<< Need a better solution for non lobby owned apply, branching, maybe
			entry.apply_func(universe, *lobby, *entry.command);
		}

		stateChangeEntries.clear();
	}
	void update(Universe& universe, Lobby& lobby) {
		for (auto& entry : stateChangeEntries)
			entry.apply_func(universe, lobby, *entry.command);

		stateChangeEntries.clear();
	}

private:
	virtual void aux_queue(std::unique_ptr<Command> command, apply_func_t apply_func, encode_func_t encode_func) = 0;

public:
	virtual	~Playout() = default;
};

// ---

class PlayoutSinglePlayer : public Playout {
public:
	PlayoutSinglePlayer() {}

private:
	virtual void aux_queue(std::unique_ptr<Command> command, apply_func_t apply_func, encode_func_t encode_func) override {
		stateChangeEntries.emplace_back(std::move(command), apply_func);
	}
};

class PlayoutMultiPlayerClient : public Playout {
	NetworkClient& network;

public:
	explicit PlayoutMultiPlayerClient(NetworkClient& network) : network(network) {}

private:
	virtual void aux_queue(std::unique_ptr<Command> command, apply_func_t apply_func, encode_func_t encode_func) override {
		(void) apply_func;

		network.send(encode_func(*command));
		// Server will echo, that will queue
	}
};

class PlayoutMultiPlayerServer : public Playout {
	NetworkServer& network;

public:
	explicit PlayoutMultiPlayerServer(NetworkServer& network) : network(network) {}

private:
	virtual void aux_queue(std::unique_ptr<Command> command, apply_func_t apply_func, encode_func_t encode_func) override {
		network.broadcast(encode_func(*command));
		stateChangeEntries.emplace_back(std::move(command), apply_func);
	}
};

// -------------------------------------------------------------------------------------------------

void apply(Universe& universe, Lobby& lobby, CommandChatMessage& command);

void apply(Universe& universe, Lobby& lobby, CommandFleetMove& command);
void apply(Universe& universe, Lobby& lobby, CommandFleetSpawn& command);
void apply(Universe& universe, Lobby& lobby, CommandClearFleets& command);
void apply(Universe& universe, Lobby& lobby, CommandShuffle& command);

void apply(Universe& universe, Lobby& lobby, CommandTrackView& command);
void apply(Universe& universe, Lobby& lobby, CommandCameraWarpTo& command);

//void apply(Universe& universe, CommandCameraMovement& command);
//void apply(Universe& universe, CommandMouseMovement& command);

// -------------------------------------------------------------------------------------------------

// =================================================================================================
// =================================================================================================
// =================================================================================================

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
