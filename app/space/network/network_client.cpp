// Project: libv, File: app/space/network/network_client.cpp

// hpp
#include <space/network/network_client.hpp>
// libv
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/connection_he.hpp>
// std
//#include <mutex>
// pro
#include <space/game/game_thread.hpp>
#include <space/game/user.hpp>
#include <space/log.hpp>
#include <space/message/internal_events.hpp>
#include <space/network/codec.hpp>
#include <space/network/lobby.hpp>
#include <space/sim/playout/playout.hpp>
#include <space/sim/simulation.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

inline std::shared_ptr<libv::archive::BinaryInput> defer_message_processing(libv::net::mtcp::message&& m, libv::archive::BinaryInput&& iar) {
	struct Storage {
		libv::net::mtcp::message m;
		libv::archive::BinaryInput iar;

		inline Storage(libv::net::mtcp::message&& m, libv::archive::BinaryInput&& iar) :
			m(std::move(m)),
			iar(std::move(iar)) {}
	};

	auto result = std::make_shared<Storage>(std::move(m), std::move(iar));
	return std::shared_ptr<libv::archive::BinaryInput>(std::move(result), &result->iar);
}

// -------------------------------------------------------------------------------------------------

class ConnectionHandler : public libv::net::mtcp::ConnectionHandler<ConnectionHandler> {
private:
//	std::mutex mutex; // NOTE: Client network API is currently single threaded, this might change with io_context timers

	GameThread& game_thread;
	Playout& playout;
//	Simulation& simulation;
//	Universe& universe;
	Lobby lobby{game_thread};

private:
	std::string name;
public:
	bool disconnectExpected = false;

public:
//	inline ConnectionHandler(std::string_view server_address, uint16_t server_port, std::string name, GameThread& game_thread, Playout& playout, Simulation& simulation, Universe& universe) :
	inline ConnectionHandler(std::string_view server_address, uint16_t server_port, std::string name, GameThread& game_thread, Playout& playout) :
			game_thread(game_thread),
			playout(playout),
//			simulation(simulation),
//			universe(universe),
			name(std::move(name)) {
		connection.connect_async(libv::net::Address(server_address, server_port));
	}

	~ConnectionHandler() {
	}

public:
	void write(std::vector<std::byte>&& message) {
		connection.send_async(std::move(message));
	}

private:
	virtual void on_connect() override {
		log_space.trace("[{}] on_connect", name);
		const auto program_version = 0; // TODO P1: Fetch global version
		write(network_encode(CTO_Introduction{name, program_version}));
	}

	virtual void on_connect_error(error_code ec) override {
		log_space.error("[{}] on_connect {}", name, libv::net::to_string(ec));
	}

	virtual void on_receive(message&& raw_message) override {
//	virtual void on_receive(message&& m) override {
		log_space.trace("[{}] on_receive:\n{}", name, debug_binary_as_json(raw_message.as_bin()));


		try {
//			SnapshotArchive<libv::archive::BasicBinaryInput> iar{false, m.as_bin()};
//			SnapshotArchive<libv::archive::BasicBinaryInput> iar{false, raw_message.as_bin()};
			libv::archive::BinaryInput iar{raw_message.as_bin()};

//			struct MessageArchivePack {
//				message msg;
//				SnapshotArchive<libv::archive::BasicBinaryInput> iar{SnapshotType::shared, msg.as_bin()};
//
//				inline explicit MessageArchivePack(message&& msg) : msg(std::move(msg)) {}
//			};
//
//			MessageArchivePack marp{std::move(m)};

			network_codec.decode(iar, [this, &raw_message, &iar]<typename T>(T&& message) mutable {
				if constexpr(std::is_same_v<T, SnapshotLobby>)
					lobby.process(std::move(message));

//				else if constexpr(std::is_same_v<T, SnapshotUniverse>) {
//					game_thread.execute([&, snapshot = std::make_shared<SnapshotUniverse>(std::move(message))] mutable {
//						// NOTE: shared_ptr is required as std::function requires copy ctor, maybe a switch to unique_function would solve this
//						static_cast<SnapshotUniverse&>(universe) = std::move(*snapshot);
//					});
//				}
				else if constexpr(std::is_same_v<T, SimulationSnapshotTag>) {
//					playout.process_snapshot(defer_message_processing(std::move(raw_message), std::move(iar)));
//					playout.loadSimulationSnapshot(defer_message_processing(std::move(raw_message), std::move(iar)));
					playout.loadSimulationSnapshot(iar, SnapshotType::shared);

//					const auto raw_message_bytes = raw_message.as_bin();
//					const auto raw_message_consumed = iar.tell_it();
//					const auto raw_message_left = raw_message_bytes.subspan(raw_message_consumed);
//					playout.process_snapshot(std::move(raw_message), raw_message_left);

//					playout.process_snapshot(std::move(message), std::move(iar), std::move(raw_message));
//					game_thread.execute([raw_message = std::move(raw_message), it = iar.tell_it()] mutable {
////						iar.
//						// NOTE: shared_ptr is required as std::function requires copy ctor, maybe a switch to unique_function would solve this
////						static_cast<SnapshotUniverse&>(universe) = std::move(*snapshot);
////						simulation.loadSnapshot(iar);
//						const auto raw_message_bytes = raw_message.as_bin();
//						const auto raw_message_left = raw_message_bytes.last(raw_message_bytes.size() - it);
//						simulation.loadSnapshot(raw_message_left);
//					});
				}
				else if constexpr(requires { typename T::lobby_command; })
					lobby.process(std::move(message));

				else if constexpr(requires { typename T::state_command; })
					playout.process_from_network(std::move(message));

//				else if constexpr(requires { T::track_command; })
//					tracking.process(message);

				else
					return Codec::RejectMessage{};
			});

		} catch (const ProtocolViolation& e) {
			log_space.error("[{}] ProtocolViolation {}", name, e.what());
			connection.cancel_and_disconnect_async();

		} catch (const Codec::UnexpectedIDException& e) {
			log_space.error("[{}] UnexpectedIDException {}", name, libv::to_underlying(e.id));
			connection.cancel_and_disconnect_async();

		} catch (const std::exception& e) {
			log_space.error("[{}] Exception {}", name, e.what());
			connection.cancel_and_disconnect_async();
		}
	}

