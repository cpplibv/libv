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
#include <space/cto.hpp>
#include <space/log.hpp>
//#include <space/universe/universe.hpp>

//#include <space/network/lobby.hpp>
#include <space/network/network_client.hpp>
#include <space/network/network_server.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct msg_pdb {
	template <typename Codec>
	static constexpr inline void message_types(Codec& codec) {
		codec.template register_type<0x10, CTO_ChatMessage>();

		codec.template register_type<0x20, CTO_FleetSpawn>();
		codec.template register_type<0x21, CTO_FleetMove>();
		codec.template register_type<0x22, CTO_FleetQueueMove>();
		codec.template register_type<0x23, CTO_ClearFleets>();
		codec.template register_type<0x24, CTO_Shuffle>();
		codec.template register_type<0x25, CTO_FleetSelect>();
		codec.template register_type<0x26, CTO_FleetSelectAdd>();

		codec.template register_type<0x30, CTO_TrackView>();
		codec.template register_type<0x31, CTO_CameraWarpTo>();
		//		codec.template register_type<0x32, CTO_CameraMovement>();
		//		codec.template register_type<0x33, CTO_MouseMovement>();
	}
};

// =================================================================================================

struct Playout {
	using encode_func_t = std::vector<std::byte>(*)(void*);
	using apply_func_t = void(*)(Universe&, Lobby&, void*);

	struct StateChangeEntry {
		//		TickIndex frameIndex;
		// TODO P1: Temp shared_ptr usage, replace it with a cache local solution (packed variant_queue), (shared_ptr used for deleter)
		std::shared_ptr<void> command;
		apply_func_t apply_func;
	};

protected:
	//TickIndex currentTickIndex;
	std::vector<StateChangeEntry> stateChangeEntries; // TODO P1: Use some kind of ring buffer (or deque like)

public:
	template <typename CommandT, typename... Args>
	void queue(Args&&... args) {
		aux_queue(
				std::make_shared<CommandT>(std::forward<Args>(args)...),
				+[](Universe& u, Lobby& se, void* c) {
					reinterpret_cast<CommandT*>(c)->apply(u, se);
				},
				+[](void* c) {
//					std::string data;
//					{
//						libv::archive::BinaryOutput os(data);
//						os << LIBV_NVP_NAMED("command", static_cast<CommandT&>(c));
//					}
//					log_space.info("Encoded:\n{}", libv::hex_dump_with_ascii(data));
//					return data;
					return codec.encode(*reinterpret_cast<CommandT*>(c));
				});
	}

public:
	static inline libv::serial::CodecCommon<Playout, libv::archive::Binary> codec{msg_pdb{}};

	template <typename CommandT>
	void receive(CommandT&& command) {
		stateChangeEntries.emplace_back(
				std::make_unique<CommandT>(std::move(command)),
				+[](Universe& u, Lobby& se, void* c) {
					reinterpret_cast<CommandT*>(c)->apply(u, se);
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
			entry.apply_func(universe, *lobby, entry.command.get());
		}

		stateChangeEntries.clear();
	}
	void update(Universe& universe, Lobby& lobby) {
		for (auto& entry : stateChangeEntries)
			entry.apply_func(universe, lobby, entry.command.get());

		stateChangeEntries.clear();
	}

private:
	virtual void aux_queue(std::shared_ptr<void> command, apply_func_t apply_func, encode_func_t encode_func) = 0;

public:
	virtual	~Playout() = default;
};

// ---

class PlayoutSinglePlayer : public Playout {
public:
	PlayoutSinglePlayer() {}

private:
	virtual void aux_queue(std::shared_ptr<void> command, apply_func_t apply_func, encode_func_t encode_func) override {
		(void) encode_func;

		stateChangeEntries.emplace_back(std::move(command), apply_func);
	}
};

class PlayoutMultiPlayerClient : public Playout {
	NetworkClient& network;

public:
	explicit PlayoutMultiPlayerClient(NetworkClient& network) : network(network) {}

private:
	virtual void aux_queue(std::shared_ptr<void> command, apply_func_t apply_func, encode_func_t encode_func) override {
		(void) apply_func;

		network.send(encode_func(command.get()));
		// Server will echo, that will queue
	}
};

class PlayoutMultiPlayerServer : public Playout {
	NetworkServer& network;

public:
	explicit PlayoutMultiPlayerServer(NetworkServer& network) : network(network) {}

private:
	virtual void aux_queue(std::shared_ptr<void> command, apply_func_t apply_func, encode_func_t encode_func) override {
		network.broadcast(encode_func(command.get()));
		stateChangeEntries.emplace_back(std::move(command), apply_func);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app

// =================================================================================================
// =================================================================================================
// =================================================================================================

//using encode_fn_t = int(*)(void*);
//using decode_fn_t = int(*)(int);
//
//
//struct CodecInfo {
//	int index;
//	encode_fn_t encode;
//	decode_fn_t decode;
//};
//
///*struct Codec {
//    std::vector<CodecInfo> registry;
//} codec;*/
//
//auto& codec_registry() {
//	static std::vector<CodecInfo> registry;
//	return registry;
//}
//
//template <typename CRTP>
//int register_codec(int x) {
//	codec_registry().emplace_back(CodecInfo{x, +[](void* x){ return reinterpret_cast<CRTP*>(x)->v0; }, +[](int x){ return x; }});
//	return x;
//}
//
//struct Command {
//
//};
//
//struct CommandA : Command {
//	static inline int index = register_codec<CommandA>(3);
//
//	int v0;
//};
//
//struct CommandB : Command {
//	static inline int index = register_codec<CommandB>(5);
//
//	int v0;
//};
//
//
//int main() {
//	std::cout << "Types: " << codec_registry().size() << "\n";
//	for (const auto& info : codec_registry())
//		std::cout << info.index << "\n";
//	return 0;
//}

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