	virtual void on_receive_error(error_code ec, message&&) override {
		log_space.error("[{}] on_receive {}", name, libv::net::to_string(ec));
	}

	virtual void on_send(message&& m) override {
		log_space.trace("[{}] on_send:\n{}", name, debug_binary_as_json(m.as_bin()));
	}

	virtual void on_send_error(error_code ec, message&& m) override {
		log_space.error("[{}] on_send {} message would have been: {}", name, libv::net::to_string(ec), debug_binary_as_json(m.as_bin()));
	}

	virtual void on_disconnect(error_code ec) override {
		if (ec)
			log_space.error("[{}] on_disconnect, disconnectExpected {}, {}", name, disconnectExpected, libv::net::to_string(ec));
		else
			log_space.trace("[{}] on_disconnect, disconnectExpected {}", name, disconnectExpected);

		if (!disconnectExpected)
			game_thread.broadcast_on_thread(mc::RequestDestroyClient{});
	}
};

using Connection = libv::net::mtcp::Connection<ConnectionHandler>;

// =================================================================================================

struct ImplNetworkClient {
	libv::net::IOContext io_context{2};
	Connection connection;

//	ImplNetworkClient(std::string server_address, uint16_t server_port, std::string name, GameThread& game_thread, Playout& playout, Simulation& simulation, Universe& universe) :
//			connection(io_context, std::move(server_address), server_port, std::move(name), game_thread, playout, simulation, universe) {}
	ImplNetworkClient(std::string server_address, uint16_t server_port, std::string name, GameThread& game_thread, Playout& playout) :
			connection(io_context, std::move(server_address), server_port, std::move(name), game_thread, playout) {}
};

// =================================================================================================

//NetworkClient::NetworkClient(std::string server_address, uint16_t server_port, GameThread& game_thread, Playout& playout, Simulation& simulation, Universe& universe, User& user) :
//		self(std::make_unique<ImplNetworkClient>(std::move(server_address), server_port, user.name, game_thread, playout, simulation, universe)) {
NetworkClient::NetworkClient(std::string server_address, uint16_t server_port, GameThread& game_thread, Playout& playout, User& user) :
		self(std::make_unique<ImplNetworkClient>(std::move(server_address), server_port, user.name, game_thread, playout)) {
}

NetworkClient::~NetworkClient() {
	self->connection->disconnectExpected = true;
//	self->connection.connection().disconnect_async();
//	self->io_context.join();
	// For the sake of forward declared ptr
}

// -------------------------------------------------------------------------------------------------

void NetworkClient::send(std::vector<std::byte> message) {
	self->connection->write(std::move(message));
}

// -------------------------------------------------------------------------------------------------

} // namespace space
